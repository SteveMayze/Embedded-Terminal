/*
 * Terminal_Command.h
 *
 *  Created on: Nov 15, 2016
 *      Author: vagrant
 */

#ifndef INCLUDE_TERMINAL_TERMINAL_COMMAND_H_
#define INCLUDE_TERMINAL_TERMINAL_COMMAND_H_

#include "../common.h"

///////////////////////////////////////////////////////////////////////
/// \brief
///////////////////////////////////////////////////////////////////////
typedef enum Terminal_DataTypes {
	Terminal_Null,
	Terminal_Decimal_Type,
	Terminal_Unsigned_Type,
	Terminal_Binary_Type,
	Terminal_Hexadecimal_Type
} Terminal_DataType;


///////////////////////////////////////////////////////////////////////
/// \brief
///////////////////////////////////////////////////////////////////////
typedef struct {

	char port;
	uint8_t pin;
	Terminal_DataType dataType;
	char* argument;
	int8_t value;

} TerminalCommand;

///////////////////////////////////////////////////////////////////////
/// \brief Initialises the TerminalCommand structure so it can be used.
/// \param TerminalCommand *cmd - The pointer to the TerminalCommand to initialise.
///////////////////////////////////////////////////////////////////////
void TerminalCommand_Initialise(TerminalCommand *cmd);

///////////////////////////////////////////////////////////////////////
/// \brief Sets the PORT value on the command
/// \param TerminalCommand *cmd - The pointer to the TerminalCommand to initialise.
/// \param uint8_t - The char that designates the port ID
///
/// \return Terminal_ReturnState_OK
///	Terminal_ReturnState_BadPort
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates TerminalCommand_setPort(TerminalCommand *cmd,
		uint8_t port);

///////////////////////////////////////////////////////////////////////
/// \brief Sets the designated pin# to the pin field of the command.
/// \param TerminalCommand *cmd - The pointer to the TerminalCommand to initialise.
/// \param uint8_t
///
/// \return Terminal_ReturnState_OK
///	Terminal_ReturnState_BadPin
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates TerminalCommand_setPin(TerminalCommand *cmd, uint8_t pin);

///////////////////////////////////////////////////////////////////////
/// \brief Sets the data-type of the value for the command.
/// \param TerminalCommand *cmd - The pointer to the TerminalCommand to initialise.
/// \param uint8_t
///
/// \return Terminal_ReturnState_OK
///	Terminal_ReturnState_BadType
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates TerminalCommand_setType(TerminalCommand *cmd,
		Terminal_DataType type);

///////////////////////////////////////////////////////////////////////
/// \brief Adds an element to the value. The order of the digits to be
///	added is least significant digit first.
/// \param TerminalCommand *cmd - The pointer to the TerminalCommand to initialise.
/// \param uint8_t
///
/// \return Terminal_ReturnState_OK
///	Terminal_ReturnState_InvalidValue
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates TerminalCommand_addValueElement(TerminalCommand *cmd,
		uint8_t ch);

Terminal_ReturnStates TerminalCommand_SetValue(TerminalCommand *cmd, int8_t value);

#endif /* INCLUDE_TERMINAL_TERMINAL_COMMAND_H_ */
