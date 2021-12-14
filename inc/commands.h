/**
 * @file    :   commands.h
 * @brief   :   An abstraction of all commands
 *
 *              This header file provides functions which are used to handle the
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

#ifndef COMMANDS_H_
#define COMMANDS_H_

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
void process_user_input(void);

/*********************************************************************************
 * @brief   :   Function to handle the author command
 *
 * @param   :   argc  	- Number of arguments
 * 				argv	- Array of arguments
 *
 * @return  :   void
*********************************************************************************/
void handle_author(int argc, char *argv[]);

/*********************************************************************************
 * @brief   :   Function to handle the help command
 *
 * @param   :   argc  	- Number of arguments
 * 				argv	- Array of arguments
 *
 * @return  :   void
*********************************************************************************/
void handle_help(int argc, char *argv[]);

/*********************************************************************************
 * @brief   :   Function to handle the reset command
 *
 * @param   :   argc  	- Number of arguments
 * 				argv	- Array of arguments
 *
 * @return  :   void
*********************************************************************************/
void handle_reset(int argc, char *argv[]);

/*********************************************************************************
 * @brief   :   Function to handle the stats command
 *
 * @param   :   argc  	- Number of arguments
 * 				argv	- Array of arguments
 *
 * @return  :   void
*********************************************************************************/
void handle_stats(int argc, char *argv[]);

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
void process_command(char *input);


#endif /* COMMANDS_H_ */
