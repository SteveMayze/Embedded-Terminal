/////////////////////////////////////////////////////////////////////////
///	\file main.c
///	\brief This is the main program code.
///
///	\author: Ronald Sousa (Opticalworm)
/////////////////////////////////////////////////////////////////////////
#include "common.h"
#include "MCU/led.h"
#include "MCU/usart2.h"
#include "MCU/tick.h"


#define TEST_BLOCK_6


#ifdef TEST_BLOCK_1
/////////////////////////////////////////////////////////////////////////
///	\brief the first user code function to be called after the ARM M0
///	has initial.
/////////////////////////////////////////////////////////////////////////
void main(void)
{
    uint8_t tempData;
    uint32_t delayCount = 0;

    Led_Init();
    Tick_init();

    SerialPort2.Open(115200);

    for ( ;; )
    {
        if(SerialPort2.GetByte(&tempData))
        {
        	SerialPort2.SendByte(tempData);

        	// Application simulation
        	for(delayCount = 1000; delayCount; delayCount--);
    		Led_Toggle(); // Toggle each byte
        }
    }
}
#endif

#ifdef TEST_BLOCK_2
#include "LIST/fifo.h"
/////////////////////////////////////////////////////////////////////////
///	\brief the first user code function to be called after the ARM M0
///	has initial.
/////////////////////////////////////////////////////////////////////////
void main(void)
{
    FIFOQueue queue;
    FIFO.Initialize(&queue);

	uint8_t data = 10;
	uint8_t tempData;
    for ( ;; )
    {
    	while(FIFO.Insert(&queue, data++)){
    		;
    	}
    	while(FIFO.Remove(&queue, &tempData)){
    		data = tempData;
    	}
    }
}
#endif

#ifdef TEST_BLOCK_3
/////////////////////////////////////////////////////////////////////////
///	\brief the first user code function to be called after the ARM M0
///	has initial.
/////////////////////////////////////////////////////////////////////////
#include "LIST/fifo.h"
void main(void)
{
    uint8_t tempData, sendData;

    Led_Init();
    Tick_init();

    SerialPort2.Open(115200);

    FIFOQueue queue;
    FIFO.Initialize(&queue);

    for ( ;; )
    {
        if(SerialPort2.GetByte(&tempData))
        {
        	if ( FALSE == FIFO.Insert(&queue, tempData)) {
        		SerialPort2.SendString("Buffer is full\r\n");
        	}
        }
        if(FIFO.Remove(&queue, &sendData)) {
        	SerialPort2.SendByte(sendData);
    		Led_Toggle(); // Toggle each byte
        }
    }
}
#endif

#ifdef TEST_BLOCK_4
/////////////////////////////////////////////////////////////////////////
///	\brief the first user code function to be called after the ARM M0
///	has initial.
/////////////////////////////////////////////////////////////////////////
#include "LIST/fifo.h"

void main(void)
{
    uint8_t tempData, sendData;
    uint_fast8_t fifoReturnValue;

    Led_Init();
    Tick_init();

    SerialPort2.Open(115200);

    FIFOQueue queue;
    FIFO.Initialize(&queue);

    for ( ;; )
    {
        if(SerialPort2.GetByte(&tempData))
        {
        	if ( FALSE == FIFO.Insert(&queue, tempData)) {
        		// SerialPort2.SendString("Buffer is full\r\n");
        		for(;;){
            		fifoReturnValue = FIFO.Remove(&queue, &sendData);
            		if( FALSE == fifoReturnValue){
            			FIFO.Insert(&queue, tempData);
    					break;
            		}
            		else if ( TRUE == fifoReturnValue){
            			SerialPort2.SendByte(sendData);
                		Led_Toggle(); // Toggle each byte
            		}
        		}
        	}
        }
    }
}
#endif


#ifdef TEST_BLOCK_5
/////////////////////////////////////////////////////////////////////////
///	\brief the first user code function to be called after the ARM M0
///	has initial.
/////////////////////////////////////////////////////////////////////////
void main(void)
{
    uint8_t tempData;

    Led_Init();
    Tick_init();

    SerialPort2.Open(9600);

    for ( ;; )
    {
        while ( TRUE == SerialPort2.GetByte(&tempData))
        {
			SerialPort2.SendByte(tempData);
        }
        // Some code to run
        Tick_DelayMs(500);
		Led_Toggle(); // Toggle each byte
    }
}
#endif

#ifdef TEST_BLOCK_6
#include "TERMINAL/Terminal.h"
/////////////////////////////////////////////////////////////////////////
///	\brief The control loop for monitoring the USART and executing the
/// send commands
/////////////////////////////////////////////////////////////////////////

void main(void) {

    Led_Init();
    Tick_init();

    SerialPort2.Open(9600);

    TerminalBuffer cmdBuffer;
    TerminalCommand cmd;
    uint_fast8_t result;

    Terminal_Initialise(&cmdBuffer, &cmd);

    for ( ;; )
    {
    	result = Terminal_ReadSerialToCommandBuffer(&cmdBuffer);
    	switch( result ){
    	case Terminal_ReturnState_Pending:
    		// Do nothing. Wait for the next piece
    		break;
    	case Terminal_ReturnState_OK:
    		result = Terminal_ParseCommandBuffer(&cmdBuffer, &cmd);
    		if( Terminal_ReturnState_OK == result){
    			// TODO Execute the command
    			// TODO Send back acknowledgment
    		    TerminalCommand_Initialise(&cmd);
    		} else {
    			// TODO Send back error
    		}
    		break;
    	default:	// Error condition
    		// TODO Send back an error
    		break;
    	}
    }
}

#endif
