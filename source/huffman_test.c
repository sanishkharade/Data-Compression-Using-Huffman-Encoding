/**
 * @file    :   test_huffman.c
 * @brief   :   An abstraction for testing the huffman algorithm
 *
 *              This source file provides the testing function which is
 *              used for thoroughly testing the huffman algorithm
 *
 * @author  :   Sanish Kharade
 * @date    :   December 10, 2021
 * @version :   1.0
 *
 * @tools   :   MCUXpresso
 *
 * @link    :   --
*/

#include <stdio.h>
#include <assert.h>
#include <inc/huffman.h>
#include <inc/huffman_test.h>
#include <string.h>

/*********************************************************************************
 * @brief   :   Tests the huffman algorithm functionality
 *
 *              This function tests the algorithm functionality by encoding and
 *              decoding multiple strings
 *
 * @param   :   None
 *
 * @return  :   void
*********************************************************************************/
void test_huffman(void)
{

	char str[][150] =
	{
			{"University of Colorado Boulder\n\r"},
			{"My name is Sanish\n\r"
			"I am a graduate student\n\r"},
			{"This is a Principles of Embedded Software Course\n\r"
			"This application is the Final Project for this course"},
			{"testing all lowercase case characters here"},
			{"TESTING ALL CAPITAL LETTERS HERE"},
			{"Special Characters ()+-./][ "},
			{"Numbers 1234567890"}

	};

    uint8_t encoded_buffer[200] = {0};
    uint8_t decoded_string[200] = {0};

	uint32_t encoded_bytes =0;

	static const int num_strings =
	  sizeof(str) / sizeof(str[0]);

	for(int i=0; i< num_strings; i++)
	{
		encoded_bytes = huffman_encode(str[i], encoded_buffer, sizeof(encoded_buffer));
		if(encoded_bytes % 8 != 0)
		{
			encoded_bytes = (encoded_bytes/8) + 1;
		}
		else
		{
			encoded_bytes /= 8;
		}
		huffman_decode(encoded_buffer, strlen(str[i]), decoded_string);

		assert(strncmp(str[i], decoded_string, strlen(str[i])) == 0);

		memset(encoded_buffer, 0, sizeof(encoded_buffer));
		memset(decoded_string, 0, sizeof(decoded_string));
		encoded_bytes = 0;
	}




}
