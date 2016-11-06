#include "TERMINAL/Terminal.h"
#include "MCU/usart2.h"
#include "stdbool.h"


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
Terminal_ReturnStates Terminal_ReadFromSerialBuffer(uint8_t *byte) {

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
		result = Terminal_ReadFromSerialBuffer(&ch);
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
