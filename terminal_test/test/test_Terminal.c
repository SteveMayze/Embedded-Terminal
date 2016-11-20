#include "unity.h"
#include "fff.h"
#include "common.h"
#include "Terminal_Buffer.h"
#include "Terminal_Command.h"
#include "Terminal.h"
#include "cmsis_device.h"
#include "stdlib.h"

// #include "mock_usart2.h"
#include "common.h"

FAKE_VALUE_FUNC( int_fast8_t, USART2_GetByte, uint_fast8_t*);

int pidx = 0;
char *psample;

int_fast8_t PDelegate_USART2_GetByte( uint8_t *ch ) {
  int_fast8_t result = (int_fast8_t) Serial_ReturnState_OK;
  if( psample[pidx] != 0) {
    *ch = psample[pidx++];
  } else {
    result = (int_fast8_t)Serial_ReturnState_BufferEmpty;
  }
  return result;
}




void test_WhenWhiteSpaceIsEncountered_thenNewToken(){

  TerminalBuffer cmdBuffer;
  TerminalBuffer_Initialise( &cmdBuffer );

  pidx = 0;
  psample = "   ABC   XYZ";

  // int_fast8_t Delegate_USART2_GetByte( uint8_t *ch ) {
  //   int_fast8_t result = (int_fast8_t) Serial_ReturnState_OK;
  //   if( sample[idx] != 0) {
  //     *ch = sample[idx++];
  //   } else {
  //     result = (int_fast8_t)Serial_ReturnState_BufferEmpty;
  //   }
  //   return result;
  // }
  USART2_GetByte_fake.custom_fake = PDelegate_USART2_GetByte;

  Terminal_ReturnStates result = Terminal_ReadSerialToCommandBuffer(&cmdBuffer);

  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_Pending, result);
  TEST_ASSERT_EQUAL_INT(1, cmdBuffer.tokenId);
  TEST_ASSERT_EQUAL_STRING("ABC", cmdBuffer.tokenBuffer[0]);
  TEST_ASSERT_EQUAL_STRING("XYZ", cmdBuffer.tokenBuffer[1]);
  TEST_ASSERT_EQUAL_INT(2, cmdBuffer.size);

}


void test_WhenReadingTokens_WhitespaceIsIgnored(){

  TerminalBuffer buffer;
  TerminalBuffer_Initialise( &buffer );

  int idx = 0;
  char *sample = "   ABC   XYZ";

  int_fast8_t Delegate_USART2_GetByte( uint8_t *ch ) {
    int_fast8_t result = (int_fast8_t) Serial_ReturnState_OK;
    if( sample[idx] != 0) {
      *ch = sample[idx++];
    } else {
      result = (int_fast8_t)Serial_ReturnState_BufferEmpty;
    }
    return result;
  }
  USART2_GetByte_fake.custom_fake = Delegate_USART2_GetByte;

  Terminal_ReturnStates result = Terminal_ReadSerialToCommandBuffer(&buffer);

  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_Pending, result);
  TEST_ASSERT_EQUAL_INT(1, buffer.tokenId);
  TEST_ASSERT_EQUAL_STRING("ABC", buffer.tokenBuffer[0]);
  TEST_ASSERT_EQUAL_STRING("XYZ", buffer.tokenBuffer[1]);

}



void test_WhenReadingTokens_WhitespaceIsIgnored2(){

  TerminalBuffer buffer;
  TerminalBuffer_Initialise( &buffer );

  int idx = 0;
  char *sample = "  \t ABC  \t XYZ   ";

  int_fast8_t Delegate_USART2_GetByte( uint8_t *ch ) {
    int_fast8_t result = (int_fast8_t) Serial_ReturnState_OK;
    if( sample[idx] != 0) {
      *ch = sample[idx++];
    } else {
      result = (int_fast8_t)Serial_ReturnState_BufferEmpty;
    }
    return result;
  }
  USART2_GetByte_fake.custom_fake = Delegate_USART2_GetByte;

  Terminal_ReturnStates result = Terminal_ReadSerialToCommandBuffer(&buffer);

  TEST_ASSERT_EQUAL_INT(1, buffer.tokenId);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_Pending, result);
  TEST_ASSERT_EQUAL_STRING("ABC", buffer.tokenBuffer[0]);
  TEST_ASSERT_EQUAL_STRING("XYZ", buffer.tokenBuffer[1]);

}


void test_WhenReadingTokens_LargeTokensWillThrowAnError(){

  TerminalBuffer buffer;
  TerminalBuffer_Initialise( &buffer );

  int idx = 0;
  char *sample = "ABC  \t DEFGHIJKLMN ";

  int_fast8_t Delegate_USART2_GetByte( uint8_t *ch ) {
    int_fast8_t result = (int_fast8_t) Serial_ReturnState_OK;
    if( sample[idx] != 0) {
      *ch = sample[idx++];
    } else {
      result = (int_fast8_t)Serial_ReturnState_BufferEmpty;
    }
    return result;
  }
  USART2_GetByte_fake.custom_fake = Delegate_USART2_GetByte;

  Terminal_ReturnStates result = Terminal_ReadSerialToCommandBuffer(&buffer);

  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_TokenTooBig, result);
  TEST_ASSERT_EQUAL_INT(1, buffer.tokenId);
  TEST_ASSERT_EQUAL_STRING("ABC", buffer.tokenBuffer[0]);
  TEST_ASSERT_EQUAL_STRING("DEFGHIJKL", buffer.tokenBuffer[1]);

}



void test_WhenReadingTokens_MoreTokensThrowsError(){

  TerminalBuffer buffer;
  TerminalBuffer_Initialise( &buffer );

  int idx = 0;
  char *sample = "ABC  \t XYZ   GHI ";

  int_fast8_t Delegate_USART2_GetByte( uint8_t *ch ) {
    int_fast8_t result = (int_fast8_t) Serial_ReturnState_OK;
    if( sample[idx] != 0) {
      *ch = sample[idx++];
    } else {
      result = (int_fast8_t)Serial_ReturnState_BufferEmpty;
    }
    return result;
  }
  USART2_GetByte_fake.custom_fake = Delegate_USART2_GetByte;

  Terminal_ReturnStates result = Terminal_ReadSerialToCommandBuffer(&buffer);

  TEST_ASSERT_EQUAL_INT(1, buffer.tokenId);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_TokenOverFlow, result);
  TEST_ASSERT_EQUAL_STRING("ABC", buffer.tokenBuffer[0]);
  TEST_ASSERT_EQUAL_STRING("XYZ", buffer.tokenBuffer[1]);

}

void test_WhenReadingTokens_CRWillCompleteTheCommand(){

  TerminalBuffer buffer;
  TerminalBuffer_Initialise( &buffer );

  int idx = 0;
  char *sample = "  \t ABC  \t XYZ  \n ";

  int_fast8_t Delegate_USART2_GetByte( uint8_t *ch ) {
    int_fast8_t result = (int_fast8_t) Serial_ReturnState_OK;
    if( sample[idx] != 0) {
      *ch = sample[idx++];
    } else {
      result = (int_fast8_t)Serial_ReturnState_BufferEmpty;
    }
    return result;
  }
  USART2_GetByte_fake.custom_fake = Delegate_USART2_GetByte;

  Terminal_ReturnStates result = Terminal_ReadSerialToCommandBuffer(&buffer);

  TEST_ASSERT_EQUAL_INT(1, buffer.tokenId);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_STRING("ABC", buffer.tokenBuffer[0]);
  TEST_ASSERT_EQUAL_STRING("XYZ", buffer.tokenBuffer[1]);

}

void test_TerminalInitialise_WillClearBuffers() {
  TerminalBuffer cmdBuffer;
  TerminalCommand cmd;
  Terminal_Initialise( &cmdBuffer, &cmd );

  TEST_ASSERT_EQUAL_INT( 0, cmdBuffer.tokenId);
  TEST_ASSERT_EQUAL_INT( 0, cmdBuffer.idx);
  TEST_ASSERT_EQUAL_INT( 0, cmdBuffer.tokenBuffer[1][0]);
  TEST_ASSERT_EQUAL_INT( 0, cmdBuffer.tokenBuffer[0][0]);
  TEST_ASSERT_EQUAL_INT(0, cmdBuffer.size);

  TEST_ASSERT_EQUAL_INT( 0, cmd.port);
  TEST_ASSERT_EQUAL_INT( 0, cmd.pin);
  TEST_ASSERT_EQUAL_INT( 0, cmd.dataType);
  TEST_ASSERT_EQUAL_STRING("", cmd.argument);


}

extern Terminal_ReturnStates Terminal_ParseOperation(uint8_t *operation, TerminalCommand *cmd);

void test_DefaultParseOperation() {
  TerminalBuffer cmdBuffer;
  TerminalCommand cmd;
  Terminal_Initialise( &cmdBuffer, &cmd );

  pidx = 0;
  psample =  "A2 U23\n";

  USART2_GetByte_fake.custom_fake = PDelegate_USART2_GetByte;

  Terminal_ReturnStates result= Terminal_ReadSerialToCommandBuffer(&cmdBuffer);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  result = Terminal_ParseOperation(cmdBuffer.tokenBuffer[0], &cmd);

  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_INT('A', cmd.port);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void test_DefaultValidCommandAndArgWillPopulateCommand() {
  TerminalBuffer cmdBuffer;
  TerminalCommand cmd;
  Terminal_Initialise( &cmdBuffer, &cmd );

  pidx = 0;
  psample =  "B2 U23\n";

  USART2_GetByte_fake.custom_fake = PDelegate_USART2_GetByte;

  Terminal_ReturnStates result= Terminal_ReadSerialToCommandBuffer(&cmdBuffer);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_INT(2, cmdBuffer.size);

  result = Terminal_ParseCommandBuffer(&cmdBuffer, &cmd);


  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_INT('B', cmd.port);
  TEST_ASSERT_EQUAL_INT(2, cmd.pin);
  TEST_ASSERT_EQUAL_INT(23, cmd.value);

}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void test_ParseAOperationWithAPINLargerThan9() {
  TerminalBuffer cmdBuffer;
  TerminalCommand cmd;
  Terminal_Initialise( &cmdBuffer, &cmd );

  pidx = 0;
  psample =  "A10 U23\n";

  USART2_GetByte_fake.custom_fake = PDelegate_USART2_GetByte;

  Terminal_ReturnStates result= Terminal_ReadSerialToCommandBuffer(&cmdBuffer);

  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_INT(2, cmdBuffer.size);

  result = Terminal_ParseCommandBuffer(&cmdBuffer, &cmd);

  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_INT('A', cmd.port);
  TEST_ASSERT_EQUAL_INT(10, cmd.pin);

}

void test_InvalidDataTypeWillReturnError() {
  TerminalBuffer cmdBuffer;
  TerminalCommand cmd;
  Terminal_Initialise( &cmdBuffer, &cmd );

  pidx = 0;
  psample =  "A2 F23\n";

  USART2_GetByte_fake.custom_fake = PDelegate_USART2_GetByte;

  Terminal_ReturnStates result= Terminal_ReadSerialToCommandBuffer(&cmdBuffer);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);

  result = Terminal_ParseCommandBuffer(&cmdBuffer, &cmd);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_InvalidType, result);

}

void test_ValueTypeOfHexIsOK() {
  TerminalBuffer cmdBuffer;
  TerminalCommand cmd;
  Terminal_Initialise( &cmdBuffer, &cmd );

  pidx = 0;
  psample =  "A2 xF0\n";

  USART2_GetByte_fake.custom_fake = PDelegate_USART2_GetByte;

  Terminal_ReturnStates result= Terminal_ReadSerialToCommandBuffer(&cmdBuffer);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);

  result = Terminal_ParseCommandBuffer(&cmdBuffer, &cmd);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_INT(0xF0, (uint8_t)cmd.value);
  TEST_ASSERT_EQUAL_INT(-16, cmd.value);
  TEST_ASSERT_EQUAL_INT('A', cmd.port);
  TEST_ASSERT_EQUAL_INT(Terminal_Hexadecimal_Type, cmd.dataType);

}

void test_ValueTypeOfBinaryIsOK() {
  TerminalBuffer cmdBuffer;
  TerminalCommand cmd;
  Terminal_Initialise( &cmdBuffer, &cmd );

  pidx = 0;
  psample =  "A2 b1010\n";

  USART2_GetByte_fake.custom_fake = PDelegate_USART2_GetByte;

  Terminal_ReturnStates result= Terminal_ReadSerialToCommandBuffer(&cmdBuffer);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);

  result = Terminal_ParseCommandBuffer(&cmdBuffer, &cmd);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_INT(0b1010, (uint8_t)cmd.value);
  TEST_ASSERT_EQUAL_INT(10, cmd.value);
  TEST_ASSERT_EQUAL_INT('A', cmd.port);
  TEST_ASSERT_EQUAL_INT(Terminal_Binary_Type, cmd.dataType);

}

void test_ValueTypeOfDecimalIsOK() {
  TerminalBuffer cmdBuffer;
  TerminalCommand cmd;
  Terminal_Initialise( &cmdBuffer, &cmd );

  pidx = 0;
  psample =  "B2 25\n";

  USART2_GetByte_fake.custom_fake = PDelegate_USART2_GetByte;

  Terminal_ReturnStates result= Terminal_ReadSerialToCommandBuffer(&cmdBuffer);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);

  result = Terminal_ParseCommandBuffer(&cmdBuffer, &cmd);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_INT(25, cmd.value);
  TEST_ASSERT_EQUAL_INT('B', cmd.port);

}


void test_LeadingZerosAreIgnored() {
  TerminalBuffer cmdBuffer;
  TerminalCommand cmd;
  Terminal_Initialise( &cmdBuffer, &cmd );

  pidx = 0;
  psample =  "A2 0025\n";

  USART2_GetByte_fake.custom_fake = PDelegate_USART2_GetByte;

  Terminal_ReturnStates result= Terminal_ReadSerialToCommandBuffer(&cmdBuffer);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);

  result = Terminal_ParseCommandBuffer(&cmdBuffer, &cmd);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_INT(25, cmd.value);
  TEST_ASSERT_EQUAL_INT('A', cmd.port);

}

void test_InvalidValueForValueType_ShouldReturnError() {
  TerminalBuffer cmdBuffer;
  TerminalCommand cmd;
  Terminal_Initialise( &cmdBuffer, &cmd );

  pidx = 0;
  psample =  "A2 b135\n";

  USART2_GetByte_fake.custom_fake = PDelegate_USART2_GetByte;

  Terminal_ReturnStates result= Terminal_ReadSerialToCommandBuffer(&cmdBuffer);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);

  result = Terminal_ParseCommandBuffer(&cmdBuffer, &cmd);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_InvalidValue, result);
  TEST_ASSERT_EQUAL_INT(0, cmd.value);
}



void test_MoreThanTwoTokens_ReturnError() {
  TerminalBuffer cmdBuffer;
  TerminalCommand cmd;
  Terminal_Initialise( &cmdBuffer, &cmd );

  pidx = 0;
  psample =  "A2 0025 ABC\n";

  USART2_GetByte_fake.custom_fake = PDelegate_USART2_GetByte;

  Terminal_ReturnStates result= Terminal_ReadSerialToCommandBuffer(&cmdBuffer);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_TokenOverFlow, result);
}

void test_InvalidPortAssignment_ReturnError() {
  TerminalBuffer cmdBuffer;
  TerminalCommand cmd;
  Terminal_Initialise( &cmdBuffer, &cmd );

  pidx = 0;
  psample =  "J3 0025 \n";

  USART2_GetByte_fake.custom_fake = PDelegate_USART2_GetByte;

  Terminal_ReturnStates result= Terminal_ReadSerialToCommandBuffer(&cmdBuffer);
  result = Terminal_ParseCommandBuffer(&cmdBuffer, &cmd);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_InvalidPort, result);


}
