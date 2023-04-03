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

/*
 anzahl knoten = sum gewichte

            approx   result
  Input     Domain   Domain    Normierte
  Gewichte  Knoten   Knoten    Gewichte
  1 1 1 1   2 | 2    2 | 2    1 1 1 1
  2 2 2 2   2 | 2    2 | 2    1 1 1 1
  2 1 1 1   2 | 2    1 | 3    8/5 4/5 4/5 4/5 * 0.8
  1.6 0.8 0.8 0.8

 */

std::vector<int> doDomainDecomp_nodeweight(const Mesh& mesh, int nDomains, const std::vector<float>& nodesWeight) {
    if(not std::all_of(nodesWeight.begin(), nodesWeight.end(), [](float x){ return x > 0; })) {
        std::cout << "All input weights must be > 0\n";
        throw "Err";
    }

    float sumWeight = std::accumulate(nodesWeight.begin(), nodesWeight.end(), 0.0f);
    float faktor = mesh.nodes.size() / sumWeight;
    std::cout << "Sum " << sumWeight << " faktor " << faktor << "\n";

    // Store the information to which vertical stripe the node belongs
    struct NodeInfo {
        // Using float instead of double for the coordinate give a few % more performance in this case.
        float XorY;
        int IP;
        int vertiStripeID;
    };

    const int approxNodesPerDomain = mesh.nodes.size()/nDomains;
    const int nVerticalStripes = std::floor(std::sqrt(nDomains));
    const int nHoristripes = std::ceil(1.0*nDomains/nVerticalStripes);
    const int vertiChunksize = mesh.nodes.size()/nVerticalStripes;


    std::vector<int> nNodesPerDomain(nDomains, approxNodesPerDomain);

    /// Mit Knotengewichten braucht man nicht den Rest aufteilen. Das führt dann dazu
    /// dass die letzte Domian garkeine Knoten mehr bekommt.
//    const int rest = mesh.nodes.size() - nDomains*approxNodesPerDomain;
//    for(int i=0; i < rest; i++) {
//        nNodesPerDomain[i]++;
//    }

    std::cout << "Split mesh into domains of chunksize approximately " << approxNodesPerDomain << "\n";
    std::cout << "Domain Decomp with " << nDomains << " threads\n";
    std::cout << nVerticalStripes << " vertical stripes " << vertiChunksize << " chunksize\n";
    std::cout << nHoristripes << " horizontal stripes\n";


    std::vector<NodeInfo> nodeInfos;
    nodeInfos.reserve(mesh.nodes.size());

    // Sort the nodes in X direction. This will split the domain into vertical stripes
    for(int IP=0; IP < mesh.nodes.size(); ++IP) {
        nodeInfos.push_back(NodeInfo{float(mesh.nodes[IP].x), IP, -1});
    }

    std::sort(nodeInfos.begin(), nodeInfos.end(), [](const NodeInfo& lhs, const NodeInfo& rhs){ return lhs.XorY < rhs.XorY; } );

    // Split the nodes into nVerticalStripes
    // The asymptotic complexity of the two for loops is linear in number of nodes.
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
    float count = 0;
    for(int vertistripeID=0; vertistripeID < nVerticalStripes; ++vertistripeID) {
        // Stupid double loop... can be rewritten. But the running time is only a few sec for 5M nodes mesh..
        for(const NodeInfo& item : nodeInfos) {
            if(item.vertiStripeID == vertistripeID) {

                //count++;
                count += nodesWeight[item.IP] * faktor;

                if(count >= nNodesPerDomain[nextDomainID]) {
                    count = 0;

                    if(nextDomainID < nDomains-1) {
                        nextDomainID++;
                    }
                }

                domainIDperNode[item.IP] = nextDomainID;
            }
        }
    }

    return domainIDperNode;
}



std::vector<int> doDomainDecomp(const Mesh& mesh, int nDomains) {
    std::vector<float> nodesWeight(mesh.nodes.size(), 1.0);
    return doDomainDecomp_nodeweight(mesh, nDomains, nodesWeight);
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


    std::vector<float> nodesWeight(mesh.nodes.size());
    for(int IP=0; IP < mesh.nodes.size(); ++IP) {
        nodesWeight[IP] = mesh.nodes[IP].z > 12 ? 10 : 1;
    }

    auto _start = std::chrono::steady_clock::now();
    std::vector<int> domainIDperNode = doDomainDecomp_nodeweight(mesh, nDomains, nodesWeight);
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
