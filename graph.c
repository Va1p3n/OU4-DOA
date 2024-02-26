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
 * Hand in date: 2024-02-20
 * 
 * Version:
 *   2024-02-20: v1.0. First hand in
*/

// ====================== PUBLIC DATA TYPES ==========================

typedef struct node {
	const char *src;
	int index;
	// array_1d *dst;
	bool seen;
} node;

typedef struct graph {
	array_2d *matrix;
	array_1d *index_map;
	int nodes_added;
	int max_nodes;
} graph;

// =================== INTERNAL FUNCTIONS ======================

int node_index(const graph *g, const char *s)
{
	for (int i = 0; i <= array_1d_high(g->index_map); i++)
	{
		if (strcmp(array_1d_inspect_value(g->index_map, i), s) == 0)
		{
			return i;
		}
	}
	
	return -1;
}

node *new_node(graph *g, const char *s)
{
	node *new_node = malloc(sizeof(node));

	new_node->src = s;
	new_node->index = g->nodes_added;
	// new_node->dst = array_1d_create(0, g->max_nodes - 1, free);
	new_node->seen = false;

	return new_node;
}

void free_node(node *n)
{
	if (n == NULL)
		return;

	free(n->src);
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

	new_graph->nodes_added = 0;
	new_graph->max_nodes = max_nodes;

	new_graph->index_map = array_1d_create(0, max_nodes - 1, free);
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
	return array_1d_has_value(g->index_map, 0);
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
	if (node_index(g, s) != -1)
		return g;

	// Adds the node to the graph
	array_1d_set_value(g->index_map, new_node(g, s), g->nodes_added);

	g->nodes_added += 1;

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
	return array_1d_inspect_value(g->index_map, node_index(g, s));
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
	

	for (int i = 0; i <= array_1d_high(g->index_map); i++)
	{
		node *node = array_1d_inspect_value(g->index_map, i);

		node->seen = false;
	}
	
	return 0;
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
	bool n1_in_graph = !strcmp(n1->src, ((node*)array_1d_inspect_value(g->index_map, n1->index))->src);
	bool n2_in_graph = !strcmp(n2->src, ((node*)array_1d_inspect_value(g->index_map, n2->index))->src);
	
	if (!n1_in_graph || !n2_in_graph)
		return NULL;
	

	int *value = malloc(sizeof(int));
	*value = 1;

	array_2d_set_value(g->matrix, value, n1->index, n2->index);

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
	dlist *neighbors = dlist_empty(free_node);

	for (int i = 0; i < g->max_nodes; i++)
	{
		int *value = array_2d_inspect_value(g->matrix, n->index, i);

		if (*value)
		{
			dlist_insert(neighbors, array_1d_inspect_value(g->index_map, i), dlist_first(neighbors));
		}
	}

	return neighbors;
}
