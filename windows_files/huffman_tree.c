/**
 * @file    :   huffman_tree.c
 * @brief   :   An abstraction for huffman tree generation functions
 *
 *              This source file provides functions which are used to
 * 				generate the Huffman Tree
 *              It also prints out the huffman tree to store in a header file
 *
 * @author  :   Sanish Sanjay Kharade
 * @date    :   December 10, 2021
 * @version :   1.0
 * 
 * @tools   :   gcc, cygwin, Visual Studio Code
 * 
 * @link    :   Huffman Tree Generation Functions have been leveraged from the following link
 * 				https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
 * 				The link has functions to generate the Huffman tree from two input arrays of
 * 				the characters and frequencies. It does not generate these arrays.
 * 
 * 				The code for generating the required arrays from a file input 
 * 				has been written by me. Functions from the above link have been modified 
 * 				to create the lookup table and store it in a header file
 * 				
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include <assert.h>

#include "huffman_tree.h"

/*
 * Structure for the huffman table
*/
typedef struct
{
	unsigned char character;
	uint32_t code;
	int code_bits;
	int frequency;
} huffman_code_t;

huffman_code_t table[26];

/*********************************************************************************
 * @brief   :   Creates a mew node of the huffman tree
 *
 * @param   :   data - character in this node	
 * 				freq - frequency of the node
 *
 * @return  : 	HuffmanTreeNode	- pointer to the new node
**********************************************************************************/
struct HuffmanTreeNode* new_node(char data, unsigned freq)
{
	struct HuffmanTreeNode* temp = (struct HuffmanTreeNode*)malloc(sizeof(struct HuffmanTreeNode));

	temp->left = temp->right = NULL;
	temp->data = data;
	temp->freq = freq;

	return temp;
}

/*********************************************************************************
 * @brief   :   Creates a the huffman tree
 *
 * @param   :   capacity - capacity of the huffman tree
 *
 * @return  : 	HuffmanTree	- pointer to the huffman tree
**********************************************************************************/
struct HuffmanTree* create_huffman_tree(unsigned capacity)
{
	struct HuffmanTree* minHeap = (struct HuffmanTree*)malloc(sizeof(struct HuffmanTree));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = (struct HuffmanTreeNode**)malloc(minHeap->capacity * sizeof(struct HuffmanTreeNode*));
	return minHeap;
}

/*********************************************************************************
 * @brief   :   swaps two nodes in the huffman tree
 *
 * @param   :   a - node a
 * 				b - node b
 *
 * @return  : 	void
**********************************************************************************/
void swap_node(struct HuffmanTreeNode** a, struct HuffmanTreeNode** b)
{
	/* Swap the nodes using a temporary node */
	struct HuffmanTreeNode* t = *a;
	*a = *b;
	*b = t;
}

/*********************************************************************************
 * @brief   :   Iterates through the tree to place new node
 *
 * @param   :   minHeap - pointer to the huffman tree
 * 				idx 	- index
 *
 * @return  : 	void
**********************************************************************************/
void minHeapify(struct HuffmanTree* minHeap, int idx)
{
	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
		smallest = left;

	if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
		smallest = right;

	if (smallest != idx) {
		swap_node(&minHeap->array[smallest], &minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

/*********************************************************************************
 * @brief   :   Checks if the huffman tgree consists only one node or not
 *
 * @param   :   minHeap - pointer to the huffman tree
 *
 * @return  : 	int - 1 - if size is 1
 * 					- 0 - if size is not 1	
**********************************************************************************/
int is_size_one(struct HuffmanTree* minHeap)
{
	return (minHeap->size == 1);
}

/*********************************************************************************
 * @brief   :   Extracts a node from the tree
 *
 * @param   :   minHeap - pointer to the huffman tree
 *
 * @return  : 	HuffmanTreeNode	- pointer to the extracted node
**********************************************************************************/
struct HuffmanTreeNode* extract_node(struct HuffmanTree* minHeap)
{
	struct HuffmanTreeNode* temp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];

	--minHeap->size;
	minHeapify(minHeap, 0);

	return temp;
}

/*********************************************************************************
 * @brief   :   Inserts a node into the huffman tree
 *
 * @param   :   minHeap 	- pointer to the huffman tree
 * 				minHeapNode	- pointer to the huffman tree node
 *
 * @return  : 	void
**********************************************************************************/
void insert_huffman_tree(struct HuffmanTree* minHeap, struct HuffmanTreeNode* minHeapNode)
{
	++minHeap->size;
	int i = minHeap->size - 1;

	while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) / 2;
	}

	minHeap->array[i] = minHeapNode;
}

/*********************************************************************************
 * @brief   :   Builds the huffman tree
 *
 * @param   :   minHeap - pointer to the huffman tree
 *
 * @return  : 	void
**********************************************************************************/
void build_huffman_tree(struct HuffmanTree* minHeap)
{
	int n = minHeap->size - 1;
	int i;

	for (i = (n - 1) / 2; i >= 0; --i)
		minHeapify(minHeap, i);
}

/*********************************************************************************
 * @brief   :   Prints an array
 *
 * @param   :   arr	- array pointer
 * 				n	- number of elements in the array
 *
 * @return  : 	void
**********************************************************************************/
void print_array(int arr[], int n)
{
	int i;
	//codes[count++].code = str()
	for (i = 0; i < n; ++i)
		printf("%d", arr[i]);

	printf("\n");
}

/*********************************************************************************
 * @brief   :   Checks if the gievn node is a leaf node
 *
 * @param   :   root - pointer to the huffman tree node
 *
 * @return  : 	int - 1 - if the node is a leaf
 * 					- 0 - if the node is not a leaf
**********************************************************************************/
int is_leaf(struct HuffmanTreeNode* root)
{
	return !(root->left) && !(root->right);
}

/*********************************************************************************
 * @brief   :   Creates and builds the huffman tree
 * 				Parent function to the create_huffman_tree 
 * 				and create_huffman_tree functions
 *
 * @param   :   data 	- array of characters
 * 				freq	- array of the frequency of characters	
 * 				size	- number of elements	
 *
 * @return  : 	HuffmanTree - pointer to the Huffman tree
**********************************************************************************/
struct HuffmanTree* create_build_huffman_tree(char data[], int freq[], int size)
{
	struct HuffmanTree* minHeap = create_huffman_tree(size);

	for (int i = 0; i < size; ++i)
		minHeap->array[i] = new_node(data[i], freq[i]);

	minHeap->size = size;
	build_huffman_tree(minHeap);

	return minHeap;
}

/*********************************************************************************
 * @brief   :   Makes the huffman tree
 * 				Parent function to the create_build_huffman_tree function
 *
 * @param   :   data 	- array of characters
 * 				freq	- array of the frequency of characters	
 * 				size	- number of elements	
 *
 * @return  : 	HuffmanTreeNode - root of the Huffman tree
**********************************************************************************/
struct HuffmanTreeNode* make_huffman_tree(char data[], int freq[], int size)
{
	struct HuffmanTreeNode *left, *right, *top;

	struct HuffmanTree* minHeap = create_build_huffman_tree(data, freq, size);

	/* Iterate while size of heap doesn't become 1 */
	while (!is_size_one(minHeap)) 
	{
		/* Extract the two minimum frequency items */
		left = extract_node(minHeap);
		right = extract_node(minHeap);

		/* 	Create a new node with sum of frequencies
		 * 	Make the two nodes as children of the new node
		 *	Add this node to the tree
		 */
		top = new_node('$', left->freq + right->freq);

		top->left = left;
		top->right = right;

		insert_huffman_tree(minHeap, top);
	}

	/* The remaining node is the root node */
	return extract_node(minHeap);
}

/*********************************************************************************
 * @brief   :   Traverses through the entire huffman tree
 *
 * @param   :   root 	- root of the huffman tree
 * 				arr		- array 
 * 				top		- counter
 *
 * @return  : 	HuffmanTreeNode - root of the Huffman tree
**********************************************************************************/
void traverse_huffman_tree(struct HuffmanTreeNode* root, int arr[], int top)
{
	/* Assign 0 to left node and recur */
	if (root->left) {
		arr[top] = 0;
		traverse_huffman_tree(root->left, arr, top + 1);
	}

	/* Assign 1 to right node and recur */
	if (root->right) {
		arr[top] = 1;
		traverse_huffman_tree(root->right, arr, top + 1);
	}

	/* Leaf node detected */
	if (is_leaf(root))
	{
		table[root->data].code_bits = top;
		for (int i = 0; i < top; i++)
		{
			if (arr[i] == 1)
			{
				table[root->data].code += 1*(arr[i]<<(top-1-i));
			}		
		}
	}
}

/*********************************************************************************
 * @brief   :  	Creates the lookup table with characters and their frequencies
 * 				in the string
 *
 * @param   :   none
 *
 * @return  : 	int - number of characters with frequency greater than 0 
**********************************************************************************/
int create_lookup_table(void)
{
	int count = 0;
	for (int i = 0; i<NUMBER_OF_CHARACTERS; i++)
	{
		table[i].character = i;
		/* If the character has a frequency increment count */
		if (table[i].frequency > 0)
		{
			count++;
		}
	}
	return count;
}

/*********************************************************************************
 * @brief   :  	Generates the header file for KL25Z which consists of the 
 * 				lookup table
 *
 * @param   :   none
 *
 * @return  : 	void
**********************************************************************************/
void generate_header_file(void)
{
	printf("#ifndef LOOKUP_TABLE_H_\n");
	printf("#define LOOKUP_TABLE_H_\n\n");
	printf("#include<stdint.h>\n\n");
	printf("typedef struct {\nunsigned char character;\nuint32_t code;\nint code_bits;\n} huffman_code_t;\n\n");

	printf("huffman_code_t huffman_codes[] = {\n");
	for (int i = 0; i < NUMBER_OF_CHARACTERS; i++)
	{
		if (i != NUMBER_OF_CHARACTERS - 1)
			printf("{%d, 0x%02x, %d},\n", table[i].character, table[i].code, table[i].code_bits);
		else
			printf("{%d, 0x%02x, %d} };\n\n", table[i].character, table[i].code, table[i].code_bits);
	}

	printf("#endif\n");
}


/*********************************************************************************
 * @brief   :  	Generates the huffman tree
 * 				Parent of all functions
 * 				Tree generation begins here
 *
 * @param   :   none
 *
 * @return  : 	void
**********************************************************************************/
void generate_huffman_tree(void)
{
	/* Open the file to take the input from */
	FILE *fileptr;
	char ch;
	fileptr = fopen("hello.txt", "r");
	/* Increment the frequency for every character */
	while((ch = fgetc(fileptr)) != EOF)
	{
		table[ch].frequency++;
	}
	fclose(fileptr);
	fileptr = NULL;

	/* Find number fof characters with frequency greater than 0 */
	int symbols = create_lookup_table();

	char non_zero_chars[symbols];
	int non_zero_freqs[symbols];

	int index = 0;

	/* Fill the non zero frequency elements into two arrays */
	for (int i = 0; i < NUMBER_OF_CHARACTERS; i++)
	{
		if (table[i].frequency > 0)
		{
			non_zero_chars[index] = table[i].character;
			non_zero_freqs[index++] = table[i].frequency;
		}
	}

	uint8_t buffer[1024];

	/* Construct the Huffman Tree */
	struct HuffmanTreeNode* root = make_huffman_tree(non_zero_chars, non_zero_freqs, symbols);

	int arr[MAX_TREE_HT], top = 0;

	/* Traverse the Huffman Tree */
	traverse_huffman_tree(root, arr, top);
}

/*********************************************************************************
 * @brief   :  	Main entry point to the application
 * 
 * @param   :   none
 *
 * @return  : 	int
**********************************************************************************/
int main()
{
	generate_huffman_tree();
	generate_header_file();

	return 0;
}
