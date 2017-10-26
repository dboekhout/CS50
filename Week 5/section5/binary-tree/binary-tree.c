/* Binary Search Tree
 * Write an insert function for a binary search tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* array with numbers to be inserted into the tree */
#define ARRAY_SIZE 20
int array[ARRAY_SIZE] = {8, 7, 15, 2, 19, 18, 0, 12, 11, 10, 16, 13, 14, 17, 5, 6, 4, 1, 3, 9};

/* the node struct */
typedef struct node
{
	int key_value;
	struct node* left;
	struct node* right;
} node;

// TODO: implement insert
void insert(int key, struct node* tree);

/* declaration of provided functions */
node* create_an_example_tree();
node* create_node(int value);
void print_tree(node *tree);

/* main function. Iterates over the elements of array and inserts
 * them into the tree.
 */
int main()
{
	// Create a root node containing the first element of the array
	node* root = create_node(array[0]);

	// Use insert to iteratively add the emaining elements to the array
	for(int i = 1; i < ARRAY_SIZE; i++)
	{
		insert(array[i], root);
	}

	// TODO: Comment out this line!!!
	//root = create_an_example_tree();

	// print result
	print_tree(root);


	// TODO (bonus): delete tree (free all memory)
}


/* insert: Inserts a new node into the tree.
 */
void insert(int key, struct node* tree)
{
	// TODO: implement!!!

	/* Tips:
	 * - This function should keep the tree sorted. I, e.,
	 *   for all nodes it holds that the right sub-tree should
	 *   only contain bigger nodes, and the left should only contain
	 *   smaller nodes.
	 * - The easiest way is to implement this function recursively.
	 *   How? Remember the tree must remain sorted. If the key is smaller
	 *   than the tree->value it should be added to the left, otherwise it
	 *   should be added to the right.
	 * - For example, let’s say `key < tree->value`. In this case we have to add the key
	 *   to the left child of the node. Now, there can be two cases:
	 *   - `tree->left == NULL`: In this case there is no left child yet.
	 *     We’ll have to create a new node as a left child.
	 *   - `tree->left != NULL`: In this case we already have a left node.
	 *     We’ll have to recurse into it. Use `insert` to add the `key` to the `tree->left` node.
	 */
	 node* trav = tree;

	 if (key < trav->key_value)
	 {
	 	if (trav->left == NULL)
	 	{
	 		trav->left = create_node(key);
	 	}
	 	else
	 	{
	 		insert(key, trav->left);
	 	}
	 }
	 else
	 {
	 	if (trav->right == NULL)
	 	{
	 		trav->right = create_node(key);
	 	}
	 	else
	 	{
	 		insert(key, trav->right);
	 	}
	 }
}

/* create_node: creates a new node for value
 * sets left and right child to NULL
 */
node* create_node(int value)
{
	// allocate memory for node
	node* np		= malloc(sizeof(node));

	// always check if malloc was successful
	if(np == NULL)
	{
		return NULL;
	}

	// set fields
	np->left		= NULL;
	np->right		= NULL;
	np->key_value	= value;

	return np;
}


/******************** Print functions ***************************/
void print_padding(char* path, int path_length, int node_width);
void print_tree_recursive(node* tree, char* path);
void print_branches(char* path, int spacing, int node_width);
void print_node_value(int value, int node_width);

/* print_tree: prints binary tree vertically.
 * Right = up, left = down.
 * NULL-pointer = *
 */
void print_tree(node* tree)
{
	// use a string to register the traversed path.
	static char path[30] = "";
	print_tree_recursive(tree, path);
	printf("\n");
}

/* print_tree_recursive: traverser depth first into right node, then left node */
void print_tree_recursive(node* tree, char* path)
{
	// static int node_value_width = 3;
	static int node_width		= 8; // min 6
	static int vertical_spacing = 1;

	// length of traversed path to get to this node
	int path_length = strlen(path);

	// print null pointers as *
	if(tree == NULL)
	{
		printf("-->*");
		return;
	}

	// print node itself
	print_node_value(tree->key_value, node_width);

	// update path -> right
	path[path_length]	= 'r';
	path[path_length+1] = '\0';

	// print right side of tree
	print_tree_recursive(tree->right, path);

	printf("\n");

	// print branch structure between nodes
	print_branches(path, vertical_spacing, node_width);

	// update path -> left
	path[path_length]	= 'l';
	path[path_length+1] = '\0';

	// print right side of tree
	print_tree_recursive(tree->left, path);

	// update path -> backtrack
	path[path_length]	= '\0';
}

/* print_node_value: prints value: "---------->(val)"
                                    |--node width--|
   if value is too long vor node_width it prints an ellipsis
   instead: "-->(...)""
*/
void print_node_value(int value, int node_width)
{
	// padding before value
	static char padding_string[] = "-------------------";

	// first convert value to string
	char value_string[10];
	int value_width, padding;
	sprintf(value_string, "%d", value);

	// then determine space left for padding
	value_width = strlen(value_string);
	padding = node_width - (value_width + 3);

	// if value is too long
	if(padding < 0)
	{
		strcpy(value_string, "...");
		padding = node_width - 6;
	}

	// print node
	printf("%.*s>(%s)", padding, padding_string, value_string);
}

/* print_branches: prints the branch structure between two nodes. E.g.:
 * From this subtree:
 *
 * ---->(3)---->(1)-->*
 *        |       |
 *        |       `---->(2)
 *
 * It prints this part:
 *
 *        |       |
 *        |       `
 *
 */
void print_branches(char* path, int spacing, int node_width)
{
	static char padding_string[] = "                ";
	int path_length = strlen(path);

	// print spacing lines
	for(int i = 0; i < spacing; i++)
	{
		print_padding(path, path_length, node_width);
    	printf("\n");
	}

	//print line before node
	print_padding(path, path_length-1, node_width);
	printf("%.*s`", node_width-1, padding_string);

}

/* print padding: prints the padding line. Prints a pipe '|', when
 * the node above isn't finished yet (i.e. the path is still in a right branch.)
 */
void print_padding(char* path, int path_length, int node_width)
{
	static char padding_string[] = "                ";
	for(int i = 0; i < path_length; i++)
	{
		if(path[i] == 'l') printf("%.*s", node_width, padding_string);
		else printf("%.*s|", node_width-1, padding_string);
	}
}
