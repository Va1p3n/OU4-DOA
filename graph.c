#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graph.h>
#include <array_1d.h>
#include <array_2d.h>

/**
 * Implemetation of graph that uses a Adjacency matrix.
 * 
 * Author: Felix Vallström (c23fvm)
 * 
 * Hand in date: 2024-03-11
 * 
 * Version:
 *   2024-02-20: v1.0. First hand in
*/

// ====================== PUBLIC DATA TYPES ==========================

typedef struct node {
	char *src;
	int index;
	bool seen;
} node;

typedef struct graph {
	array_2d *matrix;
	array_1d *index_map;
	int nodes_added;
	int max_nodes;
} graph;

// =================== INTERNAL FUNCTIONS ======================

/**
 * node_index() - gets the index of the node in the matrix and list.
 * @g: Pointer to Graph where we search for the node.
 * @s: Label for the node.
 * 
 * Returns: index of node if the label is in the graph, otherwise -1.
*/
int node_index(const graph *g, const char *s)
{
	// Iterates through the index map
	for (int i = 0; i < g->nodes_added; i++)
	{
		// Checks if the nodes label and the searched for label is the same.
		if (strcmp(((node*)array_1d_inspect_value(g->index_map, i))->src, s) == 0)
		{
			return i;
		}
	}

	// Label is not in graph.
	return -1;
}

/**
 * new_node() - Creates a new node with label.
 * @g: Pointer to Graph.
 * @s: Label to give to node.
 * 
 * Returns: Pointer to node.
*/
node *new_node(graph *g, const char *s)
{
	// Allocates space for node.
	node *new_node = malloc(sizeof(node));

	// Allocates space for and copies the label for the node.
	char *src_lbl = malloc(strlen(s) + 1);
	strcpy(src_lbl, s);

	// Assigns the label, index of the node and start seen value.
	new_node->src = src_lbl;
	new_node->index = g->nodes_added;
	new_node->seen = false;

	return new_node;
}

/**
 * free_node() - Handels the freeing of the nodes
 * @in: Pointer to a node.
 * 
 * Returns: Nothing.
*/
void free_node(void *in)
{
	node *n = in;

	if (n != NULL && n->src != NULL)
	{
		free(n->src);
		free(n);
	}
}

// =================== NODE COMPARISON FUNCTION ======================

/**
 * nodes_are_equal() - Check whether two nodes are equal.
 * @n1: Pointer to node 1.
 * @n2: Pointer to node 2.
 *
 * Returns: true if the nodes are considered equal, otherwise false.
 *
 */
bool nodes_are_equal(const node *n1,const node *n2)
{
	// Compares the node labels, same label -> same node.
	if (strcmp(n1->src, n2->src) == 0)
	{
		return true;
	}

	return false;
}

// =================== GRAPH STRUCTURE ======================

/**
 * graph_empty() - Create an empty graph.
 * @max_nodes: The maximum number of nodes the graph can hold.
 *
 * Returns: A pointer to the new graph.
 */
graph *graph_empty(int max_nodes)
{
	// Allocates memory for graph.
	graph *new_graph = malloc(sizeof(graph));

	// Assigns start values.
	new_graph->nodes_added = 0;
	new_graph->max_nodes = max_nodes;

	// Creates the adjacency matrix (array_2d) and the "translation" map (array_1d).
	new_graph->index_map = array_1d_create(0, max_nodes - 1, *free_node);
	new_graph->matrix = array_2d_create(0, max_nodes - 1, 0, max_nodes - 1, free);

	return new_graph;
}

/**
 * graph_is_empty() - Check if a graph is empty, i.e. has no nodes.
 * @g: Graph to check.
 *
 * Returns: True if graph is empty, otherwise false.
 */
bool graph_is_empty(const graph *g)
{
	// Checks if the graph has any added nodes.
	if (g->nodes_added == 0)
		return true;
	
	return false;
}

/**
 * graph_insert_node() - Inserts a node with the given name into the graph.
 * @g: Graph to manipulate.
 * @s: Node name.
 *
 * Creates a new node with a copy of the given name and puts it into
 * the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_node(graph *g, const char *s)
{
	// The node is already in the graph. This since we can not have two nodes with the same label.
	if (node_index(g, s) != -1)
		return g;

	// More nodes than said while constructing.
	if (g->nodes_added >= g->max_nodes)
	{
		fprintf(stderr, "More nodes then stated!\n");
		graph_kill(g);
		exit(EXIT_FAILURE);
	}

	// Adds the node to the graph
	array_1d_set_value(g->index_map, new_node(g, s), g->nodes_added);

	// Increase the amount of nodes in the graph.
	g->nodes_added += 1;

	// Returns the modified graph.
	return g;
}

/**
 * graph_find_node() - Find a node stored in the graph.
 * @g: Graph to manipulate.
 * @s: Node identifier, e.g. a char *.
 *
 * Returns: A pointer to the found node, or NULL.
 */
node *graph_find_node(const graph *g, const char *s)
{
	// Gets index of the node with label in s.
	int index_of_node = node_index(g, s);
	
	// The node is not in the graph.
	if (index_of_node == -1)
		return NULL;

	// Returns the node with the label in s.
	return array_1d_inspect_value(g->index_map, index_of_node);
}

/**
 * graph_node_is_seen() - Return the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to return seen status for.
 *
 * Returns: The seen status for the node.
 */
bool graph_node_is_seen(const graph *g, const node *n)
{
	// Returns the seen status for the node.
	return n->seen;
}

/**
 * graph_node_set_seen() - Set the seen status for a node.
 * @g: Graph storing the node.
 * @n: Node in the graph to set seen status for.
 * @s: Status to set.
 *
 * Returns: The modified graph.
 */
graph *graph_node_set_seen(graph *g, node *n, bool seen)
{
	// Sets the seen status for the node to seen.
	n->seen = seen;

	// Returns the graph.
	return g;
}

/**
 * graph_reset_seen() - Reset the seen status on all nodes in the graph.
 * @g: Graph to modify.
 *
 * Returns: The modified graph.
 */
graph *graph_reset_seen(graph *g)
{
	// Graph is empty, we do not need to do anything.
	if (graph_is_empty(g))
		return g;

	// Iterates through the loop and resets the seen status for all nodes.
	for (int i = 0; i < g->nodes_added; i++)
	{
		node *node = array_1d_inspect_value(g->index_map, i);
		node->seen = false;
	}
	
	// Returns the graph.
	return g;
}

/**
 * graph_insert_edge() - Insert an edge into the graph.
 * @g: Graph to manipulate.
 * @n1: Source node (pointer) for the edge.
 * @n2: Destination node (pointer) for the edge.
 *
 * NOTE: Undefined unless both nodes are already in the graph.
 *
 * Returns: The modified graph.
 */
graph *graph_insert_edge(graph *g, node *n1, node *n2)
{
	// Checks if the nodes are in the graph or not. If not it exits early.
	if (node_index(g, n1->src) == -1 || node_index(g, n2->src) == -1)
		return NULL;
	
	// Allocates space for the integer representing an edge.
	int *value = malloc(sizeof(int));
	*value = 1;

	// Sets the edge in the adjacency matrix.
	array_2d_set_value(g->matrix, value, n1->index, n2->index);

	// returns the modified graph.
	return g;
}

/**
 * graph_neighbours() - Return a list of neighbour nodes.
 * @g: Graph to inspect.
 * @n: Node to get neighbours for.
 *
 * Returns: A pointer to a list of nodes. Note: The list must be
 * dlist_kill()-ed after use.
 */
dlist *graph_neighbours(const graph *g,const node *n)
{
	// Creates empty list without a free function since we do not want the user to remove the nodes
	// that will be in the list.
	dlist *neighbors = dlist_empty(NULL);

	// Iterates through the adjacency matrix checking for neighbors.
	for (int i = 0; i < g->nodes_added; i++)
	{
		if (array_2d_has_value(g->matrix, n->index, i))
		{
			int *value = array_2d_inspect_value(g->matrix, n->index, i);

			// Add neighbor to the list if it marked as neighbor in the adjacency matrix.
			if (*value == 1)
				dlist_insert(neighbors, array_1d_inspect_value(g->index_map, i), dlist_first(neighbors));
		}
	}

	// Returns the list of neighbors.
	return neighbors;
}

/**
 * graph_kill() - Destroy a given graph.
 * @g: Graph to destroy.
 *
 * Return all dynamic memory used by the graph.
 *
 * Returns: Nothing.
 */
void graph_kill(graph *g)
{
	// Kills the adjacency matrix
	array_2d_kill(g->matrix);
	// Kills the index map and frees the resources used by the nodes.
	array_1d_kill(g->index_map);
	// Frees the remaining resources used by the graph.
	free(g);
}