PES Final Project : Data Compression for UART Debugging  
Author - Sanish Kharade

This is the repository for the final project of Principles of Embedded Software course
taught at University of Colorado Boulder

Github URL - 

About the application 
This application is used for reducing the amount of data sent over UART during  
debugging a system. This is done using Huffman encoding. It is a user interface based application  
which has a few pre defined commands.

Please refer the pdf for entire overview of the implementation.   

This repository consists of multiple folders from the MCUXpresso IDE.  
Important folders to pay attention to are -  
 
source : contains 9 source files for the program  
main.c      	- Main entry point to the program  
uart.c		- Contains all the UART functions  
cbfifo.c	- Contains all the cbfifo functions  
cbfifo_test.c	- Contains the cbfifo test function  
commands.c 	- Contains all commands    
sysclock.c	- Contains the sysclock function    
huffman.c	- Contains huffman encoding and decoding functions
huffman_test.c	- Contains the huffman test function
systick.c	- Contains the systick functions
 
inc : contains 8 header files for the program  
uart.h		- Header file for uart.c  
cbfifo.h	- Header file for cbfifo.c  
cbfifo_test.h	- Header file for cbfifo_test.c  
commands.h 	- Header file for commands.c 
sysclock.h	- Header file for sysclock.c  
huffman.h	- Header file for huffman.c
huffman_test.h	- Header file for huffman_test.c
systick.h	- Header file for systick.c
lookup_table.h	- Header file containing the huffman table

The folder windows_files contains all the files for windows serial communication  
and huffman tree generation  
It has 3 source files
huffman_tree.c 	- For generating the lookup table  
huffman_code.c	- For huffman encoding and decoding functions  
serial_port.c	- For serial communication with the microcontroller  

It has 3 header files
huffman_tree.c 	- Header file for huffman_tree.c   
huffman_code.c	- Header file for huffman_code.c 
lookup_table.h	- Header file consisting of the lookup table

It has one text file
log.txt	- logs used to generate the lookup table
 
The repository also contains driver files and library APIs which we havent used in the program    

This program was built on a Windows platform using MCUXpresso as the IDE  

To run the program -  

Clone the repository to your device  
Import the project into your workspace. 
There is no need to include the windows files in the MCUXpresso workspace.
Copy them in a separate path.    

The repository has a prebuilt lookup_table.h file which was generated from the log.txt file.
If you want to generate your own lookup table, replace my log file with yours.
Go to the path of the windows files.  
Run the commands
gcc huffman_tree.c -o huffman_tree.exe
./huffman_tree.exe > lookup_table.h  

Now the new huffman table will be created in your lookup_table.h file 

Now build and run the MCUXpresso project.
To start the serial communication, run the following commands
gcc serial_port.c huffman_code.c -o serial_port.exe  
./serial_port.exe

You should see the message "Opening serial port succesful!"

Now you can enter the commands and see the results.

In case the program gets stuck or there is an error in opening the serial port, 
disconnect and reconnect the board (remove it from the debugging state).
Also rerun the serial program using the above commands.  

In case that doesn't work, then open PuTTy just before running the serial conmmand on windows.  
   
  
NOTE -   
In case the project settings dont get imported into your workspace, you might get an error   
about the files in the inc folder.  
To solve this -   
Go to Project -> Properties -> C/C++ Build -> Settings -> MCU C Compiler -> Includes   
Here click on add and add the path to the inc folder  
