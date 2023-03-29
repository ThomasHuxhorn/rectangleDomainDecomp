# rectangleDomainDecomp


## The Idea

## Examples

### Limon
The domain decomp of this mesh with ~1700 nodes and equal node density shows the basic idea of the rectangle domain decomp.

<table>
   <caption>Number of ghost nodes with increasing number of domains</caption>
<thead>
	<tr>
		<th rowspan="2">nDomains</th>
		<th colspan="3">ghost nodes</th>		
	</tr>
  <tr>
		<th>min</th>
		<th>max</th>		
    <th>mean</th>
	</tr>
</thead>
<tbody>
  <tr> <td>2</td>  <td>77</td> <td>83</td>  <td>80</td> </tr>
  <tr> <td>4</td>  <td>41</td> <td>71</td>  <td>53.5</td> </tr>
  <tr> <td>8</td>  <td>30</td> <td>110</td> <td>66.625</td> </tr>
  <tr> <td>16</td> <td>24</td> <td>64</td>  <td>41.25 </td> </tr>
  <tr> <td>32</td> <td>16</td> <td>54</td>  <td>37.125</td> </tr>
  <tr> <td>64</td> <td>17</td> <td>37</td>  <td>26.4062</td> </tr>
</tbody>
</table>


| Domain decomposition from 2 to 64 domains | |
| ----------- | ----------- |
| <img src="/docu/images/limon_2.png" title="2 Domains" /> | <img src="/docu/images/limon_4.png" title="4 Domains" /> |
| <img src="/docu/images/limon_8.png" title="8 Domains" /> | <img src="/docu/images/limon_16.png" title="16 Domains" /> |
| <img src="/docu/images/limon_32.png" title="32 Domains" /> |  <img src="/docu/images/limon_64.png" title="64 Domains" /> |


### Northsea
The northsea mesh with 312000 nodes has a strongly varying node density from one node per 400km² up to one node per five squaremeter. You can see the node dense in the following frist two images.

<table>
   <caption>Number of ghost nodes with increasing number of domains</caption>
<thead>
	<tr>
		<th rowspan="2">nDomains</th>
		<th colspan="3">ghost nodes</th>		
	</tr>
  <tr>
		<th>min</th>
		<th>max</th>		
    <th>mean</th>
	</tr>
</thead>
<tbody>
  <tr> <td>2</td>  <td>582</td> <td>587</td>  <td>584.5</td> </tr>
  <tr> <td>4</td>  <td>534</td> <td>1181</td> <td>860.25</td> </tr>
  <tr> <td>8</td>  <td>442</td> <td>1121</td> <td>773.625</td> </tr>
  <tr> <td>16</td> <td>243</td> <td>943</td>  <td>559.438</td> </tr>
  <tr> <td>32</td> <td>59</td>  <td>775</td>  <td>393.344</td> </tr>
  <tr> <td>64</td> <td>46</td>  <td>835</td>  <td>317.484</td> </tr>
</tbody >
</table>


| Domain decomposition from 2 to 64 domains | |
| ----------- | ----------- |
| <img src="/docu/images/northsea_nodes.png" title="Nodes" /> | <img src="/docu/images/northsea_dense.png" title="Dense" /> |
| <img src="/docu/images/northsea_2.png" title="2 Domains" /> | <img src="/docu/images/northsea_4.png" title="4 Domains" /> |
| <img src="/docu/images/northsea_8.png" title="8 Domains" /> | <img src="/docu/images/northsea_16.png" title="16 Domains" /> |
| <img src="/docu/images/northsea_32.png" title="32 Domains" /> |  <img src="/docu/images/northsea_64.png" title="64 Domains" /> |



### Worms
