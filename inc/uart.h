/**
 * @file    :   uart.h
 * @brief   :   An abstraction for UART functions
 *
 *              This header file provides functions which are used to write and
 *              and read data to the serial terminal
 *
 * @author  :   Sanish Kharade
 * @date    :   December 10, 2021
 * @version :   1.0
 *
 * @tools   :   MCUXpresso IDE
 *
 * @link    :   Alexander Dean Book -Chapter 8
 * 				https://github.com/alexander-g-dean/ESF/tree/master/NXP/Code/Chapter_8/Serial-Demo/src
*/

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>
#include <MKL25Z4.H>

/*********************************************************************************
 * @brief   :   Resets the data and time stats to zero
 *
 * @param   :   none
 *
 * @return  :   void
*********************************************************************************/
void reset_stats(void);

/*********************************************************************************
 * @brief   :   Prints the data and time stats to zero
 *
 * @param   :   none
 *
 * @return  :   void
*********************************************************************************/
void print_stats(void);

/*********************************************************************************
 * @brief   :   Sets the custom string member of the stats structure to true
 *
 * @param   :   none
 *
 * @return  :   void
*********************************************************************************/
void set_custom_string_true(void);

/*********************************************************************************
 * @brief   :   Initializes the UART
 *
 * @param   :   baud_rate - baud rate for the UART
 *
 * @return  :   void
*********************************************************************************/
void Init_UART0(void);


#endif
