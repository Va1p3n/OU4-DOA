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

int label_comp_func(const char *key1, const char *key2)
{
	return strcmp(key1, key2);
}

dlist_pos get_node_pos(dlist *nodes, const char *s)
{
	dlist_pos pos = dlist_first(nodes);

	while (!dlist_is_end(nodes, pos))
	{
		node *node_at_pos = dlist_inspect(nodes, pos);

		if (label_comp_func(s, node_at_pos->src) == 0)
		{
			return pos;
		}
		
		pos = dlist_next(nodes, pos);
	}
	
	return NULL;
}

node *new_node(const char *s)
{
	node *new_node = malloc(sizeof(node));

	char *src_lbl = malloc(strlen(s) + 1);
	strcpy(src_lbl, s);

	new_node->src = src_lbl;
	new_node->dest = dlist_empty(NULL);
	new_node->seen = false;

	return new_node;
}

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

dlist *cpy_list(dlist *in_list)
{
	dlist *out_list = dlist_empty(NULL);

	dlist_pos pos = dlist_first(in_list);

	while (!dlist_is_end(in_list, pos))
	{
		dlist_insert(out_list, dlist_inspect(in_list, pos), dlist_first(out_list));

		pos = dlist_next(in_list, pos);
	}
	
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
	graph *new_graph = malloc(sizeof(graph));

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
	// The node is already in the graph
	if (get_node_pos(g->nodes, s) != NULL)
		return g;

	node *n = new_node(s);

	// Adds the node to the graph
	dlist_insert(g->nodes, n, dlist_first(g->nodes));

	g->nodes_added = g->nodes_added + 1;

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
	n->seen = seen;

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
	if (graph_is_empty(g))
		return g;

	dlist_pos pos = dlist_first(g->nodes);

	while (!dlist_is_end(g->nodes, pos))
	{
		node *node_at_pos = dlist_inspect(g->nodes, pos);
		node_at_pos->seen = false;

		pos = dlist_next(g->nodes, pos);
	}
	
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
	if (get_node_pos(g->nodes, n1->src) == NULL || get_node_pos(g->nodes, n2->src) == NULL)
		return NULL;

	dlist_insert(n1->dest, n2, dlist_first(n1->dest));

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
	dlist_kill(g->nodes);
	free(g);
}