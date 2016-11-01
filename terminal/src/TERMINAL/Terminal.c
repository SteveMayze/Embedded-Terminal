

#include "TERMINAL/Terminal.h"
#include "MCU/usart2.h"

Terminal_ReturnStates Terminal_ReadFromSerialBuffer(uint8_t *byte) {

	int_fast8_t serial_result = USART2_GetByte( byte );
	Terminal_ReturnStates result = Terminal_ReturnState_OK;

	switch(serial_result) {
	case Serial_ReturnState_OK:
		if( '\n' == *byte ){
			result = Terminal_ReturnState_IsCarriageReturn;
		} else if ( isspace( *byte )) {
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


Terminal_ReturnStates Terminal_AddCharToToken(TerminalBuffer *buffer, uint8_t ch) {

	Terminal_ReturnStates result = Terminal_ReturnState_OK;
	buffer->tokenBuffer[buffer->tokenId][buffer->idx++] = ch;
	if(buffer->idx >= TOKEN_SIZE){
		result = Terminal_ReturnState_TokenTooBig;
	}

	return result;

}
