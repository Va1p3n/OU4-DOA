#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include <graph.h>
#include <queue.h>

#define MAXNODENAME 40
#define BUFSIZE 400

/**
 * With the help of graph traverses a inputed map and searches
 * if two nodes are connecting of not.
 * 
 * Author: Felix Vallström (c23fvm)
 * 
 * Hand in date: 2024-02-20
 * 
 * Version:
 *   2024-02-20: v1.0. First hand in
*/

// ====== HELPER FUNCTIONS

/**
 * @brief Find position of first non-whitespace character.
 *
 * @param s - string to search.
 * @return int - Returns the position of the first non-whitespace character, or -1 if not found.
 */
int first_non_white_space(const char *s)
{
    int i = 0; // Start at first char.

    // Advance as long as we're loooking at white-space until we hit EOL.
    while (s[i] && isspace(s[i])) {
        i++;
    }
    // We found a non-whitespace char.
    if (s[i]) {
        // It was a proper characted. Return its position.
        return i;
    } else {
        // It was the null terminator. Return fail.
        return -1;
    }
}

/**
 * @brief Determine if the string is blank.
 *
 * @param s - string to search.
 * @return true if the line contains only whitespace chars.
 * @return false if the line contains at least one non-whitespace char.
 */

bool line_is_blank(const char *s)
{
    // Line is blank if it only contained white-space chars.
    return first_non_white_space(s) < 0;
}

/**
 * @brief Determina if the string is a comment line.
 *
 * @param s - string to search.
 * @return true if the line is a comment line.
 * @return false if the line is not a comment line.
 *
 * A comment line has a hash sign '#' as the first non-whitespace char on the line.
 */
bool line_is_comment(const char *s)
{
    int i = first_non_white_space(s);
    return (i >= 0 && s[i] == '#');
}

/**
 * @brief Extract node names from a line from the map file.
 *
 * @param buf - Input buffer.
 * @param n1 - Output buffer for the first node name. Must be at least MAXNODENAME+1 long.
 * @param n2 - Ditto for the second node name.
 * @return int - Returns the number of correctly parsed node names. If the return value is 2, both n1
 * and n2 contain node names. If the return value is less than 2, parsing of at least one node name
 * failed, in which case the content of n1 and n2 are undefined.
 */
int parse_map_line(const char *buf, char *n1, char *n2)
{
    // Create a format string the will do the work.
    char fmt[20];
    // This will generate the format string " %40s %40s" if MAXNODENAME is 40.
    snprintf(fmt, sizeof(fmt), " %%%ds %%%ds", MAXNODENAME, MAXNODENAME);

    // sscanf does all the necessary parsing.
    // Node names must be separated by whitespace.
    // Whitespace before the first node name is allowed.
    // Anything after the second node name is ignored.
    int n = sscanf(buf, fmt, n1, n2);

    // The return value from sscanf contains the number of properly parsed format codes, i.e. the
    // number of node names.
    return n;
}

// ====== START

graph *add_edge_to_map(graph *map, char *src, char *dst)
{
	map = graph_insert_node(map, src);
	map = graph_insert_node(map, dst);

	node *src_node = graph_find_node(map, src);
	node *dst_node = graph_find_node(map, dst);

	map = graph_insert_edge(map, src_node, dst_node);

	return map;
}

graph *parse_map(FILE *file)
{
	graph *map;
	int parsed = 0;
	char buffer[BUFSIZE];

	while (fgets(buffer, 60, file) != NULL)
	{
		if (!line_is_blank(buffer) && !line_is_comment(buffer))
		{
			if (parsed == 0)
			{
				int nodes;
				sscanf(&buffer[first_non_white_space(buffer)], "%d", &nodes);
				map = graph_empty(nodes);
				
				parsed += 1;
			}
			else
			{
				char src[MAXNODENAME];
				char dst[MAXNODENAME];
				
				if (parse_map_line(buffer, src, dst) != 2)
				{
					fprintf(stderr, "Parsing line error, make sure map file is correct!\n");
					exit(EXIT_FAILURE);
				}

				map = add_edge_to_map(map, src, dst);
			}
		}
	}

	fclose(file);

	return map;
}

bool find_path(graph *g, node *src, node *dest)
{
	// Reset the seen status of the nodes in the map.
	graph_reset_seen(g);

	// Sets the startnode as seen and adds it to the processing queue.
	// Queue does not get a free_func since we do not want to free the nodes.
	g = graph_node_set_seen(g, src, true);
	queue *nodes_to_process = queue_enqueue(queue_empty(NULL), src);

	// This loop ensures that all nodes that need to be processed gets processed.
	while (!queue_is_empty(nodes_to_process))
	{
		// Gets the node that we will look at this iteration.
		node *node_now_process = queue_front(nodes_to_process);
		nodes_to_process = queue_dequeue(nodes_to_process);

		// This checks if the node is the one that we are looking for.
		if (nodes_are_equal(dest, node_now_process))
		{
			// Kills the queue since we found our node.
			queue_kill(nodes_to_process);
			return true;
		}

		// Get all of the neighboring nodes.
		dlist *node_neighbors = graph_neighbours(g, node_now_process);

		while (!dlist_is_empty(node_neighbors))
		{
			// Get the first neighbor in the list
			node *now_first_neighbor_node = dlist_inspect(node_neighbors, dlist_first(node_neighbors));

			// If the node is not previously seen, we add it to the process queue and mark it as seen.
			if (!graph_node_is_seen(g, now_first_neighbor_node))
			{
				g = graph_node_set_seen(g, now_first_neighbor_node, true);
				nodes_to_process = queue_enqueue(nodes_to_process, now_first_neighbor_node);
			}
			
			// Remove the neighbor from the queue.
			dlist_remove(node_neighbors, dlist_first(node_neighbors));
		}

		// Kills the list to not cause memory leaks.
		dlist_kill(node_neighbors);
	}
	
	// Kills the queue since we are now done.
	queue_kill(nodes_to_process);

	// Node was not found.
	return false;
}

void check_nodes_search(graph *map, char *buf1, char *buf2)
{
	// Fetches the nodes if they exist
	node *src_node = graph_find_node(map, buf1);
	node *dest_node = graph_find_node(map, buf2);

	// If the start node does not exist we say that and go back.
	if (src_node == NULL)
	{
		fprintf(stderr, "Could not find node %s\n", buf1);
		return;
	}

	// If the destination node does not exist we say that and go back.
	if (dest_node == NULL)
	{
		fprintf(stderr, "Could not find node %s\n", buf2);
		return;
	}
	
	// Look for a way between the nodes, if it exist we say that and if it does not we say that.
	if (find_path(map, src_node, dest_node))
		fprintf(stderr, "There is a path from %s to %s.\n", buf1, buf2);
	else
		fprintf(stderr, "There is no path from %s to %s.\n", buf1, buf2);

}

int main(int argc, char const *argv[])
{
	bool quit = false;

	if (argc < 2 || argc > 2)
	{
		fprintf(stderr, "Expected one argument to map file...\n");
		exit(EXIT_FAILURE);
	}
	
	graph *map = parse_map(fopen(argv[1], "r"));

	char input_buffer[MAXNODENAME * 2];

	do
	{
		printf("Enter origin and destination (quit to exit): ");
		if (!fgets(input_buffer, sizeof(input_buffer), stdin))
		{
			fprintf(stderr, "Error reading input!\n");
			return EXIT_FAILURE;
		}
		
		char src_lbl[MAXNODENAME];
		char dest_lbl[MAXNODENAME];
		int parsed = sscanf(input_buffer, "%40s %40s", src_lbl, dest_lbl);

		if (parsed == EOF)
		{
			fprintf(stderr, "Error parsing input!\n");
			return EXIT_FAILURE;
		}
		
		if (parsed == 1 && strcmp(src_lbl, "quit") == 0)
		{
			printf("Normal exit.\n");
			quit = true;
		}
		else if (parsed == 2)
		{
			check_nodes_search(map, src_lbl, dest_lbl);
		}
		else
		{
			fprintf(stderr, "Invalid input..\n");
		}
	} while (!quit);
	

	graph_kill(map);
	
	return 0;
}
