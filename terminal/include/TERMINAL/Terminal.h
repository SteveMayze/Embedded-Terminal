
#ifndef INCLUDE_TERMINAL_TERMINAL_H_
#define INCLUDE_TERMINAL_TERMINAL_H_

#include "Terminal_Buffer.h"


typedef enum Terminal_ReturnStates_Type {
	Terminal_ReturnState_OK,
	Terminal_ReturnState_NoMoreData,
	Terminal_ReturnState_IsWhiteSpace,
	Terminal_ReturnState_IsCarriageReturn,
	Terminal_ReturnState_SerialError,
	Terminal_ReturnState_TokenTooBig

} Terminal_ReturnStates;


#endif /* INCLUDE_TERMINAL_TERMINAL_H_ */
