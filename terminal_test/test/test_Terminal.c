#include "unity.h"
#include "fff.h"
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

extern Terminal_ReturnStates Terminal_AddCharToToken(TerminalBuffer *buffer, uint8_t ch);

/////////////////////////////////////////////////////////////////////
/// \brief Add ch to Token Test
/////////////////////////////////////////////////////////////////////
void test_AddChToToken_DefaultScenario(void){
  TerminalBuffer buffer;
  Terminal_BufferInitialise(&buffer);

  TEST_ASSERT_EQUAL_INT(0, buffer.tokenId);
  TEST_ASSERT_EQUAL_INT(0, buffer.idx);

  Terminal_ReturnStates result = Terminal_AddCharToToken( &buffer, 'A');

  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);

  TEST_ASSERT_EQUAL_INT(0, buffer.tokenId);
  TEST_ASSERT_EQUAL_INT(1, buffer.idx);
  TEST_ASSERT_EQUAL_INT('A', buffer.tokenBuffer[0][0]);

  Terminal_AddCharToToken( &buffer, 'B');

  TEST_ASSERT_EQUAL_INT(0, buffer.tokenId);
  TEST_ASSERT_EQUAL_INT(2, buffer.idx);
  TEST_ASSERT_EQUAL_INT('B', buffer.tokenBuffer[0][1]);

}


void test_AddChToToken_TokenTooBig(void) {
   TerminalBuffer buffer;

   uint8_t sample[TOKEN_LIST_SIZE][TOKEN_SIZE]  = {65,66,67,68,69,70,71,72,73,74};

   uint_fast8_t i;
   for(i = 0; i < TOKEN_SIZE; i++){
     buffer.tokenBuffer[0][i] = sample[0][i];
   }
   buffer.tokenId = 0;
   buffer.idx = 9;

   Terminal_ReturnStates result = Terminal_AddCharToToken( &buffer, 'J');
   TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_TokenTooBig, result);

}
