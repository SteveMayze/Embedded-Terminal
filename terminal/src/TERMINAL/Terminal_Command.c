#include "TERMINAL/Terminal_Command.h"
#include <string.h>

const int Terminal_Command_DataTypes_E[] =
		{ Terminal_Decimal_Type, Terminal_Unsigned_Type, Terminal_Binary_Type,
				Terminal_Hexadecimal_Type };

///////////////////////////////////////////////////////////////////////
/// \brief Tests the type value to validate that it is in the range
/// of the enum for data types.
///////////////////////////////////////////////////////////////////////
static uint_fast8_t isDataType(Terminal_DataType type) {
	unsigned i;
	for (i = 0;i< (sizeof(Terminal_Command_DataTypes_E)/ sizeof(*Terminal_Command_DataTypes_E)); i++) {
		if (Terminal_Command_DataTypes_E[i] == type)
			return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////
/// \brief Initialises the TerminalCommand structure so it can be used.
/// \param TerminalCommand *cmd - The pointer to the TerminalCommand to initialise.
///////////////////////////////////////////////////////////////////////
void TerminalCommand_Initialise(TerminalCommand *cmd) {
	// Consider if argument needs to be freed before setting to NULL
	cmd->argument = "";
	cmd->dataType = Terminal_Null;
	cmd->port = 0;
	cmd->pin = 0;
}

///////////////////////////////////////////////////////////////////////
/// \brief Sets the PORT value on the command
/// \param TerminalCommand *cmd - The pointer to the TerminalCommand to initialise.
/// \param uint8_t - The char that designates the port ID
///
/// \return Terminal_ReturnState_OK
///	Terminal_ReturnState_BadPort
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates TerminalCommand_setPort(TerminalCommand *cmd,
		uint8_t port) {
	Terminal_ReturnStates result = Terminal_ReturnState_OK;
	if ((port >= 'A' && port <= 'D') || port == 'F') {
		cmd->port = port;
	} else {
		result = Terminal_ReturnState_InvalidPort;
	}
	return result;
}

///////////////////////////////////////////////////////////////////////
/// \brief Sets the designated pin# to the pin field of the command.
/// \param TerminalCommand *cmd - The pointer to the TerminalCommand to initialise.
/// \param uint8_t
///
/// \return Terminal_ReturnState_OK
///	Terminal_ReturnState_BadPin
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates TerminalCommand_setPin(TerminalCommand *cmd, uint8_t pin) {
	Terminal_ReturnStates result;
	if (pin < 16) {
		cmd->pin = pin;
		result = Terminal_ReturnState_OK;
	} else {
		result = Terminal_ReturnState_InvalidPin;
	}
	return result;
}

///////////////////////////////////////////////////////////////////////
/// \brief Sets the data type of the value for the command.
/// \param TerminalCommand *cmd - The pointer to the TerminalCommand to initialise.
/// \param uint8_t
///
/// \return Terminal_ReturnState_OK
///	Terminal_ReturnState_BadType
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates TerminalCommand_setType(TerminalCommand *cmd,
		Terminal_DataType type) {
	Terminal_ReturnStates result;
	if (isDataType(type)) {
		result = Terminal_ReturnState_OK;
		cmd->dataType = type;
	} else {
		return Terminal_ReturnState_InvalidType;
	}

	return result;
}

///////////////////////////////////////////////////////////////////////
/// \brief Adds an element to the value. The order of the digits to be
///	added is least significant digit first.
/// \param TerminalCommand *cmd - The pointer to the TerminalCommand to initialise.
/// \param uint8_t
///
/// \return Terminal_ReturnState_OK
///	Terminal_ReturnState_InvalidValue
///////////////////////////////////////////////////////////////////////
//Terminal_ReturnStates TerminalCommand_addValueElement(TerminalCommand *cmd,
//		uint8_t ch) {
//	Terminal_ReturnStates result;
//	char *c;
//	if (Terminal_Null == cmd->dataType) {
//		return Terminal_ReturnState_TypeNotSet;
//	}
//	if ( Terminal_Binary_Type == cmd->dataType ){
//		if( !(ch >= '0' && ch <= '1')){
//			return Terminal_ReturnState_InvalidValue;
//		}
//	}
//	result = Terminal_ReturnState_OK;
//	int clen;
//	clen = strlen(cmd->argument) + 1;
//	if (!(c = malloc(clen))) {
//		return Terminal_ReturnState_MemoryError;
//	}
//	memset(c, '\0', clen);
//	strcpy(c + 1, cmd->argument);// Copy the existing string down one position
//	*c = ch;	// Prepend the string with the new ch
//	if ((clen - 1) > 0) {
//		free(cmd->argument);
//	}
//	cmd->argument = c;
//
//	return result;
//
//}

Terminal_ReturnStates TerminalCommand_SetValue(TerminalCommand *cmd, int8_t value){
	cmd->value = value;
	return Terminal_ReturnState_OK;
}
