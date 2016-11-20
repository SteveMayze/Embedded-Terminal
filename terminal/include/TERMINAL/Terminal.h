
#ifndef INCLUDE_TERMINAL_TERMINAL_H_
#define INCLUDE_TERMINAL_TERMINAL_H_

#include "Terminal_Buffer.h"
#include "Terminal_Command.h"


void Terminal_Initialise(TerminalBuffer *cmdBuffer, TerminalCommand *cmd);

///////////////////////////////////////////////////////////////////////
/// \brief Reads the content of the serial buffer to the TerminalBuffer
/// where it is collated into a set of tokens. The return state will
/// determine the validity of the data being collected.
/// \return Terminal_ReturnStates
/// Terminal_ReturnState_SerialError: A problem in the USART layer
/// Terminal_ReturnState_Pending: Serial buffer is empty but the command is not complete
/// Terminal_ReturnState_OK: Token collection is complete and valid
/// Terminal_ReturnState_TokenTooBig: The token size has been exceeded
/// Terminal_ReturnState_TokenOverFlow: Syntax error - too many tokens provided.
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates Terminal_ReadSerialToCommandBuffer(TerminalBuffer *buffer);

///////////////////////////////////////////////////////////////////////
/// \brief Parse the command buffer structure to populate the actual
///        command structure with the information to be able to execute
///        the operation using the argument.
/// \param TerminalBuffer *buffer
/// \param TerminalCommand *cmd
///
/// \return Terminal_ReturnState_OK
///         Terminal_ReturnState_InvalidPort,
///         Terminal_ReturnState_InvalidPin
///         Terminal_ReturnState_InvalidType,
///         Terminal_ReturnState_TypeNotSet, Terminal_ReturnState_InvalidValue,
///         Terminal_ReturnState_MemoryError
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates Terminal_ParseCommandBuffer(TerminalBuffer *buffer, TerminalCommand *cmd);

#endif /* INCLUDE_TERMINAL_TERMINAL_H_ */
