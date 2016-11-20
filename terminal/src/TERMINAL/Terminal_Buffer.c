#include "TERMINAL/Terminal_Buffer.h"
#include "MCU/usart2.h"
#include "stdbool.h"
#include "ctype.h"


///////////////////////////////////////////////////////////////////////
/// \brief Intialises the TerminalBuffer to a clean, reset state.
///
/// \param cmdBuffer: TerminalBuffer
///
///////////////////////////////////////////////////////////////////////
void TerminalBuffer_Initialise(TerminalBuffer *cmdBuffer) {

	cmdBuffer->idx = 0;
	cmdBuffer->tokenId = 0;
	cmdBuffer->size = 0;
	int i, j;
	for (i = 0; i < TOKEN_LIST_SIZE; i++) {
		for (j = 0; j < TOKEN_SIZE; j++) {
			cmdBuffer->tokenBuffer[i][j] = 0;
		}
	}
	cmdBuffer->inToken = false;

}



///////////////////////////////////////////////////////////////////////
/// \brief Reads a character from the serial buffer
/// \param byte: uint8_t
///
/// \return Terminal_ReturnStates Terminal_ReturnState_OK on normal
/// execution. For special cases, other states are returned
/// Terminal_ReturnState_SerialError: For serial related errors
/// Terminal_ReturnState_NoMoreData: For no more data available in
/// the serial buffer.
/// Terminal_ReturnState_IsWhiteSpace: where white space is detected
/// other than carriage return
/// Terminal_ReturnState_IsCarriageReturn: When carriage return is detected.
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates TerminalBuffer_ReadFromSerialBuffer(uint8_t *byte) {

	int_fast8_t serial_result = USART2_GetByte(byte);
	Terminal_ReturnStates result = Terminal_ReturnState_OK;

	switch (serial_result) {
	case Serial_ReturnState_OK:
		if ('\n' == *byte) {
			result = Terminal_ReturnState_IsCarriageReturn;
		} else if (isspace(*byte)) {
			result = Terminal_ReturnState_IsWhiteSpace;
		}
		break;
	case Serial_RetrunState_PortNotOpenError:
	case Serial_ReturnState_InvalidPointer:
	case Serial_ReturnState_BufferOverflow:
		result = Terminal_ReturnState_SerialError;
		break;
	case Serial_ReturnState_BufferEmpty:
		result = Terminal_ReturnState_NoMoreData;
		break;
	default:
		break;
	}
	return result;
}

///////////////////////////////////////////////////////////////////////
/// \brief Adds a character to the next available position in the
/// current token.
/// \param buffer: TerminalBuffer
/// \param ch: uint8_t
///
/// \return Terminal_ReturnStates Terminal_ReturnState_OK on normal
/// execution. Otherwise Terminal_ReturnState_TokenTooBig when the
/// token buffer size has been exceeded.
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates TerminalBuffer_AddCharToToken(TerminalBuffer *cmdBuffer,
		uint8_t ch) {

	if( 0 == cmdBuffer->idx ){
	   cmdBuffer->size++;
	}

	Terminal_ReturnStates result = Terminal_ReturnState_OK;
	cmdBuffer->tokenBuffer[cmdBuffer->tokenId][cmdBuffer->idx++] = ch;
	if (cmdBuffer->idx > TOKEN_SIZE -1) {
		result = Terminal_ReturnState_TokenTooBig;
		cmdBuffer->tokenBuffer[cmdBuffer->tokenId][TOKEN_SIZE-1] = 0;
	}
	cmdBuffer->inToken = true;

	return result;

}


///////////////////////////////////////////////////////////////////////
/// \brief Sets up the TerminalBuffer to accept a new token.
/// \param buffer: TerminalBuffer
///
/// \return Terminal_ReturnStates Terminal_ReturnState_OK on normal
/// execution. Otherwise Terminal_ReturnState_TokenOverFlow when the
/// token array has been exceeded.
///////////////////////////////////////////////////////////////////////
Terminal_ReturnStates TerminalBuffer_NewToken(TerminalBuffer *cmdBuffer) {

	if (cmdBuffer->tokenId >= TOKEN_LIST_SIZE - 1) {
		return Terminal_ReturnState_TokenOverFlow;
	}
	if (cmdBuffer->idx > 0) {
		cmdBuffer->tokenId++;
		cmdBuffer->idx = 0;
	}
	cmdBuffer->inToken = true;
	return Terminal_ReturnState_OK;
}

void TerminalBuffer_endToken(TerminalBuffer *cmdBuffer) {

	cmdBuffer->inToken = false;

}
