/**
 * @file    :   huffman_tree.h
 * @brief   :   An abstraction for huffman tree generation functions
 *
 *              This header file provides an abstraction for functions which 
 * 				are used to generate the Huffman Tree
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
#include <stdint.h>

#define MAX_TREE_HT 100

#define NUMBER_OF_CHARACTERS 128
/*
 * 	HuffmanTreeNode is a node of the huffman tree
 * 	data	- one of the input characters
 * 	freq	- frequency of the character
 *	left	- left child of this node
 *	right	- right child of this node
*/
struct HuffmanTreeNode
{
	char data;
	unsigned freq; 
	struct HuffmanTreeNode *left, *right; 
};

/*
 * 	HuffmanTree is a collection of all the Huffman Tree nodes
 * 	size		- current size of the heap
 * 	capacity	- capacity of the min heap
 *	array		- array of HuffmanTreeNode node pointers
*/
struct HuffmanTree
{
	unsigned size;
	unsigned capacity; 
	struct HuffmanTreeNode** array; 
};

/*********************************************************************************
 * @brief   :   Creates a mew node of the huffman tree
 *
 * @param   :   data - character in this node	
 * 				freq - frequency of the node
 *
 * @return  : 	HuffmanTreeNode	- pointer to the new node
**********************************************************************************/
struct HuffmanTreeNode* new_node(char data, unsigned freq);

/*********************************************************************************
 * @brief   :   Creates a the huffman tree
 *
 * @param   :   capacity - capacity of the huffman tree
 *
 * @return  : 	HuffmanTree	- pointer to the huffman tree
**********************************************************************************/
struct HuffmanTree* create_huffman_tree(unsigned capacity);

/*********************************************************************************
 * @brief   :   swaps two nodes in the huffman tree
 *
 * @param   :   a - node a
 * 				b - node b
 *
 * @return  : 	void
**********************************************************************************/
void swap_node(struct HuffmanTreeNode** a, struct HuffmanTreeNode** b);

/*********************************************************************************
 * @brief   :   Iterates through the tree to place new node
 *
 * @param   :   minHeap - pointer to the huffman tree
 * 				idx 	- index
 *
 * @return  : 	void
**********************************************************************************/
void minHeapify(struct HuffmanTree* minHeap, int idx);

/*********************************************************************************
 * @brief   :   Checks if the huffman tgree consists only one node or not
 *
 * @param   :   minHeap - pointer to the huffman tree
 *
 * @return  : 	int - 1 - if size is 1
 * 					- 0 - if size is not 1	
**********************************************************************************/
int is_size_one(struct HuffmanTree* minHeap);

/*********************************************************************************
 * @brief   :   Extracts a node from the tree
 *
 * @param   :   minHeap - pointer to the huffman tree
 *
 * @return  : 	HuffmanTreeNode	- pointer to the extracted node
**********************************************************************************/
struct HuffmanTreeNode* extract_node(struct HuffmanTree* minHeap);

/*********************************************************************************
 * @brief   :   Inserts a node into the huffman tree
 *
 * @param   :   minHeap 	- pointer to the huffman tree
 * 				minHeapNode	- pointer to the huffman tree node
 *
 * @return  : 	void
**********************************************************************************/
void insert_huffman_tree(struct HuffmanTree* minHeap, struct HuffmanTreeNode* minHeapNode);

/*********************************************************************************
 * @brief   :   Builds the huffman tree
 *
 * @param   :   minHeap - pointer to the huffman tree
 *
 * @return  : 	void
**********************************************************************************/
void build_huffman_tree(struct HuffmanTree* minHeap);

/*********************************************************************************
 * @brief   :   Prints an array
 *
 * @param   :   arr	- array pointer
 * 				n	- number of elements in the array
 *
 * @return  : 	void
**********************************************************************************/
void print_array(int arr[], int n);

/*********************************************************************************
 * @brief   :   Checks if the gievn node is a leaf node
 *
 * @param   :   root - pointer to the huffman tree node
 *
 * @return  : 	int - 1- if the node is a leaf
 * 					- 0 - if the node is not a leaf
**********************************************************************************/
int is_leaf(struct HuffmanTreeNode* root);

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
struct HuffmanTree* create_build_huffman_tree(char data[], int freq[], int size);

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
struct HuffmanTreeNode* make_huffman_tree(char data[], int freq[], int size);

/*********************************************************************************
 * @brief   :   Traverses through the entire huffman tree
 *
 * @param   :   root 	- root of the huffman tree
 * 				arr		- array 
 * 				top		- counter
 *
 * @return  : 	HuffmanTreeNode - root of the Huffman tree
**********************************************************************************/
void traverse_huffman_tree(struct HuffmanTreeNode* root, int arr[], int top);

/*********************************************************************************
 * @brief   :  	Creates the lookup table with characters and their frequencies
 * 				in the string
 *
 * @param   :   none
 *
 * @return  : 	void
**********************************************************************************/
int create_lookup_table(void);

/*********************************************************************************
 * @brief   :  	Generates the header file for KL25Z which consists of the 
 * 				lookup table
 *
 * @param   :   none
 *
 * @return  : 	void
**********************************************************************************/
void generate_header_file(void);

/*********************************************************************************
 * @brief   :  	Generates the huffman tree
 * 				Parent of all functions
 * 				Tree generation begins here
 *
 * @param   :   none
 *
 * @return  : 	void
**********************************************************************************/
void generate_huffman_tree(void);


