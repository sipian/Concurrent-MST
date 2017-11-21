To run the program execute

 make input=testcases/geeks_for_geeks_prim_MST_example.txt

where in input you specify the input file path where the graph representation is given.
Graph should be UNDIRECTED and CONNECTED.

Graph Representation in the file
 
no_of_vertices
no_of_edges
vertex1 vertex2 weight1
vertex3 vertex4 weight3
......


Note since it is undirected if you have given "v1 v2 w" as input DO NOT give "v2 v1 w" as input.
Also no of edges should count "v1 v2 w" & "v2 v1 w" as 1 edge.

sample input --> graph is a 3-complete graph
3
3
0 1 23
2 1 4
2 0 5