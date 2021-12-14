/**
 * @file    :   main.c
 * @brief   :   Main entry point to PES Final Project program
 *
 *              This file contains the main code for the program
 *
 * @author  :   Sanish Sanjay Kharade
 * @date    :   December 10, 2021
 * @version :   1.0
 *
 * @tools   :   MCUXpresso IDE
 *
 * @link    :   -
 *
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "cbfifo_test.h"
#include "huffman_test.h"
#include "commands.h"
#include "sysclock.h"
#include "uart.h"
#include "systick.h"


/*********************************************************************************
 * @brief   :   Application entry point.
 *
 * @param   :   None
 *
 * @return  :   0
*********************************************************************************/
int main(void)
{
	/* Initialize the clocks */
	sysclock_init();
	systick_init();

	/* Initialize the UART */
    Init_UART0();

    /* Test the cbfifo if in DEBUG mode */
#ifdef DEBUG
    test_cbfifo();
    test_huffman();
#endif


	while (1)
	{
		process_user_input();
	}

}
