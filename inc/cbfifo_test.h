/**
 * @file    :   test_cbfifo.h
 * @brief   :   An abstraction for testing the cbfifo
 *
 *              This header file provides an abstraction of testing function
 *              which is used for thoroughly testing the cbfifo.
 *
 * @author  :   Sanish Kharade & Howdy Pierce
 * @date    :   December 10, 2021
 * @version :   1.0
 *
 * @tools   :   MCUXpresso
 *
 * @link    :   --
*/

#ifndef CBFIFO_TEST_H_
#define CBFIFO_TEST_H_

/*********************************************************************************
 * @brief   :   Tests the cbfifo functionality for multiple instances
 *
 *              This function tests the cbfifo functionality thoroughly by
 *              by using the enqueue and dequeue functions is different order.
 *
 * @param   :   None
 *
 * @return  :   void
*********************************************************************************/
void test_cbfifo(void);


#endif /* CBFIFO_TEST_H_ */
