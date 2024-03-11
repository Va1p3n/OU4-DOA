# Graph implementation

## Implementation questions

#### How should the node-names, nodes and the graph be represented?
Node-names will be the only way for the main program to intract with the graph. Nodes will contain the the name of the 
node (i.e the airport code) and a boolean wether the node is seen when performing the width first search over the graph.

#### Who has the responsability for the deallocation of memmory?
The datatype should have the final responsability to deallocate memory used in the nodes. We can do this since we
know all the data that will be stored within the nodes.

#### How do I keep track if a node is seen by the width-first algorithm?
We have a flag in the node type that is a boolean for wether the node has been seen or not.

#### How do I reset the nodes between searches?
The nodes can be reset every time we perform a new search. This way we do not have to reset if we do not make another
search.

## graph.c
This implementation will be made using a connection matrix. 

## graph2.c
This implementetion will be made using a dynamic datatype. Table?

## Is_connected.c (MAIN)
This is the main program and starts by reading a directed graph from the input file (passed as a argument). It is later
suposed to be interactable and answer if two nodes are connected by doing a width-first search.

###### Compile command test
***gcc -Wall -I lib -o testimp lib/array_1d.c lib/array_2d.c lib/dlist.c test_imp.c graph.c***

***gcc -Wall -I lib -o testimp lib/dlist.c test_imp.c graph2.c***

###### Compile command is_connected
***gcc -Wall -std=c99 -o is_connected is_connected.c graph.c -I lib lib/dlist.c lib/queue.c lib/list.c lib/array_1d.c lib/array_2d.c***

***gcc -Wall -std=c99 -o is_connected_2 is_connected.c graph2.c -I lib lib/dlist.c lib/queue.c lib/list.c***
