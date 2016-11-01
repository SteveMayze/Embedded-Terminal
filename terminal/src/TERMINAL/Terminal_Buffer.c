
#include "TERMINAL/Terminal_Buffer.h"


void Terminal_BufferInitialise(TerminalBuffer *cmdBuffer) {

	cmdBuffer->idx = 0;
	cmdBuffer->tokenId = 0;
	cmdBuffer->tokenBuffer[0][0] = 0;
	cmdBuffer->tokenBuffer[1][0] = 0;


}
