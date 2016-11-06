
#ifndef INCLUDE_TERMINAL_TERMINAL_BUFFER_H_
#define INCLUDE_TERMINAL_TERMINAL_BUFFER_H_

#include "../common.h"

#define TERMINAL_COMMAND_PENDING 8

#define TOKEN_LIST_SIZE  2
#define TOKEN_SIZE 10


typedef struct {

	uint8_t tokenBuffer[TOKEN_LIST_SIZE][TOKEN_SIZE];
	uint8_t tokenId;
	uint8_t idx;
	uint8_t inToken;

} TerminalBuffer;

void Terminal_BufferInitialise(TerminalBuffer *cmdBuffer);

Terminal_ReturnStates TerminalBuffer_AddCharToToken(TerminalBuffer *buffer,
		uint8_t ch) ;

Terminal_ReturnStates TerminalBuffer_NewToken(TerminalBuffer *cmdBuffer);

void TerminalBuffer_endToken(TerminalBuffer *cmdBuffer);

#endif /* INCLUDE_TERMINAL_TERMINAL_BUFFER_H_ */
