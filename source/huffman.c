/**
 * @file    :   huffman.c
 * @brief   :   An abstraction for huffman encoding and decoding functions
 *
 *              This source file provides abstraction of functions which
 * 				are used to encode and decode the data
 *
 * @author  :   Sanish Sanjay Kharade
 * @date    :   December 10, 2021
 * @version :   1.0
 *
 * @tools   :   MCUXpresso IDE
 *
 * @link    :   Credits to Prof. Howdy Pierce for the huffman_encode function
 *
*/


#include <stdio.h>
#include "huffman.h"
#include "lookup_table.h"
#include <assert.h>
#include <string.h>
#include <stdint.h>

/* Macro to calculate minimum of two numbers*/
#define min(x,y) ((x<=y)?x:y)

/*********************************************************************************
 * @brief   :  	Decodes the encoded buffer and prints the string
 *
 * @param   :   encoded_buffer	- encoded buffer
 * 				encoded_bits	- number of original bytes
 * 				decoded_buffer	- buffer to be filled with the decoded string
 *
 * @return  : 	void
**********************************************************************************/
void huffman_decode(uint8_t encoded_buffer[], uint16_t encoded_bits, uint8_t decoded_buffer[])
{
	uint8_t *buf = encoded_buffer;

	uint16_t ebuf_id = 0, dbuf_id = 0;
	uint16_t code1 = 0, code_length = 1;
	uint16_t i = 1, bit_pos=0;
	//bool char_found = false;

	while(1)
	{
		code1 |= (buf[ebuf_id] & 0x80) >> 7;

		while(huffman_codes[i].character != '~')
		{
			if(code_length == huffman_codes[i].code_bits)//&& not zero
			{
				if(code1 == huffman_codes[i].code)
				{

					decoded_buffer[dbuf_id++] = huffman_codes[i].character;
					code1 = 0;
					code_length = 0;
					//char_found = true;
					break;
				}
			}
			i++;
		}
		// if(char_found == false)
		// {
		//   decoded_buffer[dbuf_id++] = huffman_codes[i].character;
		// }
		// else
		// {
		//   char_found = false;
		// }
		i=1;
		code_length++;
		code1 <<= 1;
		buf[ebuf_id] <<= 1;
		bit_pos++;
		if(bit_pos == 8)
		{
			bit_pos = 0;
			ebuf_id++;
		}
		if(dbuf_id == encoded_bits)
			break;

	}

	decoded_buffer[dbuf_id]= '\0';
	printf("%s\n\n", decoded_buffer);
}

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
int huffman_encode(const char *message, uint8_t *buffer, size_t nbytes)
{
    /* Current write position into encoded data */
	int buf_idx = 0;

    /*  Number of bits already written at buf_idx */
	int bits_written=0;

    /*  Huffman code for the current symbol */
	int hc_idx;

	memset(buffer, 0, nbytes);

	for (const char *p = message; *p != '\0'; p++)
	{
		/* Find the huffman code for this symbol */
		for (hc_idx= 0; huffman_codes[hc_idx].character != *p; hc_idx++);
			//assert (huff_code[hc_idx].symbol - HUFF_CODE_END_SYMBOL);

        //		if(huffman_codes[hc_idx].code_bits == 0)
        //		{
        //			hc_idx == 126;
        //		}

        /* Get the code and number of code bits */
		uint32_t code = huffman_codes[hc_idx].code;
		int code_bits = huffman_codes[hc_idx].code_bits;

		while (code_bits > 0)
		{

			/* this write: the number of bits to grab in this go-round */
			int this_write = min(8 - bits_written, code_bits);

			assert (bits_written + this_write <= 8);

			/* read shift: position within code to get the bits from */
            int read_shift = code_bits - this_write;

			/* tmp: bits to write, shifted down to lsb */
			uint32_t tmp = (code >> read_shift) & ((1UL<<this_write)-1);

			/* write_shift: shift to fit into this buffer */
			int write_shift = 8 - bits_written - this_write;

            /* buffer[buf_idx]: buffer we're writing into */
			buffer[buf_idx] |= tmp << write_shift;

			bits_written += this_write;
			code_bits -= this_write;
			if (bits_written == 8)
			{
				bits_written = 0;
				buf_idx++;
				assert(buf_idx < nbytes);
			}
		}
	}

//	if(bits_written != 8)
//	{
//		buf_idx++;
//	}

	//change
	//printf("bits written = %d\n", 8*buf_idx + bits_written);
	//return buf_idx;
	return 8*buf_idx + bits_written;
}
