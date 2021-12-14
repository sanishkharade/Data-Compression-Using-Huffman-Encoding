/**
 * @file    :   huffman_code.c
 * @brief   :   An abstraction for huffman encoding and decoding functions
 *
 *              This source file provides functions which are used to
 * 				encode and decode the data
 *
 * @author  :   Sanish Sanjay Kharade
 * @date    :   December 10, 2021
 * @version :   1.0
 * 
 * @tools   :   gcc, cygwin, Visual Studio Code
 * 
 * @link    :   Windows serial programming
 * 				https://www.pololu.com/docs/0J73/15.6
 * 				https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
 * 				
*/
#include<windows.h>
#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<string.h>
#include <time.h>
#include<assert.h>

#include "huffman_code.h"

#define baud_rate 9600

/*********************************************************************************
 * @brief   :  	Sets the baud rate of the opened COM port
 *
 * @param   :   hComm - handle of the COM port
 *
 * @return  : 	void
**********************************************************************************/
void set_baud_rate(HANDLE hComm)
{
	DCB state = {0};
	state.DCBlength = sizeof(DCB);
	state.BaudRate = baud_rate;
	state.ByteSize = 8;
	state.Parity = NOPARITY;
	state.StopBits = ONESTOPBIT;
	bool success = SetCommState(hComm, &state);
}

/*********************************************************************************
 * @brief   :  	To generate a delay in milliseconds
 * 
 * @param   :   hComm - handle of the COM port
 *
 * @return  : 	void
**********************************************************************************/
void delay_ms(int milliseconds)
{
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + milliseconds);
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
	HANDLE hComm;
	BOOL Status;
	DWORD dwEventMask;

	char str[200] = {0}; 
	printf("Welcome to PES Final Project - Data Compression using Huffman Encoding\n");

	hComm = CreateFileA("\\\\.\\COM6",                //port name
						GENERIC_READ | GENERIC_WRITE, //Read/Write
						0,                            // No Sharing
						NULL,                         // No Security
						OPEN_EXISTING,// Open existing port only
						0,            // Non Overlapped I/O
						NULL);        // Null for Comm Devices

	if (hComm == INVALID_HANDLE_VALUE)
	{
		printf("Error in opening serial port\n\n");
		goto exit;
	}
	else
		printf("Opening serial port successful!\n\n");

	set_baud_rate(hComm);

	DWORD data, datawritten;

	uint8_t TempChar;

	uint8_t rcvd_buffer[200]={0};
	uint8_t rcvd_buf_idx = 0;
	uint8_t decoded_buffer[200]={0};
	int enc = 0;
	int original_size = 0;
	int reduced_size = 0;
	float percent_reduction = 0;

	while(1)
	{
		/* Set all variables and buffers to 0 */
		rcvd_buf_idx = 0;
		enc = 0;
		original_size = 0;
		reduced_size = 0;

		memset(str, 0, sizeof(str));
		memset(decoded_buffer, 0, sizeof(decoded_buffer));
		memset(rcvd_buffer, 0, sizeof(rcvd_buffer));

		printf("Enter command or string to encode\n");
		gets(str);

		/*	
		 *	gets doesnt store the \r in the string when we press enter
		 *	hence we add a \r so that it can be understood by the 
		 *	command processor on the KL25Z
		*/
		str[strlen(str)] = '\r';

		/* Send the string one character at a time */
		for(int i = 0; i<strlen(str); i++) 
		{
			Status = WriteFile(hComm, &(str[i]), 1, &datawritten, NULL);
			delay_ms(10);
		}

		/*	
		 * 	KL25Z first sends the original length of string
		 *	Then it sends the total encoded bits
		 *	Then it sends the reduced length of the encoded string
		 *	Read all these one at a time
		*/

		/* Read the original size of the string */
		delay_ms(10);
		Status = ReadFile(hComm, &TempChar, sizeof(TempChar), &data, NULL);
		original_size = TempChar;

		/* Read the number of encoded bits */
		delay_ms(10);
		Status = ReadFile(hComm, &TempChar, sizeof(TempChar), &data, NULL);
		enc = TempChar;

		/* Read the reduced size of the string */
		delay_ms(10);
		Status = ReadFile(hComm, &TempChar, sizeof(TempChar), &data, NULL);
		reduced_size = TempChar;

		/* Read the encoded buffer */
		for(int j=0;j<reduced_size;j++)
		{
			Status = ReadFile(hComm, &TempChar, sizeof(TempChar), &data, NULL);
			rcvd_buffer[rcvd_buf_idx++] = TempChar;
		}

		/* Uncomment the below lines to print the received sizes */
		// printf("\nOriginal size of string = %d bytes\n", original_size);
		// printf("Encrypted bits = %d\n", enc);
		// printf("Reduced size of string = %d bytes\n", reduced_size);

		//  Calculate the percent reduction and print it 
		// percent_reduction = (float)((original_size - reduced_size) * 100) / original_size;
		// printf("Percentage reduced_size is %f\n", percent_reduction);

		/* Uncomment below for loop for printing the encoded bytes for debugging */
		// for(int p = 0; p< rcvd_buf_idx; p++)
		// {
		// 	printf("buffer[%d] = %02X\n", p, rcvd_buffer[p]);
		// }

		/* Decode the data and print it */
		printf("Decoded String - \n");
		huffman_decode(rcvd_buffer, original_size, decoded_buffer);

		str[strlen(str) -1] = '\0';
		if((strncmp(str, "author", 6) && strncmp(str, "help", 4) && strncmp(str, "stats", 5) && strncmp(str, "reset", 5) ))
		{
			assert(strncmp(str, decoded_buffer, original_size) == 0);
			printf("Strings matched\n\n");
		}	
			
	}

	/* In case there is an error in opening the serial port */
	exit : 
		printf("Closing serial port\n");
		CloseHandle(hComm);//Closing the Serial Port

		return 0;
}

//gcc serial_port.c huffman_code.c -o serial.exe