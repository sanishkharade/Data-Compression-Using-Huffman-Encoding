/**
 * @file    :   commands.c
 * @brief   :   An abstraction of all commands
 *
 *              This source file provides functions which are used to handle the
 *              commands of the application.
 *
 * @author  :   Sanish Kharade
 * @date    :   December 10, 2021
 * @version :   1.0
 *
 * @tools   :   MCUXpresso IDE
 *
 * @link    :   -
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "commands.h"
#include "huffman.h"
#include "uart.h"


/* Function Pointer */
typedef void (*command_handler_t)(int, char *argv[]);
typedef struct
{
    const char *name;
    command_handler_t handler;
    const char *help_string;
} command_table_t;

/* Command Table
 * This table is an array of structures which consists of the function name,
 * function handle and a help string for each command
 * */
static const command_table_t commands[] = {

		{"author", handle_author, "\n\r\t\tPrint the author of this code\n\r"},
		{"help", handle_help, "\n\r\t\tPrint this help message\n\r"},
		{"stats", handle_stats, "\n\r\t\tPrint the statistics\n\r"},
		{"reset", handle_reset, "\n\r\t\tReset the timer and byte stats\n\r"}
};

/* Statically defined length of command table */
static const int num_commands =
  sizeof(commands) / sizeof(command_table_t);


/*********************************************************************************
 * @brief   :   Function to process user input
 *
 *              This function processes the user input, stores it in a buffer and
 *              passes that buffer to the process command when user presses Enter
 *
 * @param   :   none
 *
 * @return  :   void
*********************************************************************************/
void process_user_input(void)
{

    uint8_t c = 0;
    int i = 0;
    char buffer[100] = {0};

    while(1)
    {
    	while(c != '\r' && c != '\n')
    	{
    		c = getchar();
    		if(c == '\b' || c == 127) //del=127
    		{
    			/* Remove previous character from terminal */
    			if(i > 0)
    			{
    				buffer[--i] = 0;
    			}
    		}
    		else
    		{
    			buffer[i++] = c;
    		}
    	}
    	c = 0;
    	process_command(buffer);
    	memset(buffer, 0, 100);
    	i=0;

    }

}


/*********************************************************************************
 * @brief   :   Function to check if input for dump command is valid
 *
 * @param   :   ch	- argument received by dump command
 * 				len	- length of argument
 *
 * @return  :   bool	- true 	- if argument is valid
 * 					 	- false	- if argument is invalid
*********************************************************************************/
static bool checkHex(char *ch, int len)
{
	for(int i=0; i< len;i++)
	{
        if ((ch[i] < '0' || ch[i] > '9') &&
            (ch[i] < 'A' || ch[i] > 'F') &&
            (ch[i] < 'a' || ch[i] > 'f') &&
			(ch[i] != 'x' && ch[i] != 'X'))
        {
        	printf("Invalid argument\n\r");
        	return false;
        }
	}
	return true;
}

/*********************************************************************************
 * @brief   :   Function to handle the author command
 *
 * @param   :   argc  	- Number of arguments
 * 				argv	- Array of arguments
 *
 * @return  :   void
*********************************************************************************/
void handle_author(int argc, char *argv[])
{
	/* Check valid number of arguments */
	if(argc > 1)
	{
		printf("Too many arguments for the author command\n\rEnter help command for syntax of all commands\n\r");
		return;
	}
	printf("Sanish Kharade\n\r");
}


/*********************************************************************************
 * @brief   :   Function to handle the help command
 *
 * @param   :   argc  	- Number of arguments
 * 				argv	- Array of arguments
 *
 * @return  :   void
*********************************************************************************/
void handle_help(int argc, char *argv[])
{
	/* Check valid number of arguments */
	if(argc > 1)
	{
		printf("Too many arguments for the help command\n\rEnter help command for syntax of all commands\n\r");
		return;
	}
	uint8_t buff[100]={0};
	uint8_t buff2[300]={0};

	strcat(buff2, "PES Final Project\n\r");
	for(int i = 0; i < num_commands; i++)
	{
		sprintf(buff, "%s%s", commands[i].name, commands[i].help_string);
		strcat(buff2, buff);

		memset(buff, 0, sizeof(buff));
	}

	strcat(buff2, "\n\rEnter anything else for encoding and decoding over the serial port\n\r");

	printf("%s", buff2);
}
/*********************************************************************************
 * @brief   :   Function to handle the reset command
 *
 * @param   :   argc  	- Number of arguments
 * 				argv	- Array of arguments
 *
 * @return  :   void
*********************************************************************************/
void handle_reset(int argc, char *argv[])
{
	/* Check valid number of arguments */
	if(argc > 1)
	{
		printf("Too many arguments for the reset command\n\rEnter help command for syntax of all commands\n\r");
		return;
	}

	printf("Timer and Bytes Reset\n\r");

	reset_stats();

}
/*********************************************************************************
 * @brief   :   Function to handle the stats command
 *
 * @param   :   argc  	- Number of arguments
 * 				argv	- Array of arguments
 *
 * @return  :   void
*********************************************************************************/
void handle_stats(int argc, char *argv[])
{

	if(argc > 1)
	{
		printf("Too many arguments for the stats command\n\rEnter help command for syntax of all commands\n\r");
		return;
	}
	print_stats();

}
/*********************************************************************************
 * @brief   :   Function to process the command received from the user
 *
 * 				This function processed the command entered by the user, splits
 * 				it into tokens and stores them in an array argv.
 * 				It then calls the appropriate function to handle that command
 *
 * @param   :   input  	- string entered by the user
 *
 * @return  :   void
*********************************************************************************/
void process_command(char *input)
{


    char *p = input;
    char *end;
    char saved[50]={0};
    strcpy(saved, input);
    /* Find end of string */
    for (end = input; *end != '\0'; end++);

    /* Put a \0 at the end of the string so that printf detects it */
    uint8_t saved_end = strlen(p);
    saved[saved_end-1] = 0;

    /* Tokenize input in place */
    bool in_token = false;
    char *argv[10];
    int argc = 0;
    memset(argv, 0, sizeof(argv));

    /* Run through the input and split into tokens */
    for (p = input; p < end; p++)
    {
        if(in_token)
        {
        	/* If we are inside a token */
            if(*p == ' ' || *p == '\t' || *p == '\r')
            {
            	/* If character is whitespace*/
                in_token = false;
                *p = '\0';
                argc++;
            }
        }
        else
        {
        	/* If we are outside a token */
            if(*p != ' ' && *p != '\t')
            {
            	/* If character is not whitespace*/
                in_token = true;
                argv[argc] = p;
            }
        }
    }

    /* No command */
    if (argc == 0)
    {
    	return;
    }

    uint8_t buffer[100] = {0};
    uint8_t decodedstring[100] = {0};

    /* Dispatch argc/argv to handler */
    bool valid_command = false;
    for (int i=0; i < num_commands; i++)
    {
    	if (strcasecmp(argv[0], commands[i].name) == 0)
    	{
    		valid_command = true;
    		commands[i].handler(argc, argv);
    		break;
    	}
    }
    if(!valid_command)
    {
    	/* Custom string received for encoding and decoding */
    	set_custom_string_true();

    	printf("%s", saved);
    }

}
