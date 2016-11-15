
#include "TERMINAL/Terminal_Command.h"


///////////////////////////////////////////////////////////////////////
/// \brief Initialises the TerminalCommand structure so it can be used.
/// \param TerminalCommand *cmd - The pointer to the TerminalCommand to initialise.
///////////////////////////////////////////////////////////////////////
void TerminalCommand_Initialise( TerminalCommand *cmd){
	cmd->argument = "";
	cmd->dataType = Terminal_Decimal_Type;
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
Terminal_ReturnStates TerminalCommand_setPort( TerminalCommand *cmd, uint8_t port){
	Terminal_ReturnStates result;
	if (( port >= 'A' && port <= 'D') || port == 'F') {
		cmd->port = port;
		result = Terminal_ReturnState_OK;
	} else {
		result = Terminal_ReturnState_BadPort;
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
Terminal_ReturnStates TerminalCommand_setPin( TerminalCommand *cmd, uint8_t pin){
	return Terminal_ReturnState_OK;
}

///////////////////////////////////////////////////////////////////////
/// \brief Sets the data type of the value for the command.
/// \param TerminalCommand *cmd - The pointer to the TerminalCommand to initialise.
/// \param uint8_t
///
/// \return Terminal_ReturnState_OK
///	Terminal_ReturnState_BadType
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates TerminalCommand_setType( TerminalCommand *cmd, Terminal_DataType type) {
	return Terminal_ReturnState_OK;
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
Terminal_ReturnStates TerminalCommand_addValueElement( TerminalCommand *cmd, uint8_t ch){
	return Terminal_ReturnState_OK;
}
