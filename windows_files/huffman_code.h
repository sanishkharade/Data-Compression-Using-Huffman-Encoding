/**
 * @file    :   huffman_code.h
 * @brief   :   An abstraction for huffman encoding and decoding functions
 *
 *              This source file provides abstraction of functions which 
 * 				are used to encode and decode the data
 *
 * @author  :   Sanish Sanjay Kharade
 * @date    :   December 10, 2021
 * @version :   1.0
 * 
 * @tools   :   gcc, cygwin, Visual Studio Code
 * 
 * @link    :   Credits to Prof. Howdy Pierce for the huffman_encode function
 * 				
*/

/*********************************************************************************
 * @brief   :  	Decodes the encoded buffer and prints the string
 *
 * @param   :   encoded_buffer	- encoded buffer
 * 				encoded_bits	- number of original bytes
 * 				decoded_buffer	- buffer to be filled with the decoded string
 *
 * @return  : 	void
**********************************************************************************/
void huffman_decode(uint8_t encoded_buffer[], uint16_t encoded_bits, uint8_t decoded_buffer[]);

/*********************************************************************************
 * @brief   :  	Encodes the message using the huffman lookup table for
 * 				data compression
 * 	
 * 				Credits to Prof. Howdy Pierce for this function
 *
 * @param   :   message	- message to be encoded
 * 				buffer	- buffer to fill with encoded message
 * 				nbytes	- size of the buffer
 *
 * @return  : 	void
**********************************************************************************/
int huffman_encode(const char *message, uint8_t *buffer, size_t nbytes);