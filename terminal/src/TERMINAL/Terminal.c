#include "common.h"
#include "TERMINAL/Terminal.h"
#include "MCU/usart2.h"
#include "stdbool.h"
#include "ctype.h"
#include "stdlib.h"
#include "string.h"

#define LEXICAL_PORT (uint_fast8_t)0
#define LEXICAL_PIN (uint_fast8_t)1
#define LEXICAL_TYPE (uint_fast8_t)2
#define LEXICAL_VALUE (uint_fast8_t)3
#define LEXICAL_END (uint_fast8_t)4

///////////////////////////////////////////////////////////////////////
/// \brief Initialises the command buffer and command structures
/// \param byte: uint8_t
///
///////////////////////////////////////////////////////////////////////
void Terminal_Initialise(TerminalBuffer *cmdBuffer, TerminalCommand *cmd) {
	TerminalBuffer_Initialise(cmdBuffer);
	TerminalCommand_Initialise(cmd);
}

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
Terminal_ReturnStates Terminal_ReadSerialToCommandBuffer(TerminalBuffer *buffer) {
	Terminal_ReturnStates result = Terminal_ReturnState_OK;

	uint8_t ch;
	bool moreData = true;

	do {
		result = TerminalBuffer_ReadFromSerialBuffer(&ch);
		switch (result) {
		case Terminal_ReturnState_SerialError:
			break;
		case Terminal_ReturnState_NoMoreData:
			result = Terminal_ReturnState_Pending;
			moreData = false;
			break;
		case Terminal_ReturnState_IsCarriageReturn:
			result = Terminal_ReturnState_OK;
			moreData = false;
			break;
		case Terminal_ReturnState_IsWhiteSpace:
			TerminalBuffer_endToken(buffer);
			break;
		default:
			if ( true != buffer->inToken) {
				result = TerminalBuffer_NewToken(buffer);
			}

			if (result != Terminal_ReturnState_OK) {
				moreData = false;
			} else {
				result = TerminalBuffer_AddCharToToken(buffer, ch);
				if (result != Terminal_ReturnState_OK) {
					moreData = false;
				}
			}
			break;
		}
	} while (moreData);

	return result;
}

///////////////////////////////////////////////////////////////////////
/// \brief Parses the OPERATION token to determine the PORT and PIN assignment
/// \param char* operation - The String token to be parsed
/// \param TerminalCommand *cmd - The Command structure that will take the PORT and PIN assignments
///
/// \return Terminal_ReturnState_OK, Terminal_ReturnState_InvalidPort,
///         Terminal_ReturnState_InvalidPin
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates Terminal_ParseOperation(uint8_t *operation,
		TerminalCommand *cmd) {
	Terminal_ReturnStates result = Terminal_ReturnState_OK;

	uint_fast8_t lexicalState = LEXICAL_PORT;
	uint_fast8_t tokenSize = sizeof(operation);
	uint_fast8_t i;
	uint8_t pInt = 0;
	for (i = 0; i < tokenSize; i++) {
		switch (lexicalState) {
		case LEXICAL_PORT :
			result = TerminalCommand_setPort(cmd, operation[i]);
			lexicalState++;
			break;
		case LEXICAL_PIN :
			if (operation[i] >= '0' && operation[i] <= '9') {
				pInt = strtol((char*) operation + i, NULL, 10);
				lexicalState = LEXICAL_END;
			}
			result = TerminalCommand_setPin(cmd, pInt);
			break;
		default:
			break;
		}
		if (Terminal_ReturnState_OK != result) {
			break;
		}
	}

	return result;
}

uint_fast8_t isType(uint8_t ch) {
	uint_fast8_t result = Terminal_ReturnState_OK;
	char *pattern = "0123456789bBuUxX";
	int i;
	for (i = 0; i < 16; i++) {
		if (pattern[i] == ch) {
			return result;
		}
	}
	return Terminal_ReturnState_InvalidType;
}

uint_fast8_t validValue(int8_t *value, char *span) {
	uint_fast8_t result = Terminal_ReturnState_OK;
	size_t dcount = strlen( (char*)value );
	size_t spansize = strspn((char*)value, span);
	if (dcount != spansize) {
		result = Terminal_ReturnState_InvalidValue;
	}
	return result;
}

///////////////////////////////////////////////////////////////////////
/// \brief Parses the ARGUMENT token to determine the data type and to
///        extract the value.
/// \param  char *argument
/// \param TerminalCommand *cmd
///
/// \return Terminal_ReturnState_OK, Terminal_ReturnState_InvalidType,
///         Terminal_ReturnState_TypeNotSet, Terminal_ReturnState_InvalidValue,
///         Terminal_ReturnState_MemoryError
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates Terminal_ParseArgument(uint8_t *argument,
		TerminalCommand *cmd) {
	Terminal_ReturnStates result = Terminal_ReturnState_OK;

	uint_fast8_t lexicalState = LEXICAL_TYPE;
	uint_fast8_t tokenSize = strlen((char*)argument);
	uint_fast8_t i;
	int8_t pValue = 0;
	int8_t base = 10;
	char *span = "1234567890";
	for (i = 0; i < tokenSize; i++) {
		switch (lexicalState) {
		case LEXICAL_TYPE :
			result = isType(argument[i]);
			if (!(Terminal_ReturnState_OK == result)) {
				return result;
			}

			if ('u' == argument[i] || 'U' == argument[i]) {
				result = TerminalCommand_setType(cmd, Terminal_Unsigned_Type);
			} else if ('x' == argument[i] || 'X' == argument[i]) {
				span = "0123456789ABCDEF";
				result = TerminalCommand_setType(cmd,
						Terminal_Hexadecimal_Type);
				base = 16;
			} else if ('b' == argument[i] || 'B' == argument[i]) {
				span = "01";
				result = TerminalCommand_setType(cmd, Terminal_Binary_Type);
				base = 2;
			} else {
				// result = TerminalCommand_addValueElement( argument[i]);
				char *span = "1234567890";
				if (Terminal_ReturnState_OK == validValue( (argument + 1), span)) {
					pValue = strtol((char*) argument + i, NULL, base);
					result = TerminalCommand_SetValue(cmd, pValue);
					lexicalState = LEXICAL_END;
				} else {
					return Terminal_ReturnState_InvalidValue;
				}
			}
			lexicalState++;
			break;
		case LEXICAL_VALUE :
			if (Terminal_ReturnState_OK == validValue(argument + 1, span)) {
				pValue = strtol((char*) argument + i, NULL, base);
				result = TerminalCommand_SetValue(cmd, pValue);
				lexicalState = LEXICAL_END;
			} else {
				return Terminal_ReturnState_InvalidValue;
			}
			break;
		default:
			break;
		}
		if (Terminal_ReturnState_OK != result) {
			break;
		}
	}

	return result;
}

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
Terminal_ReturnStates Terminal_ParseCommandBuffer(TerminalBuffer *cmdBuffer,
		TerminalCommand *cmd) {

	Terminal_ReturnStates result = Terminal_ReturnState_OK;
	int tokenId;
	for (tokenId = 0; tokenId < cmdBuffer->size; tokenId++) {
		switch (tokenId) {
		case 0:
			result = Terminal_ParseOperation(cmdBuffer->tokenBuffer[0], cmd);
			break;
		case 1:
			result = Terminal_ParseArgument(cmdBuffer->tokenBuffer[1], cmd);
			break;
		default:
			result = Terminal_ReturnState_TokenOverFlow;
		}
		if (Terminal_ReturnState_OK != result) {
			break;
		}
	}
	return result;
}
