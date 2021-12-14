/*********************************************************************************
 * @file    :   systick.c
 * @brief   :   An abstraction for systick and time functions
 *
 *              This source file provides functions which are used to initialize
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
#include "systick.h"


/* Set global variables which can be used by the time functions */
static ticktime_t startup_time = 0, reset_time = 0;
static bool tsi_poll_flag = false;

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
void systick_init(void)
{
	/* Load value calculation to get a tick of 1ms
	 * Using external clock of 24/16 = 1.5 MHz
	 * Thus VAL register will increment every (1/3) us.
	 * Thus to get 50ms we need the value = (50ms)/(1/3us) = 1500
	 *
	 * */
	//SysTick->LOAD = (150000L);
	SysTick->LOAD = (1500L);
	NVIC_SetPriority(SysTick_IRQn, 3);
	SysTick->VAL = 0;

	/* Enable interrupt
	 * Select external clock (not setting the bit means external clock is selected)
	 * Enable the interrupt
	 * */
	SysTick->CTRL = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

/*********************************************************************************
 * @brief   :   Systick Handler
 *
 *              This function is called whenever the systick interrupt is triggered
 *              It increments the value of variables keeping track of time
 *              It also sets the tsi poll flag
 *
 * @param   :   None
 *
 * @return  :   void
*********************************************************************************/
void SysTick_Handler()
{
	/* Increment the timer values and set the tsi poll flag */
	startup_time++;

	reset_time++;

}

/*********************************************************************************
 * @brief   :   Keeps time since starup
 *
 * @param   :   None
 *
 * @return  :   ticktime_t 	- number of ticks since startup
 * 							  (time in 1/20th of a second)
*********************************************************************************/
ticktime_t now(void)
{
	return startup_time;
}

/*********************************************************************************
 * @brief   :   Resets the reset_time variable
 *
 * @param   :   None
 *
 * @return  :   void
*********************************************************************************/
void reset_timer(void)
{
	reset_time = 0;
}

/*********************************************************************************
 * @brief   :   Returns the reset_time variable
 *
 * @param   :   None
 *
 * @return  :   ticktime_t - value of reset_time variable
*********************************************************************************/
ticktime_t get_timer(void)
{
	return reset_time;
}


