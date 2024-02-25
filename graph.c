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
	char *src;
	array_1d **dst;
	bool seen;
} node;

typedef struct graph {
	array_2d *nodes;
	array_1d *index_map;
	int nodes_added;
	int max_nodes;
} graph;

// =================== INTERNAL FUNCTIONS ======================

int node_index(graph *g, const char *s)
{
	for (int i = 0; i < array_1d_high(g->index_map); i++)
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
	new_node->dst = array_1d_create(0, g->max_nodes - 1, free);
	new_node->seen = false;

	return new_node;
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
	new_graph->nodes = array_2d_create(0, max_nodes - 1, 0, max_nodes - 1, free);

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
	array_1d_set_value(g->index_map, s, g->nodes_added);

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
	
}