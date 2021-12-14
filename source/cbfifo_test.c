/**
 * @file    :   test_cbfifo.c
 * @brief   :   An abstraction for testing the cbfifo
 *
 *              This source file provides an abstraction of testing function
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

#include <stdio.h>
#include <assert.h>
#include <inc/cbfifo.h>
#include <inc/cbfifo_test.h>
#include <string.h>

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
void test_cbfifo()
{
	char *str ="To be, or not to be: that is the question:\n"
	"Whether 'tis nobler in the mind to suffer\n"
	"The slings and arrows of outrageous fortune,\n"
	"Or to take arms against a sea of troubles,\n"
	"And by opposing end them? To die, to sleep--\n"
	"No more--and by a sleep to say we end\n"
	"The heart-ache and the thousand natural shocks\n"
	"That flesh is heir to, 'tis a consummation\n"
	"Devoutly to be wish'd. To die, to sleep;\n"
	"To sleep: perchance to dream: ay, there's the rub;\n"
	"For in that sleep of death what dreams may come\n"
	"When we have shuffled off this mortal coil,\n"
	"Must give us pause.";

	char bufTx[1024];
	const int capTx = cbfifo_capacity(kTx_FIFO);
	int chunk_size_Tx = (capTx-2) / 4;
	int wposTx=0, rposTx=0;

	char bufRx[1024];
	const int capRx = cbfifo_capacity(kRx_FIFO);
	int chunk_size_Rx = (capRx-2) / 4;
	int wposRx=0, rposRx=0;

	// asserts in following 2 lines -- this is not testing the student,
	// it's validating that the test is correct
	assert(strlen(str) >= capTx*2);
	assert(sizeof(bufTx) > capTx);
	assert(capTx == 256 || capTx == 127);

	// asserts in following 2 lines -- this is not testing the student,
	// it's validating that the test is correct
	assert(strlen(str) >= capRx*2);
	assert(sizeof(bufRx) > capRx);
	assert(capRx == 256 || capRx == 127);

	assert(cbfifo_length(kTx_FIFO) == 0);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, capTx) == 0);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 1) == 0);

	// enqueue 10 bytes, then dequeue same amt
	assert(cbfifo_enqueue(kTx_FIFO, str, 10) == 10);
	assert(cbfifo_length(kTx_FIFO) == 10);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 10) == 10);
	assert(strncmp(bufTx, str, 10) == 0);
	assert(cbfifo_length(kTx_FIFO) == 0);

	// enqueue 20 bytes;  dequeue 5, then another 20
	assert(cbfifo_enqueue(kTx_FIFO, str, 20) == 20);
	assert(cbfifo_length(kTx_FIFO) == 20);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 5) == 5);
	assert(cbfifo_length(kTx_FIFO) == 15);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx+5, 20) == 15);
	assert(cbfifo_length(kTx_FIFO) == 0);
	assert(strncmp(bufTx, str, 20) == 0);

	assert(cbfifo_length(kRx_FIFO) == 0);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, capRx) == 0);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 1) == 0);

	// enqueue 10 bytes, then dequeue same amt
	assert(cbfifo_enqueue(kRx_FIFO, str, 10) == 10);
	assert(cbfifo_length(kRx_FIFO) == 10);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 10) == 10);
	assert(strncmp(bufRx, str, 10) == 0);
	assert(cbfifo_length(kRx_FIFO) == 0);

	// fill buffer and then read it back out
	assert(cbfifo_enqueue(kTx_FIFO, str, capTx) == capTx);
	assert(cbfifo_length(kTx_FIFO) == capTx);
	assert(cbfifo_enqueue(kTx_FIFO, str, 1) == 0);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, capTx) == capTx);
	assert(cbfifo_length(kTx_FIFO) == 0);
	assert(strncmp(bufTx, str, capTx) == 0);

	// Add 20 bytes and pull out 18
	assert(cbfifo_enqueue(kTx_FIFO, str, 20) == 20);
	assert(cbfifo_length(kTx_FIFO) == 20);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 18) == 18);
	assert(cbfifo_length(kTx_FIFO) == 2);
	assert(strncmp(bufTx, str, 18) == 0);

	// Now add a bunch of data in 4 chunks

	for (int i=0; i<4; i++) {
	assert(cbfifo_enqueue(kTx_FIFO, str+i*chunk_size_Tx, chunk_size_Tx) == chunk_size_Tx);
	assert(cbfifo_length(kTx_FIFO) == (i+1)*chunk_size_Tx + 2);
	}
	assert(cbfifo_length(kTx_FIFO) == 4*chunk_size_Tx + 2);

	// Take out the 2 remaining bytes from above
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 2) == 2);
	assert(strncmp(bufTx, str+18, 2) == 0);

	// now read those chunks out a byte at a time
	for (int i=0; i<chunk_size_Tx*4; i++) {
	assert(cbfifo_dequeue(kTx_FIFO, bufTx+i, 1) == 1);
	assert(cbfifo_length(kTx_FIFO) == chunk_size_Tx*4 - i - 1);
	}
	assert(strncmp(bufTx, str, chunk_size_Tx*4) == 0);

	// enqueue 20 bytes;  dequeue 5, then another 20
	assert(cbfifo_enqueue(kRx_FIFO, str, 20) == 20);
	assert(cbfifo_length(kRx_FIFO) == 20);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 5) == 5);
	assert(cbfifo_length(kRx_FIFO) == 15);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx+5, 20) == 15);
	assert(cbfifo_length(kRx_FIFO) == 0);
	assert(strncmp(bufRx, str, 20) == 0);

	// fill buffer and then read it back out
	assert(cbfifo_enqueue(kRx_FIFO, str, capRx) == capRx);
	assert(cbfifo_length(kRx_FIFO) == capRx);
	assert(cbfifo_enqueue(kRx_FIFO, str, 1) == 0);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, capRx) == capRx);
	assert(cbfifo_length(kRx_FIFO) == 0);
	assert(strncmp(bufRx, str, capRx) == 0);

	// write more than capacity
	assert(cbfifo_enqueue(kTx_FIFO, str, 65) == 65);
	assert(cbfifo_enqueue(kTx_FIFO, str+65, capTx) == capTx-65);
	assert(cbfifo_length(kTx_FIFO) == capTx);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, capTx) == capTx);
	assert(cbfifo_length(kTx_FIFO) == 0);
	assert(strncmp(bufTx, str, capTx) == 0);

	// write zero bytes
	assert(cbfifo_enqueue(kTx_FIFO, str, 0) == 0);
	assert(cbfifo_length(kTx_FIFO) == 0);

	// Exercise the following conditions:
	//    enqueue when read < write:
	//        bytes < CAP-write  (1)
	//        bytes exactly CAP-write  (2)
	//        bytes > CAP-write but < space available (3)
	//        bytes exactly the space available (4)
	//        bytes > space available (5)
	assert(cbfifo_enqueue(kTx_FIFO, str, 32) == 32);  // advance so that read < write
	assert(cbfifo_length(kTx_FIFO) == 32);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 16) == 16);
	assert(cbfifo_length(kTx_FIFO) == 16);
	assert(strncmp(bufTx, str, 16) == 0);

	// Add 20 bytes and pull out 18
	assert(cbfifo_enqueue(kRx_FIFO, str, 20) == 20);
	assert(cbfifo_length(kRx_FIFO) == 20);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 18) == 18);
	assert(cbfifo_length(kRx_FIFO) == 2);
	assert(strncmp(bufRx, str, 18) == 0);

	// Now add a bunch of data in 4 chunks

	for (int i=0; i<4; i++) {
	assert(cbfifo_enqueue(kRx_FIFO, str+i*chunk_size_Rx, chunk_size_Rx) == chunk_size_Rx);
	assert(cbfifo_length(kRx_FIFO) == (i+1)*chunk_size_Rx + 2);
	}
	assert(cbfifo_length(kRx_FIFO) == 4*chunk_size_Rx + 2);

	assert(cbfifo_enqueue(kTx_FIFO, str+32, 32) == 32);  // (1)
	assert(cbfifo_length(kTx_FIFO) == 48);
	assert(cbfifo_enqueue(kTx_FIFO, str+64, capTx-64) == capTx-64);  // (2)
	assert(cbfifo_length(kTx_FIFO) == capTx-16);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx+16, capTx-16) == capTx-16);
	assert(strncmp(bufTx, str, capTx) == 0);

	assert(cbfifo_enqueue(kTx_FIFO, str, 32) == 32);  // advance so that read < write
	assert(cbfifo_length(kTx_FIFO) == 32);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 16) == 16);
	assert(cbfifo_length(kTx_FIFO) == 16);
	assert(strncmp(bufTx, str, 16) == 0);

	// Take out the 2 remaining bytes from above
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 2) == 2);
	assert(strncmp(bufRx, str+18, 2) == 0);

	// now read those chunks out a byte at a time
	for (int i=0; i<chunk_size_Rx*4; i++) {
	assert(cbfifo_dequeue(kRx_FIFO, bufRx+i, 1) == 1);
	assert(cbfifo_length(kRx_FIFO) == chunk_size_Rx*4 - i - 1);
	}
	assert(strncmp(bufRx, str, chunk_size_Rx*4) == 0);

	// write more than capacity
	assert(cbfifo_enqueue(kRx_FIFO, str, 65) == 65);
	assert(cbfifo_enqueue(kRx_FIFO, str+65, capRx) == capRx-65);
	assert(cbfifo_length(kRx_FIFO) == capRx);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, capRx) == capRx);
	assert(cbfifo_length(kRx_FIFO) == 0);
	assert(strncmp(bufRx, str, capRx) == 0);

	assert(cbfifo_enqueue(kTx_FIFO, str+32, capTx-20) == capTx-20);  // (3)
	assert(cbfifo_length(kTx_FIFO) == capTx-4);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, capTx-8) == capTx-8);
	assert(strncmp(bufTx, str+16, capTx-8) == 0);
	assert(cbfifo_length(kTx_FIFO) == 4);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 8) == 4);
	assert(strncmp(bufTx, str+16+capTx-8, 4) == 0);
	assert(cbfifo_length(kTx_FIFO) == 0);

	assert(cbfifo_enqueue(kTx_FIFO, str, 49) == 49);  // advance so that read < write
	assert(cbfifo_length(kTx_FIFO) == 49);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 16) == 16);
	assert(cbfifo_length(kTx_FIFO) == 33);
	assert(strncmp(bufTx, str, 16) == 0);

	// write zero bytes
	assert(cbfifo_enqueue(kRx_FIFO, str, 0) == 0);
	assert(cbfifo_length(kRx_FIFO) == 0);

	// Exercise the following conditions:
	//    enqueue when read < write:
	//        bytes < CAP-write  (1)
	//        bytes exactly CAP-write  (2)
	//        bytes > CAP-write but < space available (3)
	//        bytes exactly the space available (4)
	//        bytes > space available (5)
	assert(cbfifo_enqueue(kRx_FIFO, str, 32) == 32);  // advance so that read < write
	assert(cbfifo_length(kRx_FIFO) == 32);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 16) == 16);
	assert(cbfifo_length(kRx_FIFO) == 16);
	assert(strncmp(bufRx, str, 16) == 0);

	assert(cbfifo_enqueue(kRx_FIFO, str+32, 32) == 32);  // (1)
	assert(cbfifo_length(kRx_FIFO) == 48);
	assert(cbfifo_enqueue(kRx_FIFO, str+64, capRx-64) == capRx-64);  // (2)
	assert(cbfifo_length(kRx_FIFO) == capRx-16);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx+16, capRx-16) == capRx-16);
	assert(strncmp(bufRx, str, capRx) == 0);

	assert(cbfifo_enqueue(kTx_FIFO, str+49, capTx-33) == capTx-33);  // (4)
	assert(cbfifo_length(kTx_FIFO) == capTx);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, capTx) == capTx);
	assert(cbfifo_length(kTx_FIFO) == 0);
	assert(strncmp(bufTx, str+16, capTx) == 0);

	assert(cbfifo_enqueue(kRx_FIFO, str, 32) == 32);  // advance so that read < write
	assert(cbfifo_length(kRx_FIFO) == 32);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 16) == 16);
	assert(cbfifo_length(kRx_FIFO) == 16);
	assert(strncmp(bufRx, str, 16) == 0);

	assert(cbfifo_enqueue(kRx_FIFO, str+32, capRx-20) == capRx-20);  // (3)
	assert(cbfifo_length(kRx_FIFO) == capRx-4);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, capRx-8) == capRx-8);
	assert(strncmp(bufRx, str+16, capRx-8) == 0);
	assert(cbfifo_length(kRx_FIFO) == 4);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 8) == 4);
	assert(strncmp(bufRx, str+16+capRx-8, 4) == 0);
	assert(cbfifo_length(kRx_FIFO) == 0);

	assert(cbfifo_enqueue(kRx_FIFO, str, 49) == 49);  // advance so that read < write
	assert(cbfifo_length(kRx_FIFO) == 49);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 16) == 16);
	assert(cbfifo_length(kRx_FIFO) == 33);
	assert(strncmp(bufRx, str, 16) == 0);

	assert(cbfifo_enqueue(kRx_FIFO, str+49, capRx-33) == capRx-33);  // (4)
	assert(cbfifo_length(kRx_FIFO) == capRx);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, capRx) == capRx);
	assert(cbfifo_length(kRx_FIFO) == 0);
	assert(strncmp(bufRx, str+16, capRx) == 0);


	assert(cbfifo_enqueue(kTx_FIFO, str, 32) == 32);  // advance so that read < write
	assert(cbfifo_length(kTx_FIFO) == 32);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 16) == 16);
	assert(cbfifo_length(kTx_FIFO) == 16);
	assert(strncmp(bufTx, str, 16) == 0);

	assert(cbfifo_enqueue(kTx_FIFO, str+32, capTx) == capTx-16);  // (5)
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 1) == 1);
	assert(cbfifo_length(kTx_FIFO) == capTx-1);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx+1, capTx-1) == capTx-1);
	assert(cbfifo_length(kTx_FIFO) == 0);
	assert(strncmp(bufTx, str+16, capTx) == 0);

	//    enqueue when write < read:
	//        bytes < read-write (6)
	//        bytes exactly read-write (= the space available) (7)
	//        bytes > space available (8)

	assert(cbfifo_enqueue(kTx_FIFO, str, capTx-4) == capTx-4);
	wposTx += capTx-4;
	assert(cbfifo_length(kTx_FIFO) == capTx-4);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 32) == 32);
	rposTx += 32;
	assert(cbfifo_length(kTx_FIFO) == capTx-36);
	assert(strncmp(bufTx, str, 32) == 0);
	assert(cbfifo_enqueue(kTx_FIFO, str+wposTx, 12) == 12);
	wposTx += 12;
	assert(cbfifo_length(kTx_FIFO) == capTx-24);

	assert(cbfifo_enqueue(kRx_FIFO, str, 32) == 32);  // advance so that read < write
	assert(cbfifo_length(kRx_FIFO) == 32);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 16) == 16);
	assert(cbfifo_length(kRx_FIFO) == 16);
	assert(strncmp(bufRx, str, 16) == 0);

	assert(cbfifo_enqueue(kRx_FIFO, str+32, capRx) == capRx-16);  // (5)
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 1) == 1);
	assert(cbfifo_length(kRx_FIFO) == capRx-1);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx+1, capRx-1) == capRx-1);
	assert(cbfifo_length(kRx_FIFO) == 0);
	assert(strncmp(bufRx, str+16, capRx) == 0);

	//    enqueue when write < read:
	//        bytes < read-write (6)
	//        bytes exactly read-write (= the space available) (7)
	//        bytes > space available (8)

	assert(cbfifo_enqueue(kRx_FIFO, str, capRx-4) == capRx-4);
	wposRx += capRx-4;
	assert(cbfifo_length(kRx_FIFO) == capRx-4);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 32) == 32);
	rposRx += 32;
	assert(cbfifo_length(kRx_FIFO) == capRx-36);
	assert(strncmp(bufRx, str, 32) == 0);
	assert(cbfifo_enqueue(kRx_FIFO, str+wposRx, 12) == 12);
	wposRx += 12;
	assert(cbfifo_length(kRx_FIFO) == capRx-24);

	assert(cbfifo_enqueue(kTx_FIFO, str+wposTx, 16) == 16);  // (6)
	assert(cbfifo_length(kTx_FIFO) == capTx-8);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, capTx) == capTx-8);
	assert(cbfifo_length(kTx_FIFO) == 0);
	assert(strncmp(bufTx, str+rposTx, capTx-8) == 0);

	// reset
	wposTx=0;
	rposTx=0;
	assert(cbfifo_enqueue(kTx_FIFO, str, capTx-4) == capTx-4);
	wposTx += capTx-4;
	assert(cbfifo_length(kTx_FIFO) == capTx-4);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 32) == 32);
	rposTx += 32;
	assert(cbfifo_length(kTx_FIFO) == capTx-36);
	assert(strncmp(bufTx, str, 32) == 0);
	assert(cbfifo_enqueue(kTx_FIFO, str+wposTx, 12) == 12);
	wposTx += 12;
	assert(cbfifo_length(kTx_FIFO) == capTx-24);

	assert(cbfifo_enqueue(kTx_FIFO, str+wposTx, 24) == 24);  // (7)
	assert(cbfifo_length(kTx_FIFO) == capTx);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, capTx) == capTx);
	assert(cbfifo_length(kTx_FIFO) == 0);
	assert(strncmp(bufTx, str+rposTx, capTx) == 0);

	assert(cbfifo_enqueue(kRx_FIFO, str+wposRx, 16) == 16);  // (6)
	assert(cbfifo_length(kRx_FIFO) == capRx-8);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, capRx) == capRx-8);
	assert(cbfifo_length(kRx_FIFO) == 0);
	assert(strncmp(bufRx, str+rposRx, capRx-8) == 0);

	// reset
	wposRx=0;
	rposRx=0;
	assert(cbfifo_enqueue(kRx_FIFO, str, capRx-4) == capRx-4);
	wposRx += capRx-4;
	assert(cbfifo_length(kRx_FIFO) == capRx-4);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 32) == 32);
	rposRx += 32;
	assert(cbfifo_length(kRx_FIFO) == capRx-36);
	assert(strncmp(bufRx, str, 32) == 0);
	assert(cbfifo_enqueue(kRx_FIFO, str+wposRx, 12) == 12);
	wposRx += 12;
	assert(cbfifo_length(kRx_FIFO) == capRx-24);

	assert(cbfifo_enqueue(kRx_FIFO, str+wposRx, 24) == 24);  // (7)
	assert(cbfifo_length(kRx_FIFO) == capRx);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, capRx) == capRx);
	assert(cbfifo_length(kRx_FIFO) == 0);
	assert(strncmp(bufRx, str+rposRx, capRx) == 0);

	// reset
	wposTx=0;
	rposTx=0;
	assert(cbfifo_enqueue(kTx_FIFO, str, capTx-4) == capTx-4);
	wposTx += capTx-4;
	assert(cbfifo_length(kTx_FIFO) == capTx-4);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, 32) == 32);
	rposTx += 32;
	assert(cbfifo_length(kTx_FIFO) == capTx-36);
	assert(strncmp(bufTx, str, 32) == 0);
	assert(cbfifo_enqueue(kTx_FIFO, str+wposTx, 12) == 12);
	wposTx += 12;
	assert(cbfifo_length(kTx_FIFO) == capTx-24);

	assert(cbfifo_enqueue(kTx_FIFO, str+wposTx, 64) == 24);  // (8)
	assert(cbfifo_length(kTx_FIFO) == capTx);
	assert(cbfifo_dequeue(kTx_FIFO, bufTx, capTx) == capTx);
	assert(cbfifo_length(kTx_FIFO) == 0);
	assert(strncmp(bufTx, str+rposTx, capTx) == 0);

	// reset
	wposRx=0;
	rposRx=0;
	assert(cbfifo_enqueue(kRx_FIFO, str, capRx-4) == capRx-4);
	wposRx += capRx-4;
	assert(cbfifo_length(kRx_FIFO) == capRx-4);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, 32) == 32);
	rposRx += 32;
	assert(cbfifo_length(kRx_FIFO) == capRx-36);
	assert(strncmp(bufRx, str, 32) == 0);
	assert(cbfifo_enqueue(kRx_FIFO, str+wposRx, 12) == 12);
	wposRx += 12;
	assert(cbfifo_length(kRx_FIFO) == capRx-24);

	assert(cbfifo_enqueue(kRx_FIFO, str+wposRx, 64) == 24);  // (8)
	assert(cbfifo_length(kRx_FIFO) == capRx);
	assert(cbfifo_dequeue(kRx_FIFO, bufRx, capRx) == capRx);
	assert(cbfifo_length(kRx_FIFO) == 0);
	assert(strncmp(bufRx, str+rposRx, capRx) == 0);


	//printf("%s: passed all test cases\n\r", __FUNCTION__);
}

