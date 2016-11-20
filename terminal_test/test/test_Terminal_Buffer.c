#include "unity.h"
#include "fff.h"
#include "common.h"
#include "Terminal_Buffer.h"
// #include "Terminal_Command.h"
// #include "Terminal.h"
#include "cmsis_device.h"

// #include "mock_usart2.h"
#include "common.h"


FAKE_VALUE_FUNC( int_fast8_t, USART2_GetByte, uint_fast8_t*);
extern Terminal_ReturnStates TerminalBuffer_ReadFromSerialBuffer(uint8_t *byte);


/////////////////////////////////////////////////////////////////////
/// \brief Add ch to Token Test
/////////////////////////////////////////////////////////////////////
void test_AddChToToken_DefaultScenario(void){
  TerminalBuffer buffer;
  TerminalBuffer_Initialise(&buffer);

  TEST_ASSERT_EQUAL_INT(0, buffer.tokenId);
  TEST_ASSERT_EQUAL_INT(0, buffer.idx);
  TEST_ASSERT_EQUAL_INT(0, buffer.size);

  Terminal_ReturnStates result = TerminalBuffer_AddCharToToken( &buffer, 'A');

  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);

  TEST_ASSERT_EQUAL_INT(0, buffer.tokenId);
  TEST_ASSERT_EQUAL_INT(1, buffer.idx);
  TEST_ASSERT_EQUAL_INT('A', buffer.tokenBuffer[0][0]);
  TEST_ASSERT_EQUAL_INT(1, buffer.size);

  TerminalBuffer_AddCharToToken( &buffer, 'B');

  TEST_ASSERT_EQUAL_INT(0, buffer.tokenId);
  TEST_ASSERT_EQUAL_INT(2, buffer.idx);
  TEST_ASSERT_EQUAL_INT('B', buffer.tokenBuffer[0][1]);
  TEST_ASSERT_EQUAL_INT(1, buffer.size);

}



void test_AddChToToken_TokenTooBig(void) {
   TerminalBuffer buffer;
   TerminalBuffer_Initialise(&buffer);
   char *sample = "ABCDEFGHIJKL";
   Terminal_ReturnStates result;
   uint_fast8_t i;
   for(i = 0; i < 12; i++){
      result = TerminalBuffer_AddCharToToken( &buffer, sample[i]);
      if( i < TOKEN_SIZE -1) {
        TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
      }
      else {
        TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_TokenTooBig, result);
      }
   }
}




void test_WhenBufferInitialised_ThenItShouldBeClean(void) {

     TerminalBuffer cmdBuffer;
     TerminalBuffer_Initialise( &cmdBuffer );

     TEST_ASSERT_EQUAL_INT( 0, cmdBuffer.tokenId);
     TEST_ASSERT_EQUAL_INT( 0, cmdBuffer.idx);
     TEST_ASSERT_EQUAL_INT( 0, cmdBuffer.tokenBuffer[1][0]);
     TEST_ASSERT_EQUAL_INT( 0, cmdBuffer.tokenBuffer[0][0]);
     TEST_ASSERT_EQUAL_INT(0, cmdBuffer.size);

}

void test_whenNewTokenCreated_thenIncrTokenId(void) {

  TerminalBuffer cmdBuffer;
  TerminalBuffer_Initialise( &cmdBuffer);
  TerminalBuffer_AddCharToToken(&cmdBuffer, 'A');
  TEST_ASSERT_EQUAL_INT(1, cmdBuffer.size);

  Terminal_ReturnStates result = TerminalBuffer_NewToken( &cmdBuffer );

  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_INT(1, cmdBuffer.tokenId);
  TEST_ASSERT_EQUAL_INT(1, cmdBuffer.size);


}

void test_whenAnotherNewTokenCreated_thenThrowError(void) {

  TerminalBuffer cmdBuffer;
  TEST_ASSERT_EQUAL_INT(0, cmdBuffer.size);

  TerminalBuffer_Initialise( &cmdBuffer);
  TerminalBuffer_AddCharToToken(&cmdBuffer, 'A');
  TEST_ASSERT_EQUAL_INT(1, cmdBuffer.size);
  TEST_ASSERT_EQUAL_INT(1, cmdBuffer.size);
Terminal_ReturnStates result = TerminalBuffer_NewToken( &cmdBuffer );

  TerminalBuffer_AddCharToToken(&cmdBuffer, 'B');
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_INT(2, cmdBuffer.size);
  TEST_ASSERT_EQUAL_INT(1, cmdBuffer.tokenId);

  TerminalBuffer_AddCharToToken(&cmdBuffer, 'C');
  TEST_ASSERT_EQUAL_INT(2, cmdBuffer.size);

  result = TerminalBuffer_NewToken( &cmdBuffer );
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_TokenOverFlow, result);


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
  Terminal_ReturnStates result = TerminalBuffer_ReadFromSerialBuffer( &ch );

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
  Terminal_ReturnStates result = TerminalBuffer_ReadFromSerialBuffer( &ch );

  TEST_ASSERT_EQUAL_INT( Terminal_ReturnState_SerialError, result );

}


void test_ReadSerialToBuffer_DefaultScenario_NonWhiteSpace(void){
  uint8_t ch;

  int_fast8_t Delegate_USART2_GetByte( uint8_t *ch ) {
     *ch = 'A';
     return (int_fast8_t)Serial_ReturnState_OK;
  }

  USART2_GetByte_fake.custom_fake = Delegate_USART2_GetByte;
  Terminal_ReturnStates result = TerminalBuffer_ReadFromSerialBuffer( &ch );

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
  Terminal_ReturnStates result = TerminalBuffer_ReadFromSerialBuffer( &ch );

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
  Terminal_ReturnStates result = TerminalBuffer_ReadFromSerialBuffer( &ch );

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
  Terminal_ReturnStates result = TerminalBuffer_ReadFromSerialBuffer( &ch );

  TEST_ASSERT_EQUAL_INT( '\n', ch );
  TEST_ASSERT_TRUE( !isalpha(ch))
  TEST_ASSERT_EQUAL_INT( Terminal_ReturnState_IsCarriageReturn, result );

}
