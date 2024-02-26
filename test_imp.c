#include <stdio.h>
#include <graph.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dlist.h>
/**
 * @brief This is a fast test to test if a graph impletion 
 * manages simple implementations
 * 
 * made by: Ronny Wegner (c23rwr)
 * 
 */
#define MAX_SIZE 100

void error(const char* FAIL,const char* GOT)
{
    fprintf(stderr,"%s\n",FAIL);
    fprintf(stderr,"%s\n",GOT);
    exit(EXIT_FAILURE);
}
/**
 * The function `new_graph_is_empty` creates a new graph and checks if it is empty, printing an error
 * message if it is not.
 */
void new_graph_is_empty()
{
    graph *g = graph_empty(MAX_SIZE);

    if(!graph_is_empty(g))
    {
        error(  "Fail: expected new created graph to be empty",
                "Got: the newly created graph is not");
    }

    graph_kill(g);
    fprintf(stderr,"new_graph_is_empty - OK\n");
}

/**
 * The function `new_graph_has_no_edges` creates a new graph and checks if it has no edges.
 */
void new_graph_has_no_edges()
{
    graph *g = graph_empty(MAX_SIZE);
    if(graph_has_edges(g))
    {
        error(  "Fail: Excepted new graph to not have edges",
                "Got: new graph has edges");
    }

    graph_kill(g);
    fprintf(stderr,"new_graph_has_no_edges - OK\n");
}

/**
 * Creates a graph with a specified number of nodes named "nod1", "nod2", ..., "nodi".
 * 
 * @param g An existing graph pointer.
 * @param i The number of nodes to create in the graph.
 * 
 * @return A graph pointer to the newly created graph with `i` nodes.
 */
graph *new_graph_with_x_amount_of_nodes(int i)
{
    graph *g = graph_empty(MAX_SIZE);

    char str[100];
    for( int j = 1; j < i + 1;++j)
    {
        sprintf(str,"nod%d",j);
        g = graph_insert_node(g,str);
    }
    return g;
}

/**
 * The function graph_insert_a_node creates a graph, inserts a node named "node1", and checks if the
 * graph is not empty.
 */
void graph_insert_a_node()
{
    graph *g = new_graph_with_x_amount_of_nodes(1);
    if(graph_is_empty(g))
    {
        error(  "Fail: Expected a node in graph",
                "Got: The graph is still empty");
    }
    graph_kill(g);
    fprintf(stderr,"graph_insert_a_nod - OK\n");
}

/**
 * The function checks if a graph node has a self-reference.
 */
void check_if_the_nodes_selfreference()
{
    graph *g = new_graph_with_x_amount_of_nodes(1);

    if(!graph_has_edges(g))
    {
        error(  "FAIL: Expected selfreference",
                "GOT: No edges in the graph after insertion");
    }
    graph_kill(g);
    fprintf(stderr,"check_if_the_nodes_selfreference - OK\n");
        
}

/**
 * The function `get_node_from_graph` creates a graph with one node, searches for a specific node, and
 * then cleans up the graph.
 */
void get_node_from_graph()
{
    graph *g = new_graph_with_x_amount_of_nodes(1);

    if(NULL == graph_find_node(g,"nod1"))
    {
        error(  "FAIL: Didn't find a node",
                "GOT: NULL as node pointer");
    }

    graph_kill(g);
    fprintf(stderr, "get_node_from_graph - OK\n");
}

/**
 * The function creates a graph with two nodes and checks if the second node is present in the graph.
 */
void get_second_node_from_graph()
{
    graph *g = new_graph_with_x_amount_of_nodes(2);

    if(NULL == graph_find_node(g,"nod2"))
    {
        error(  "FAIL: Didn't find a node",
                "GOT: NULL as node pointer");
    }

    graph_kill(g);
    fprintf(stderr, "get_second_node_from_graph - OK\n");
}

void check_neighbors()
{

    graph *g = new_graph_with_x_amount_of_nodes(2);

    // Get the node corresponding to the node name
    node *n1 = graph_find_node(g,"nod1");
    node *n2 = graph_find_node(g,"nod2");
    
    graph_insert_edge(g,n1,n2);

    // Get the neighborlist
    dlist *neighbors = graph_neighbours(g,n1); 

    // get the neighbor node 


    dlist_pos pos = dlist_first(neighbors);

    // Hava bool to turn true if the neighbor is 
    // in the list 
    bool found_the_neighbor = false;

    // Look for the neighbor inte the list 
    while(!dlist_is_end(neighbors,pos))
    {
        node *compare_node = dlist_inspect(neighbors,pos);

        if(nodes_are_equal(n2,compare_node))
        {
            found_the_neighbor = true;
        }
        pos = dlist_next(neighbors,pos);
    }


    if(!found_the_neighbor)
    {
        error(  "FAIL: did not find the neigbor when asked after",
                "GOT: No neighbor found when edges was inserted");
    }

    dlist_kill(neighbors);
    graph_kill(g);
    fprintf(stderr, "check_neighbors - OK\n");
}

/**
 * The function `new_node_is_seen` creates a new graph with one node, checks if the node is marked as
 * seen, and reports an error if it is.
 */
void new_node_is_seen()
{
    graph *g = new_graph_with_x_amount_of_nodes(1);

    node *n = graph_find_node(g,"nod1");

    if(graph_node_is_seen(g,n))
    {
        error(  "FAIL: New node marked as seen",
                "Expected: New node not to be seen");
    }

    graph_kill(g);
    fprintf(stderr,"new_node_is_seen - OK\n");
}

/**
 * The function `node_set_to_seen` creates a graph with one node, marks the node as seen, and checks if
 * the node is marked as seen.
 */
void node_set_to_seen()
{
    graph *g = new_graph_with_x_amount_of_nodes(1);

    node *n = graph_find_node(g,"nod1");

    g = graph_node_set_seen(g,n,true);

    if(!graph_node_is_seen(g,n))
    {
        error(  "FAIL: Node marked as not seen after marked seen",
                "Expected: Node to be marked as seen");
    }

    graph_kill(g);
    fprintf(stderr,"node_set_to_seen - OK\n");

}

/**
 * The function creates a graph with one node, sets the node as seen, then checks if the node is seen
 * and prints a message accordingly.
 */
void node_set_to_seen_then_not_to_seen()
{
    graph *g = new_graph_with_x_amount_of_nodes(1);

    node *n = graph_find_node(g,"nod1");

    g = graph_node_set_seen(g,n,true);

    if(!graph_node_is_seen(g,n))
    {
        error(  "FAIL: Node read as seen",
                "Expected: Node read as not seen");
    }

    graph_kill(g);
    fprintf(stderr,"new_node_to_seen_then_not_to_seen - OK\n");

}

/**
 * The function `check_if_reset_seen_works` tests the functionality of marking a node as seen and then
 * resetting the seen status in a graph data structure.
 */

void check_if_reset_seen_works()
{
    graph *g = new_graph_with_x_amount_of_nodes(6);

    char node_name[10];
    node *n;


    // Mark every node as seen
    for(int i = 1; i <= 6; ++i)
    {
        sprintf(node_name,"nod%d",i);
        n = graph_find_node(g,node_name);

        g = graph_node_set_seen(g,n,true);
    }

    g = graph_reset_seen(g);

    // Check if every node is not seen. 
    for(int i = 1; i <= 6; ++i)
    {
        sprintf(node_name,"nod%d",i);
        n = graph_find_node(g,node_name);

        if(graph_node_is_seen(g,n))
        {
            error(  "FAIL: Wiped nodes from seen still read as seen ",
                    "Expected: Not to see seen");
        }
    }


    graph_kill(g);
    fprintf(stderr,"check_if_reset_seen_works - OK\n");
}



int main(void)
{
    new_graph_is_empty();
    new_graph_has_no_edges();
    graph_insert_a_node();
    check_if_the_nodes_selfreference();
    get_node_from_graph();
    get_second_node_from_graph();
    check_neighbors();
    new_node_is_seen();
    node_set_to_seen();
    node_set_to_seen_then_not_to_seen();
    check_if_reset_seen_works();
    printf("All test successfully completed\n");
    return 0; 
}