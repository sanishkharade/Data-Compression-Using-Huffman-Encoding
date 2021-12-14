/**
 * @file    :   cbfifo.h
 * @brief   :   An abstraction for cbfifo functions
 *
 *              This header file provides functions which are used to write and
 *              and read data into a cbfifo
 *
 * @author  :   Sanish Kharade & Howdy Pierce
 * @date    :   December 10, 2021
 * @version :   1.0
 *
 * @tools   :   MCUXpresso IDE
 *
 * @link    :   -
*/
#ifndef CBFIFO_H_
#define CBFIFO_H_

#include <stdlib.h>  // for size_t
#include <stdint.h>

/* Enum to store handle of different cbfifos
 * Created for encapsulation
 * last element of enum will always contain the total number of cbfifos
 * */
typedef enum
{
	kTx_FIFO,
	kRx_FIFO,
	kNUM_FIFOS
}cbfifo_handle_t;

enum cbfifo_state_s
{
    EMPTY = 0,
    PARTIALLY_FILLED = 1,
    FULL = 2
};


/* Enum to store state of cbfifo */
typedef enum cbfifo_state_s cbfifo_state_t;

/* Declaration of cbfifo type of cbfifo */
typedef struct cbfifo_s cbfifo_t;


/*********************************************************************************
 * @brief   :   Enqueues data onto the FIFO
 *
 *              This function enqueues enqueues data onto the FIFO, up to
 *              the limit of the available FIFO capacity
 *
 * @param   :   cbf_handle 	handle of cbfifo on which enqueue needs to be performed
 * 				buf     - Pointer to the data
 *              nbyte   - Max number of bytes to enqueue
 *
 * @return  :   size_t  - The number of bytes actually enqueued, which could be 0.
 *              -1      - in case of an error
 *
*********************************************************************************/
size_t cbfifo_enqueue(cbfifo_handle_t cbf_handle, void *buf, size_t nbyte);

/*********************************************************************************
 * @brief   :   Dequeues data from the FIFO
 *
 *              This function attempts to remove up to nbytes bytes of data from the
 *              FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * @param   :   cbf_handle 	handle of cbfifo on which dequeue needs to be performed
 * 				buf     - Destination for the dequeued data
 *              nbyte   - Bytes of data requested
 *
 * @return  :   size_t  - The number of bytes actually dequeued, which will be
 *                        between 0 and nbyte
 *              -1      - in case of an error
 *
*********************************************************************************/
size_t cbfifo_dequeue(cbfifo_handle_t cbf_handle, void *buf, size_t nbyte);

/*********************************************************************************
 * @brief   :   Returns the number of bytes currently on the cbfifo.
 *
 * @param   :   cbf_handle - handle of cbfifo
 *
 * @return  :   size_t  - number of bytes currently available to be dequeued
 *                        from the FIFO
 *
*********************************************************************************/
size_t cbfifo_length(cbfifo_handle_t cbf_handle);


/*********************************************************************************
 * @brief   :   Returns the capacity of the cbfifo.
 *
 * @param   :   cbf_handle - handle of cbfifo
 *
 * @return  :   size_t  - capacity of the cbfifo in bytes.
 *
*********************************************************************************/
size_t cbfifo_capacity(cbfifo_handle_t cbf_handle);

#endif // _CBFIFO_H_
