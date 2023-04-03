# rectangleDomainDecomp


## The Idea
The idea is to split the mesh into sqrt(nDomains) vertical stripes by sorting the nodes in X direction. Assign each node a vertical stripe. Sort the nodes in Y direction. Loop over nodes belong to the first vertical stripe and assigm as many nodes as need to the first domain. Then the second domain and so fort until no more nodes are left in the first vertical stripe. Then do the same for the next vertical stripe.
The result are rectangle-like domains.

## Examples with equal number of nodes per domain

### Limon
The domain decomp of this mesh with ~1700 nodes and equal node density shows the basic idea of the rectangle domain decomp.

<table>
   <caption>Number of ghost nodes with increasing number of domains</caption>
<thead>
	<tr>
		<th rowspan="2">nDomains</th>
		<th colspan="3">ghost nodes</th>		
    <th colspan="3">parmetis ghost nodes</th>
	</tr>
  <tr>
		<th>min</th>
		<th>max</th>		
    <th>mean</th>
    <th>min</th>
		<th>max</th>		
    <th>mean</th>
	</tr>
</thead>
<tbody>
  <tr> <td>2</td>  <td>77</td> <td>83</td>  <td>80</td>      <td>27</td> <td>27</td> <td>27.0</td> </tr>
  <tr> <td>4</td>  <td>41</td> <td>71</td>  <td>53.5</td>    <td>15</td> <td>44</td> <td>32.25</td> </tr>
  <tr> <td>8</td>  <td>30</td> <td>110</td> <td>66.625</td>  <td>18</td> <td>38</td> <td>30.125</td> </tr>
  <tr> <td>16</td> <td>24</td> <td>64</td>  <td>41.25 </td>  <td>16</td> <td>42</td> <td>26.8125</td> </tr>
  <tr> <td>32</td> <td>16</td> <td>54</td>  <td>37.125</td>  <td>12</td> <td>32</td> <td>21.78125</td> </tr>
  <tr> <td>64</td> <td>17</td> <td>37</td>  <td>26.4062</td> <td>10</td> <td>39</td> <td>21.4375</td> </tr>
</tbody>
</table>


| Domain decomposition from 2 to 64 domains | |
| ----------- | ----------- |
| <img src="/docu/images/limon_2.png" title="2 Domains" /> | <img src="/docu/images/limon_4.png" title="4 Domains" /> |
| <img src="/docu/images/limon_8.png" title="8 Domains" /> | <img src="/docu/images/limon_16.png" title="16 Domains" /> |
| <img src="/docu/images/limon_32.png" title="32 Domains" /> |  <img src="/docu/images/limon_64.png" title="64 Domains" /> |



### Northsea
The northsea mesh with 312000 nodes has a strongly varying node density from one node per 400km² up to one node per five square meter. You can see the node dense in the following frist two images.

<table>
   <caption>Number of ghost nodes with increasing number of domains</caption>
<thead>
	<tr>
		<th rowspan="2">nDomains</th>
		<th colspan="3">ghost nodes</th>		
    <th colspan="3">parmetis ghost nodes</th>
	</tr>
  <tr>
		<th>min</th>
		<th>max</th>		
    <th>mean</th>
    <th>min</th>
		<th>max</th>		
    <th>mean</th>
	</tr>
</thead>
<tbody>
  <tr> <td>2</td>  <td>582</td> <td>587</td>  <td>584.5</td>   <td>197</td> <td>202</td> <td>199.5</td> </tr>
  <tr> <td>4</td>  <td>534</td> <td>1181</td> <td>860.25</td>  <td>183</td> <td>392</td> <td>250.750</td> </tr>
  <tr> <td>8</td>  <td>442</td> <td>1121</td> <td>773.625</td> <td>38</td> <td>462</td> <td>278.625</td> </tr>
  <tr> <td>16</td> <td>243</td> <td>943</td>  <td>559.438</td> <td>58</td> <td>474</td> <td>217.0625</td> </tr>
  <tr> <td>32</td> <td>59</td>  <td>775</td>  <td>393.344</td> <td>18</td> <td>431</td> <td>195.875</td> </tr>
  <tr> <td>64</td> <td>46</td>  <td>835</td>  <td>317.484</td> <td>12</td> <td>313</td> <td>157.046875</td> </tr>
</tbody >
</table>


| Domain decomposition from 2 to 64 domains | |
| ----------- | ----------- |
| <img src="/docu/images/northsea_nodes.png" title="Nodes" /> | <img src="/docu/images/northsea_dense.png" title="Dense" /> |
| <img src="/docu/images/northsea_2.png" title="2 Domains" /> | <img src="/docu/images/northsea_4.png" title="4 Domains" /> |
| <img src="/docu/images/northsea_8.png" title="8 Domains" /> | <img src="/docu/images/northsea_16.png" title="16 Domains" /> |
| <img src="/docu/images/northsea_32.png" title="32 Domains" /> |  <img src="/docu/images/northsea_64.png" title="64 Domains" /> |



### Worms
The worms mesh with 6.3M nodes  with varying node density from one node per 400 square meters in farming areas up to one node per square meter in urban areas. You can see the node dense in the following frist two images.

<table>
   <caption>Number of ghost nodes and run time with increasing number of domains</caption>
<thead>
	<tr>
		<th rowspan="2">nDomains</th>
		<th colspan="3">ghost nodes</th>
    <th>run time</th>
    <th colspan="3">parmetis ghost nodes</th>
	</tr>
  <tr>
		<th>min</th>
		<th>max</th>		
    <th>mean</th>
    <th>seconds</th>
    <th>min</th>
		<th>max</th>		
    <th>mean</th>
	</tr>
</thead>
<tbody>
  <tr> <td>2</td>  <td>2824</td> <td>2862</td>  <td>2843</td>    <td>1.1</td>  <td>1200</td> <td>1292</td> <td>1246.0</td> </tr>
  <tr> <td>4</td>  <td>1800</td> <td>2640</td> <td>2334.25</td>  <td>1.1</td>  <td>1016</td> <td>1616</td> <td>1368.0</td> </tr>
  <tr> <td>8</td>  <td>1109</td> <td>3510</td> <td>2282.62</td>  <td>1.1</td>  <td>746</td> <td>1760</td> <td>1223.0</td> </tr>
  <tr> <td>16</td> <td>1019</td> <td>2651</td>  <td>1760.75</td> <td>1.1</td>  <td>605</td> <td>1787</td> <td>1076.375</td> </tr>
  <tr> <td>32</td> <td>900</td>  <td>2125</td>  <td>1446.62</td> <td>1.1</td>  <td>401</td> <td>1279</td> <td>827.343750</td> </tr>
  <tr> <td>64</td> <td>520</td>  <td>2004</td>  <td>1060.14</td> <td>1.1</td>  <td>271</td> <td>825</td> <td>586.453125</td> </tr>
  <tr> <td>128</td> <td>507</td> <td>1457</td> <td>785.812</td>  <td>1.1</td>  <td>242</td> <td>681</td> <td>445.781250</td> </tr>
  <tr> <td>1024</td>  <td>151</td> <td>1057</td> <td>317.413</td> <td>1.2</td> <td></td> <td></td> <td></td> </tr>
  <tr> <td>8196</td>  <td>35</td>  <td>889</td>  <td>127.689</td> <td>1.6</td> <td></td> <td></td> <td></td> </tr>
  <tr> <td>32784</td> <td>11</td>  <td>533</td>  <td>70.4163</td> <td>2.1</td> <td></td> <td></td> <td></td> </tr>
  <tr> <td>65568</td> <td>8</td>   <td>373</td>  <td>52.2802</td> <td>2.6</td> <td></td> <td></td> <td></td> </tr>
</tbody >
</table>

Note: Only the time for the domain decomp count, no I/O. Speedtest was run on a 12 years old Intel i7-2640M CPU @ 2.80GHz with compiler optimization -O2.

| Domain decomposition from 2 to 64 domains | |
| ----------- | ----------- |
| <img src="/docu/images/worms_nodes.png" title="Nodes" /> | <img src="/docu/images/worms_dense.png" title="Dense" /> |
| <img src="/docu/images/worms_2.png" title="2 Domains" /> | <img src="/docu/images/worms_4.png" title="4 Domains" /> |
| <img src="/docu/images/worms_8.png" title="8 Domains" /> | <img src="/docu/images/worms_16.png" title="16 Domains" /> |
| <img src="/docu/images/worms_32.png" title="32 Domains" /> |  <img src="/docu/images/worms_64.png" title="64 Domains" /> |



## Examples with nnod weights

### Limon
To show the domain decomposition with node weights the mesh depth is used. For every node with a depth > 12 the weight is set to 10, otherwiese 1. The resulting weight is shown in the first image. Where blue is weight of 1 and green of 10.
As you can see, on the left side of the mesh there are less domain.

| Domain decomposition with node weights and 64 domains | |
| ----------- | ----------- |
| <img src="/docu/images/limon_depth_ab12.png" title="Weights" /> | <img src="/docu/images/limon_nodeweight_64.png" title="64 Domains" /> |


