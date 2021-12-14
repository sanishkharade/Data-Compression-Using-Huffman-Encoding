/**
 * @file    :   uart.c
 * @brief   :   An abstraction for UART functions
 *
 *              This source file provides functions which are used to write and
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
#include <stdio.h>
#include <string.h>
#include "cbfifo.h"
#include "uart.h"
#include "huffman.h"
#include "systick.h"

#define UART_OVERSAMPLE_RATE 	(16)
#define BUS_CLOCK 				(24e6)
#define SYS_CLOCK				(24e6)

#define UART_BAUDRATE			(9600)
#define UART_DATA_SIZE			(8)
#define UART_PARITY				(0)
#define UART_STOP_BITS			(2)


/* Structure for the stats */
struct
{
	uint16_t bytes;
	uint16_t reduced_bytes;
	uint16_t timer;
	bool custom_string;
}stats;

static uint32_t error_counter = 0;


/*********************************************************************************
 * @brief   :   Initializes the UART
 *
 * @param   :   baud_rate - baud rate for the UART
 *
 * @return  :   void
*********************************************************************************/
void Init_UART0(void)
{
	uint16_t sbr;
	uint8_t temp;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 48 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);
	//SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK)/(UART_BAUDRATE * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	// Disable interrupts for RX active edge and LIN break detect, select two stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(1) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(0) | UART0_C1_PE(0);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0)
			| UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Try it a different way
	UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
									UART0_S1_FE_MASK | UART0_S1_PF_MASK;

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	/* Enable interrupts */
	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);


	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	// Clear the UART RDRF flag
	temp = UART0->D;
	UART0->S1 &= ~UART0_S1_RDRF_MASK;

}

/*********************************************************************************
 * @brief   :   Interrupt Handler for UART0
 *
 * @param   :   none
 *
 * @return  :   void
*********************************************************************************/
void UART0_IRQHandler(void)
{
	uint8_t ch;


	/* UART Error */
	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK |
		UART_S1_FE_MASK | UART_S1_PF_MASK))
	{
		// clear the error flags
		UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK |
								UART0_S1_FE_MASK | UART0_S1_PF_MASK;
		// read the data register to clear RDRF
		ch = UART0->D;
	}
	/* Receive Interrupt */
	if (UART0->S1 & UART0_S1_RDRF_MASK) {
		// received a character
		ch = UART0->D;

		if(cbfifo_enqueue(kRx_FIFO, &ch, 1) != 1)
		{
			/* For bytes that were silently ignored */
			error_counter++;
		}
	}
	/* Transmit Interrupt */
	if ( (UART0->C2 & UART0_C2_TIE_MASK) && // transmitter interrupt enabled
			(UART0->S1 & UART0_S1_TDRE_MASK) )
	{

		if(cbfifo_dequeue(kTx_FIFO, &ch, 1) == 1)//return 0 or 1
			UART0->D = ch;
		else
		{
			UART0->C2 &= ~UART0_C2_TIE_MASK;
			stats.timer += get_timer();
		}
	}
}

/*********************************************************************************
 * @brief   :   Function to read data from UART
 *
 *              This is a predefined function which is being overwritten here.
 *              getchar() will call this function to get data from the UART
 *
 * @param   :   none
 *
 * @return  :   int	- character read from user
 * 				-1	- error
*********************************************************************************/
int __sys_readc(void)
{
	int c;

	/* Wait until a character is received */
	while(cbfifo_length(kRx_FIFO) == 0);

	if(cbfifo_dequeue(kRx_FIFO, &c, 1))
		return c;
	else
		return -1;

}


/*********************************************************************************
 * @brief   :   Function to write data onto UART
 *
 *              This is a predefined function which is being overwritten here.
 *              putchar() and printf() will call this function to print data on the UART
 *
 * @param   :   handle	- where the data is to be printed
 * 				buf		- character array containing the data
 * 				size	- number of bytes to be printed
 *
 * @return  :   int	- character read from user
 * 				-1	- error
*********************************************************************************/
int __sys_write(int handle, char *buf, int size)
{
	//static uint16_t bytes = 0, reduced_bytes = 0;

//	if(stats.custom_string == true)
//	{
//		stats.bytes += size;
//	}
	stats.bytes += size;
	uint8_t buffer[300]={0};
	uint32_t bits=0;
	//size += cbfifo_length(kTx_FIFO);
	if(cbfifo_enqueue(kTx_FIFO, &size, 1) != 1)
	{
		return -1;
	}
	size = huffman_encode(buf, buffer, sizeof(buffer));
	if(cbfifo_enqueue(kTx_FIFO, &size, 1) != 1)
	{
		return -1;
	}
	if(size % 8 != 0)
	{
		size = (size/8) + 1;
	}
	else
	{
		size /= 8;
	}
	if(cbfifo_enqueue(kTx_FIFO, &size, 1) != 1)
	{
		return -1;
	}
//	if(stats.custom_string == true)
//	{
//		stats.reduced_bytes += size;
//		stats.custom_string = false;
//	}
	stats.reduced_bytes += size;
	//stats.custom_string = false;
	/* Wait until there is enough room on the Tx fifo */
	while(size >= cbfifo_capacity(kTx_FIFO) - cbfifo_length(kTx_FIFO));

	if(cbfifo_enqueue(kTx_FIFO, buffer, size) != size)
	{
		return -1;
	}


//	if(cbfifo_enqueue(kTx_FIFO, buf, size) != size)
//	{
//		return -1;
//	}

	/* If Tx cbfifo is not empty, enable the transmit interrupt */
	if (cbfifo_length(kTx_FIFO))
	{
		UART0->C2 |= UART0_C2_TIE(1);
		reset_timer();
		return 0;
	}
	else
	{
		// queue is empty so disable transmitter interrupt
		UART0->C2 &= ~UART0_C2_TIE_MASK;
		return 0;
	}
}
/*
 * Stat functions are included in the UART file because
 * all of them are stats related to the UART
 *
 * */

/*********************************************************************************
 * @brief   :   Resets the data and time stats to zero
 *
 * @param   :   none
 *
 * @return  :   void
*********************************************************************************/
void reset_stats(void)
{
	stats.bytes = 0;
	stats.reduced_bytes = 0;
	stats.timer = 0;
}
/*********************************************************************************
 * @brief   :   Prints the data and time stats to zero
 *
 * @param   :   none
 *
 * @return  :   void
*********************************************************************************/
void print_stats(void)
{
	uint8_t buff[200]={0};
	uint8_t reduction = ((stats.bytes - stats.reduced_bytes)*100)/ stats.bytes;
	sprintf(buff, "Original Bytes = %d\n\rReduced bytes = %d\n\rPercent Reduction = %d percent\n\rTime = %d ms\n\r",stats.bytes, stats.reduced_bytes, reduction, stats.timer);

	printf("%s", buff);
}
/*********************************************************************************
 * @brief   :   Sets the custom string member of the stats structure to true
 *
 * @param   :   none
 *
 * @return  :   void
*********************************************************************************/
void set_custom_string_true(void)
{
	stats.custom_string = true;
}
