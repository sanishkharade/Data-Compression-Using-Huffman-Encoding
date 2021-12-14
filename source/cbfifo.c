/**
 * @file    :   cbfifo.c
 * @brief   :   An abstraction for cbfifo functions
 *
 *              This source file provides functions which are used to write and
 *              and read data into a cbfifo
 *              It also contains the definition of the cbfifo and enum cb_error.
 *
 * @author  :   Sanish Kharade
 * @date    :   December 10, 2021
 * @version :   1.0
 *
 * @tools   :   MCUXpresso IDE
 *
 * @link    :   -
*/

#include <inc/cbfifo.h>
#include <stdio.h>
#include <stdint.h>
#include <MKL25Z4.h>

/* Capacity of CBFIFO */
#define CBFIFO_CAPACITY 256

/* Definition of cbfifo structure */
struct cbfifo_s
{
    uint8_t cbfifo_array[CBFIFO_CAPACITY];
    size_t length;

    cbfifo_state_t state;

    size_t write;
    size_t read;

};

/* An array of cbfifos declared statically*/
static cbfifo_t gfifos[kNUM_FIFOS];

/* Enum for storing error code
 * Unused right now but can be used to display error messages
 * */
typedef enum {
    NO_ERROR = 0,
    INVALID_BUF = 1,
    FULL_FIFO = 2,
    EMPTY_FIFO = 3
}cb_error_t;

/* Create a variable of type error */
static cb_error_t cb_error = NO_ERROR;


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
********************************************************************************/
size_t cbfifo_enqueue(cbfifo_handle_t cbf_handle, void *buf, size_t nbyte)
{
	/* Disable the interrupts */
	uint32_t masking_state;
	masking_state = __get_PRIMASK();
	__disable_irq();

    cb_error = NO_ERROR;

    /* Check if pointer is NULL */
    if(buf == NULL)
    {
        cb_error = INVALID_BUF;
        __set_PRIMASK(masking_state);
        return -1;
    }

    /* Return 0 if nothing needs to be enqueued */
    if(nbyte == 0)
    {
    	__set_PRIMASK(masking_state);
        return 0;
    }

    /*If nbytes is large then set it to the max possible number that can be enqueued*/
    if(nbyte > cbfifo_capacity(cbf_handle) - gfifos[cbf_handle].length)
    {
        nbyte = cbfifo_capacity(cbf_handle) - gfifos[cbf_handle].length;
    }

    size_t i = 0;

    /* If cbfifo is full */
    if(gfifos[cbf_handle].state == FULL)
    {
        cb_error = FULL_FIFO;
        __set_PRIMASK(masking_state);
        return 0;
    }
    /* If cbfifo is not full */
    else
    {
        /* Update state of cbfifo */
    	gfifos[cbf_handle].state = PARTIALLY_FILLED;
        while( (gfifos[cbf_handle].state != FULL) && (nbyte > 0) )
        {
            /* Copy elements from the buffer to the cbfifo and update write location */
            ((uint8_t*)gfifos[cbf_handle].cbfifo_array)[gfifos[cbf_handle].write] = ((uint8_t*)buf)[i];
            gfifos[cbf_handle].write = (gfifos[cbf_handle].write + 1) & (cbfifo_capacity(cbf_handle) - 1);

            i++;
            nbyte--;

            /* Check if cbfifo has become full due to addition of one byte */
            if(gfifos[cbf_handle].write == gfifos[cbf_handle].read)
            {
            	gfifos[cbf_handle].state = FULL;

                /* If still bytes are pending to be written */
                if(nbyte > 0 )
                    cb_error = FULL_FIFO;
            }
        }

        /* Update the length of the CBFIFO */
        gfifos[cbf_handle].length += i;

        /* Enable the interrupts */
        __set_PRIMASK(masking_state);

        /* Return the number of bytes enqueued*/
        return i;
    }
}

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
size_t cbfifo_dequeue(cbfifo_handle_t cbf_handle, void *buf, size_t nbyte)
{
	/* Disable the interrupts */
	uint32_t masking_state;
	masking_state = __get_PRIMASK();
	__disable_irq();

    cb_error = NO_ERROR;
    /* Check if pointer is NULL */
    if(buf == NULL)
    {
        cb_error = INVALID_BUF;
        __set_PRIMASK(masking_state);
        return -1;
    }

    /* Return 0 if nothing needs to be dequeued */
    if(nbyte == 0)
    {
    	__set_PRIMASK(masking_state);
        return 0;
    }

    /* If nbyte is larger than length then set it to length */
    if(nbyte > gfifos[cbf_handle].length)
    {
        nbyte = gfifos[cbf_handle].length;
    }
    size_t i = 0;

    /* If cbfifo is empty */
    if(gfifos[cbf_handle].state == EMPTY)
    {
        cb_error = EMPTY_FIFO;
        __set_PRIMASK(masking_state);
        return 0;
    }
    /* If cbfifo is not empty */
    else
    {
        /* Update state of cbfifo */
    	gfifos[cbf_handle].state = PARTIALLY_FILLED;
        while( (gfifos[cbf_handle].state != EMPTY) && (nbyte > 0) )
        {
            /* Copy elements from the cbfifo to the buffer and update read location */
            ((uint8_t*)buf)[i] = ((uint8_t*)gfifos[cbf_handle].cbfifo_array)[gfifos[cbf_handle].read];

            /* Can set the dequeued buffer position to a fixed value */
            ((uint8_t*)gfifos[cbf_handle].cbfifo_array)[gfifos[cbf_handle].read] = '"';
            gfifos[cbf_handle].read = (gfifos[cbf_handle].read + 1) & (cbfifo_capacity(cbf_handle) - 1);

            i++;
            nbyte--;

            /* Check if cbfifo has become empty due to removal of one byte */
            if(gfifos[cbf_handle].write == gfifos[cbf_handle].read)
            {
            	gfifos[cbf_handle].state = EMPTY;

                /* If still bytes are pending to be read */
                if(nbyte > 0 )
                    cb_error = EMPTY_FIFO;

            }
        }

        /* Update the length of the CBFIFO */
        gfifos[cbf_handle].length -= i;

        /* Enable the interrupts */
        __set_PRIMASK(masking_state);

        /* Return the number of bytes dequeued*/
        return i;
        //return ((uint8_t*)buf)[0];
    }
}
/*********************************************************************************
 * @brief   :   Returns the number of bytes currently on the cbfifo.
 *
 * @param   :   cbf_handle - handle of cbfifo
 *
 * @return  :   size_t  - number of bytes currently available to be dequeued
 *                        from the FIFO
 *
*********************************************************************************/
size_t cbfifo_length(cbfifo_handle_t cbf_handle)
{
    return gfifos[cbf_handle].length;
}
/*********************************************************************************
 * @brief   :   Returns the capacity of the cbfifo.
 *
 * @param   :   cbf_handle - handle of cbfifo
 *
 * @return  :   size_t  - capacity of the cbfifo in bytes.
 *
*********************************************************************************/
size_t cbfifo_capacity(cbfifo_handle_t cbf_handle)
{
	return CBFIFO_CAPACITY;
}

