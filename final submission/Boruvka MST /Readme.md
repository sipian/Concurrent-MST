# Readme

To run the program, execute the following command

```
$ make input = testcases/geeks_for_geeks_prim_MST_example.txt
```

where in input you specify the input file path where the graph representation is given.

### Note: 

* Graph should be UNDIRECTED and CONNECTED.
* Since the graph is undirected if input edge is given as "v1 v2 w" then DO NOT give "v2 v1 w" as another input edge.
* Graph Representation in the file
 
```
no_of_vertices
no_of_edges
vertex1 vertex2 weight1
vertex3 vertex4 weight3
......
```

### Sample input:

```
sample input --> graph is a 3-complete graph
3
3
0 1 23
2 1 4
2 0 5
```