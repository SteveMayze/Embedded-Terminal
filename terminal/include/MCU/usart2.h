////////////////////////////////////////////////////////////////////////////////
/// \file usart2.h
///	\author: Ronald Sousa (@Opticalworm)
////////////////////////////////////////////////////////////////////////////////

#ifndef __USART_TWO_MCU_H__
#define __USART_TWO_MCU_H__

	#include "common.h"
    #include "MCU/SerialStructure.h"

    int_fast8_t USART2_GetByte(uint8_t *destination);

    extern SerialInterface SerialPort2;

#endif
