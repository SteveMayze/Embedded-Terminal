#include "unity.h"
#include "fff.h"
#include "common.h"
#include "Terminal.h"
#include "Terminal_Buffer.h"
#include "cmsis_device.h"

// #include "mock_usart2.h"
#include "common.h"

FAKE_VALUE_FUNC( int_fast8_t, USART2_GetByte, uint_fast8_t*);
extern Terminal_ReturnStates Terminal_ReadFromSerialBuffer(uint8_t *byte);

void test_ReadSerialToBuffer_DefaultScenario_NonWhiteSpace(void){
  uint8_t ch;

  int_fast8_t Delegate_USART2_GetByte( uint8_t *ch ) {
     *ch = 'A';
     return (int_fast8_t)Serial_ReturnState_OK;
  }

  USART2_GetByte_fake.custom_fake = Delegate_USART2_GetByte;
  Terminal_ReturnStates result = Terminal_ReadFromSerialBuffer( &ch );

  TEST_ASSERT_EQUAL_INT( 'A', ch );
  TEST_ASSERT_EQUAL_INT( Terminal_ReturnState_OK, result );

}


void test_ReadSerialToBuffer_WhiteSpaceReturned(void){
  uint8_t ch;

  int_fast8_t Delegate_USART2_GetByte( uint8_t *ch ) {
     *ch = ' ';
     return (int_fast8_t)Serial_ReturnState_OK;
  }

  USART2_GetByte_fake.custom_fake = Delegate_USART2_GetByte;
  Terminal_ReturnStates result = Terminal_ReadFromSerialBuffer( &ch );

  TEST_ASSERT_EQUAL_INT( ' ', ch );
  TEST_ASSERT_TRUE( isspace( ch ));
  TEST_ASSERT_EQUAL_INT( Terminal_ReturnState_IsWhiteSpace, result );

}

void test_ReadSerialToBuffer_TabReturnedAsWhiteSpace(void){
  uint8_t ch;

  int_fast8_t Delegate_USART2_GetByte( uint8_t *ch ) {
     *ch = '\t';
     return (int_fast8_t)Serial_ReturnState_OK;
  }

  USART2_GetByte_fake.custom_fake = Delegate_USART2_GetByte;
  Terminal_ReturnStates result = Terminal_ReadFromSerialBuffer( &ch );

  TEST_ASSERT_EQUAL_INT( '\t', ch );
  TEST_ASSERT_TRUE( isspace( ch ));
  TEST_ASSERT_EQUAL_INT( Terminal_ReturnState_IsWhiteSpace, result );

}

void test_ReadSerialToBuffer_CarriageReturn(void){
  uint8_t ch;

  int_fast8_t Delegate_USART2_GetByte( uint8_t *ch ) {
     *ch = '\n';
     return (int_fast8_t)Serial_ReturnState_OK;
  }

  USART2_GetByte_fake.custom_fake = Delegate_USART2_GetByte;
  Terminal_ReturnStates result = Terminal_ReadFromSerialBuffer( &ch );

  TEST_ASSERT_EQUAL_INT( '\n', ch );
  TEST_ASSERT_TRUE( !isalpha(ch))
  TEST_ASSERT_EQUAL_INT( Terminal_ReturnState_IsCarriageReturn, result );

}

/////////////////////////////////////////////////////////////////////
/// \brief ReadSerialToBuffer Test - No More Data
/////////////////////////////////////////////////////////////////////
void test_ReadSerialToBuffer_NoMoreData(void){
  uint8_t ch;

  int_fast8_t Delegate_USART2_GetByte( uint8_t *ch ) {
     return (int_fast8_t)Serial_ReturnState_BufferEmpty;
  }

  USART2_GetByte_fake.custom_fake = Delegate_USART2_GetByte;
  Terminal_ReturnStates result = Terminal_ReadFromSerialBuffer( &ch );

  TEST_ASSERT_EQUAL_INT( Terminal_ReturnState_NoMoreData, result );

}

/////////////////////////////////////////////////////////////////////
/// \brief ReadSerialToBuffer Test - Serial Error
/////////////////////////////////////////////////////////////////////
void test_ReadSerialToBuffer_SerialError(void){
  uint8_t ch;

  int_fast8_t Delegate_USART2_GetByte( uint8_t *ch ) {
     return (int_fast8_t)Serial_RetrunState_PortNotOpenError;
  }

  USART2_GetByte_fake.custom_fake = Delegate_USART2_GetByte;
  Terminal_ReturnStates result = Terminal_ReadFromSerialBuffer( &ch );

  TEST_ASSERT_EQUAL_INT( Terminal_ReturnState_SerialError, result );

}

extern Terminal_ReturnStates Terminal_ReadSerialToCommandBuffer(TerminalBuffer *buffer);


void test_WhenWhiteSpaceIsEncountered_thenNewToken(){

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

  TEST_ASSERT_EQUAL_INT(1, buffer.tokenId);
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_TokenTooBig, result);
  TEST_ASSERT_EQUAL_STRING("ABC", buffer.tokenBuffer[0]);
  TEST_ASSERT_EQUAL_STRING("DEFGHIJKL", buffer.tokenBuffer[1]);

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
