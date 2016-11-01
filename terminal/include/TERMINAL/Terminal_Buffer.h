
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

} TerminalBuffer;

void Terminal_BufferInitialise(TerminalBuffer *cmdBuffer);

#endif /* INCLUDE_TERMINAL_TERMINAL_BUFFER_H_ */
