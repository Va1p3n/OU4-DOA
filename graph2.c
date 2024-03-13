#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graph.h>
#include <dlist.h>

/**
 * Implemetation of graph that uses a dynamic datatype.
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
	dlist *dest;
	bool seen;
} node;

typedef struct graph {
	dlist *nodes;
	int nodes_added;
	int max_nodes;
} graph;

// =================== INTERNAL FUNCTIONS ======================

/**
 * label_com_func() - Compares two labels.
 * @label1: A label (string).
 * @label2: A label (string).
 * 
 * Returns: 0 if equal, undefined (<0 or >0) if not equal.
*/
int label_comp_func(const char *label1, const char *label2)
{
	return strcmp(label1, label2);
}

/**
 * get_node_pos() - Gets the position of a node in the list
 * @nodes: Pointer to dlist.
 * @s: A label (string).
 * 
 * Returns: A dlist position if the node with the specified label is in the list, otherwise NULL.
*/
dlist_pos get_node_pos(dlist *nodes, const char *s)
{
	dlist_pos pos = dlist_first(nodes);

	// Iterates through the list untill pos is the end position.
	while (!dlist_is_end(nodes, pos))
	{
		node *node_at_pos = dlist_inspect(nodes, pos);

		// Compares the labels, if the same we return the position.
		if (label_comp_func(s, node_at_pos->src) == 0)
			return pos;
		
		// Takes the next position in the list.
		pos = dlist_next(nodes, pos);
	}
	
	// Node is not in list.
	return NULL;
}

/**
 * new_node() - Creates a new node.
 * @s: A label (string) for the new node.
 * 
 * Returns: A pointer to the node.
*/
node *new_node(const char *s)
{
	// Allocates memory for the node.
	node *new_node = malloc(sizeof(node));

	// Assigns memory and copies the label for the node.
	char *src_lbl = malloc(strlen(s) + 1);
	strcpy(src_lbl, s);

	// Assigns the label, creates a neighbor list and a start seen value.
	new_node->src = src_lbl;
	new_node->dest = dlist_empty(NULL);
	new_node->seen = false;

	return new_node;
}

/**
 * free_node() - Frees the resources used by a node
 * @in: A pointer to a node.
 * 
 * Returns: Nothing.
*/
void free_node(void *in)
{
	node *n = in;

	if (n != NULL && n->src != NULL)
	{
		free(n->src);
		dlist_kill(n->dest);
		free(n);
	}
}

/**
 * cpy_list() - Makes a reverse copy of a neighbor list for a node.
 * @in_list: A dlist to copy.
*/
dlist *cpy_list(dlist *in_list)
{
	// Creates the outlist
	dlist *out_list = dlist_empty(NULL);

	dlist_pos pos = dlist_first(in_list);

	// Iterates through the in_list and adds every node it has to the outlist.
	while (!dlist_is_end(in_list, pos))
	{
		dlist_insert(out_list, dlist_inspect(in_list, pos), dlist_first(out_list));

		pos = dlist_next(in_list, pos);
	}
	
	// Returns the copied list.
	return out_list;
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
	// Allocates space for the graph.
	graph *new_graph = malloc(sizeof(graph));

	// Saves the max amount of nodes.
	new_graph->max_nodes = max_nodes;
	
	new_graph->nodes = dlist_empty(*free_node);

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
	return dlist_is_empty(g->nodes);
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
	if (get_node_pos(g->nodes, s) != NULL)
		return g;

	// More nodes than said while constructing.
	if (g->nodes_added >= g->max_nodes)
	{
		fprintf(stderr, "More nodes then stated!\n");
		graph_kill(g);
		exit(EXIT_FAILURE);
	}
	
	// The new node
	node *n = new_node(s);

	// Adds the node to the graph
	dlist_insert(g->nodes, n, dlist_first(g->nodes));

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
	// Gets a position for the node in the graph, NULL if node is not in graph.
	dlist_pos pos_of_node = get_node_pos(g->nodes, s);

	// Node is not in graph
	if (pos_of_node == NULL)
		return NULL;
	
	// Node found, returns node
	return dlist_inspect(g->nodes, pos_of_node);
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

	dlist_pos pos = dlist_first(g->nodes);

	// Iterates through the list and resets the seen status for all nodes.
	while (!dlist_is_end(g->nodes, pos))
	{
		node *node_at_pos = dlist_inspect(g->nodes, pos);
		node_at_pos->seen = false;

		pos = dlist_next(g->nodes, pos);
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
	if (get_node_pos(g->nodes, n1->src) == NULL || get_node_pos(g->nodes, n2->src) == NULL)
		return NULL;

	// Inserts n2 into n1:s neighbors list.
	dlist_insert(n1->dest, n2, dlist_first(n1->dest));

	// Returns the graph.
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
	// Returns a copy of n:s neighbor list. Copy because we do not want the user to kill the list within the node.
	return cpy_list(n->dest);
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
	// Kils the list of nodes and frees the resources used by the nodes.
	dlist_kill(g->nodes);
	// Frees the remaining resources used by the graph.
	free(g);
}