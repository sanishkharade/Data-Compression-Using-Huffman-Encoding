/*********************************************************************************
 * @file    :   systick.h
 * @brief   :   An abstraction for systick and time functions
 *
 *              This header file provides functions which are used to initialize
 *              the Systick timer and getting the time at different instances in
 *              the program.
 *
 * @author  :   Sanish Kharade
 * @date    :   December 10, 2021
 * @version :   1.0
 *
 * @tools   :   MCUXpresso IDE
 *
 * @link    :   ARM Reference Manual
*********************************************************************************/
#ifndef SYSTICK_H_
#define SYSTICK_H_
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "MKL25Z4.h"

typedef uint32_t ticktime_t;

/*********************************************************************************
 * @brief   :   Initializes the Systick Timer
 *
 *              This function initializes the systick timer by loading
 *              appropriate values into its configuration registers
 *
 * @param   :   None
 *
 * @return  :   void
*********************************************************************************/
void systick_init(void);

/*********************************************************************************
 * @brief   :   Keeps time since starup
 *
 * @param   :   None
 *
 * @return  :   ticktime_t 	- number of ticks since startup
 * 							  (time in 1/20th of a second)
*********************************************************************************/
ticktime_t now(void);

/*********************************************************************************
 * @brief   :   Resets the reset_time variable
 *
 * @param   :   None
 *
 * @return  :   void
*********************************************************************************/
void reset_timer(void);

/*********************************************************************************
 * @brief   :   Returns the reset_time variable
 *
 * @param   :   None
 *
 * @return  :   ticktime_t - value of reset_time variable
*********************************************************************************/
ticktime_t get_timer(void);


#endif /* SYSTICK_H_ */
