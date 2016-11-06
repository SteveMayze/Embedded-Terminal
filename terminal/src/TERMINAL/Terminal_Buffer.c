#include "TERMINAL/Terminal_Buffer.h"
#include "stdbool.h"


///////////////////////////////////////////////////////////////////////
/// \brief Intialises the TerminalBuffer to a clean, reset state.
///
/// \param cmdBuffer: TerminalBuffer
///
///////////////////////////////////////////////////////////////////////
void TerminalBuffer_Initialise(TerminalBuffer *cmdBuffer) {

	cmdBuffer->idx = 0;
	cmdBuffer->tokenId = 0;
	int i, j;
	for (i = 0; i < TOKEN_LIST_SIZE; i++) {
		for (j = 0; j < TOKEN_SIZE; j++) {
			cmdBuffer->tokenBuffer[i][j] = 0;
		}
	}
	cmdBuffer->inToken = false;

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
Terminal_ReturnStates TerminalBuffer_AddCharToToken(TerminalBuffer *buffer,
		uint8_t ch) {

	Terminal_ReturnStates result = Terminal_ReturnState_OK;
	buffer->tokenBuffer[buffer->tokenId][buffer->idx++] = ch;
	if (buffer->idx > TOKEN_SIZE -1) {
		result = Terminal_ReturnState_TokenTooBig;
		buffer->tokenBuffer[buffer->tokenId][TOKEN_SIZE-1] = 0;
	}
	buffer->inToken = true;

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
