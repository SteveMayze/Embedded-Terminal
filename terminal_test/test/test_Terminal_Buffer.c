
#include "unity.h"
#include "common.h"
#include "test_Terminal_Buffer.h"
#include "Terminal_Buffer.h"




/////////////////////////////////////////////////////////////////////
/// \brief Add ch to Token Test
/////////////////////////////////////////////////////////////////////
void test_AddChToToken_DefaultScenario(void){
  TerminalBuffer buffer;
  TerminalBuffer_Initialise(&buffer);

  TEST_ASSERT_EQUAL_INT(0, buffer.tokenId);
  TEST_ASSERT_EQUAL_INT(0, buffer.idx);

  Terminal_ReturnStates result = TerminalBuffer_AddCharToToken( &buffer, 'A');

  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);

  TEST_ASSERT_EQUAL_INT(0, buffer.tokenId);
  TEST_ASSERT_EQUAL_INT(1, buffer.idx);
  TEST_ASSERT_EQUAL_INT('A', buffer.tokenBuffer[0][0]);

  TerminalBuffer_AddCharToToken( &buffer, 'B');

  TEST_ASSERT_EQUAL_INT(0, buffer.tokenId);
  TEST_ASSERT_EQUAL_INT(2, buffer.idx);
  TEST_ASSERT_EQUAL_INT('B', buffer.tokenBuffer[0][1]);

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

}

void test_whenNewTokenCreated_thenIncrTokenId(void) {

  TerminalBuffer buffer;
  TerminalBuffer_Initialise( &buffer);
  TerminalBuffer_AddCharToToken(&buffer, 'A');
  Terminal_ReturnStates result = TerminalBuffer_NewToken( &buffer );

  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_INT(1, buffer.tokenId);


}

void test_whenAnotherNewTokenCreated_thenThrowError(void) {

  TerminalBuffer buffer;
  TerminalBuffer_Initialise( &buffer);

  TerminalBuffer_Initialise( &buffer);
  TerminalBuffer_AddCharToToken(&buffer, 'A');
  Terminal_ReturnStates result = TerminalBuffer_NewToken( &buffer );

  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
  TEST_ASSERT_EQUAL_INT(1, buffer.tokenId);

  TerminalBuffer_AddCharToToken(&buffer, 'B');

  result = TerminalBuffer_NewToken( &buffer );
  TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_TokenOverFlow, result);


}
