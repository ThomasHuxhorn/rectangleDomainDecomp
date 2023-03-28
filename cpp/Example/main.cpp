#include <iostream>

#include <vector>
#include <array>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <set>
#include <ranges>
#include <numeric>

#include "Mesh.hpp"
#include "writeMesh.hpp"
#include "readMesh.hpp"


std::vector<int> doComainDecomp(const Mesh& mesh, int nDomains) {
    // Store the information to which vertical stripe the node belongs
    struct NodeInfo {
        // Using float instead of double for the coordinate give a few % more performance in this case.
        float XorY;
        int IP;
        int vertiStripeID;
    };


    const int nVerticalStripes = std::floor(std::sqrt(nDomains));
    const int nHoristripes = std::ceil(1.0*nDomains/nVerticalStripes);
    const int approxChunksize = mesh.nodes.size()/nDomains;
    const int vertiChunksize = mesh.nodes.size()/nVerticalStripes;

    std::cout << "Domain Decomp with " << nDomains << " threads\n";
    std::cout << "Split mesh into domains of chunksize approximately " << approxChunksize << "\n";
    std::cout << nVerticalStripes << " vertical stripes " << vertiChunksize << " chunksize\n";
    std::cout << nHoristripes << " horizontal stripes\n";

    std::vector<int> chunksizePerDomain(nDomains, approxChunksize);

    const int rest = mesh.nodes.size() - nDomains*approxChunksize;
    for(int i=0; i < rest; i++) {
        chunksizePerDomain[i]++;
    }

    std::vector<NodeInfo> nodeInfos;
    nodeInfos.reserve(mesh.nodes.size());

    // Sort the nodes in X direction. This will split the domain into vertical stripes
    for(int IP=0; IP < mesh.nodes.size(); ++IP) {
        nodeInfos.push_back(NodeInfo{float(mesh.nodes[IP].x), IP, -1});
    }

    std::sort(nodeInfos.begin(), nodeInfos.end(), [](const NodeInfo& lhs, const NodeInfo& rhs){ return lhs.XorY < rhs.XorY; } );

    // Split the nodes into nVerticalStripes
    size_t IP_sorted = 0;
    for(int vertistripeID=0; vertistripeID < nVerticalStripes; ++vertistripeID) {
        for(int i=0; i < vertiChunksize; ++i) {
            nodeInfos[IP_sorted].vertiStripeID = vertistripeID;
            IP_sorted++;
        }
    }

    for(; IP_sorted < mesh.nodes.size(); ++IP_sorted) {
        nodeInfos[IP_sorted].vertiStripeID = nVerticalStripes-1;
    }

    // Sort the nodes in Y direction. This will split the vertical stripes horizontally and create rectangles
    for(NodeInfo& item : nodeInfos) {
        item.XorY = mesh.nodes[item.IP].y;
    }

    std::sort(nodeInfos.begin(), nodeInfos.end(), [](const NodeInfo& lhs, const NodeInfo& rhs){ return lhs.XorY < rhs.XorY; } );

    std::vector<int> domainIDperNode(mesh.nodes.size(), -1);

    int nextDomainID = 0;
    int count = 0;
    for(int vertistripeID=0; vertistripeID < nVerticalStripes; ++vertistripeID) {
        // Stupid double loop... can be rewritten. But the running time is only a few sec for 5M nodes mesh..
        for(const NodeInfo& item : nodeInfos) {
            if(item.vertiStripeID == vertistripeID) {
                domainIDperNode[item.IP] = nextDomainID;
                count++;

                if(count == chunksizePerDomain[nextDomainID]) {
                    count = 0;

                    if(nextDomainID < nDomains-1) {
                        nextDomainID++;
                    }
                }
            }
        }
    }

    return domainIDperNode;
}

void printNodesPerDomain(const Mesh& mesh, int nDomains, const std::vector<int>& domainIDperNode) {
    const int approxChunksize = mesh.nodes.size()/nDomains;

    // Calc number of nodes per domain
    std::vector<int> nodesPerDomain(nDomains);
    for(int domainID : domainIDperNode) {
        nodesPerDomain[domainID]++;
    }

    // Calc number of ghost nodes per domain
    std::vector<std::set<int>> ghostsNodesPerDomain(std::max(size_t(nDomains), mesh.nodes.size()));
    for(const Element& ele : mesh.elements) {
        if(domainIDperNode[ele[0]] != domainIDperNode[ele[1]]) {
            ghostsNodesPerDomain[domainIDperNode[ele[0]]].insert(ele[1]);
            ghostsNodesPerDomain[domainIDperNode[ele[1]]].insert(ele[0]);
        }

        if(domainIDperNode[ele[1]] != domainIDperNode[ele[2]]) {
            ghostsNodesPerDomain[domainIDperNode[ele[1]]].insert(ele[2]);
            ghostsNodesPerDomain[domainIDperNode[ele[2]]].insert(ele[1]);
        }

        if(domainIDperNode[ele[2]] != domainIDperNode[ele[0]]) {
            ghostsNodesPerDomain[domainIDperNode[ele[2]]].insert(ele[0]);
            ghostsNodesPerDomain[domainIDperNode[ele[0]]].insert(ele[2]);
        }
    }

    std::vector<int> ghostsPerDomain(nDomains);
    for(int domainID=0; domainID < nDomains; ++domainID) {
        ghostsPerDomain[domainID] = ghostsNodesPerDomain[domainID].size();
    }

    // Print details for every domain
    std::cout << "Domain nNodes nGhosts %ghosts %total %chunk\n";
    for(int domainID=0; domainID < nDomains; ++domainID) {
        std::cout << domainID << " " << nodesPerDomain[domainID] << " " << ghostsPerDomain[domainID] << " " << 100.0f*ghostsPerDomain[domainID]/nodesPerDomain[domainID];
        std::cout << " " << 100.0f*nodesPerDomain[domainID]/mesh.nodes.size() << " " << 100.0f*nodesPerDomain[domainID]/approxChunksize << "\n";
    }
    std::cout << "\n";

    const std::ranges::minmax_result r = std::ranges::minmax(ghostsPerDomain);
    double mean = std::accumulate(ghostsPerDomain.begin(), ghostsPerDomain.end(), 0.0)/nDomains;


    // Print summary
    std::cout << "nThreads min max mean ghost nodes\n";
    std::cout << nDomains << " " << r.min << " " << r.max << " " << mean << "\n";
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        std::cout << "usage: programm mesh.dat nThreads\n";
        return EXIT_FAILURE;
    }

    const int nDomains = std::stoi(argv[2]);

    Mesh mesh = readMeshXFN(argv[1]);
    std::cout << "Mesh with " << mesh.nodes.size() << " nodes and " << mesh.elements.size() << " elements\n";


    auto _start = std::chrono::steady_clock::now();
    std::vector<int> domainIDperNode = doComainDecomp(mesh, nDomains);
    auto end = std::chrono::steady_clock::now();

    printNodesPerDomain(mesh, nDomains, domainIDperNode);

    std::cout << "\ndomain decomp took " << std::chrono::duration<double>(end-_start).count() << " sec\n";

    // copy result back to mesh for visulation
    for(int IP=0; IP < mesh.nodes.size(); ++IP) {
        mesh.nodes[IP].z = domainIDperNode[IP];
    }

    writeMeshXFN(mesh);

    return EXIT_SUCCESS;
}
