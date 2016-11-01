
#include "unity.h"
#include "test_Terminal_Buffer.h"
#include "Terminal_Buffer.h"

void test_WhenBufferInitialised_ThenItShouldBeClean(void) {

     TerminalBuffer cmdBuffer;
     Terminal_BufferInitialise( &cmdBuffer );

     TEST_ASSERT_EQUAL_INT( 0, cmdBuffer.tokenId);
     TEST_ASSERT_EQUAL_INT( 0, cmdBuffer.idx);
     TEST_ASSERT_EQUAL_INT( 0, cmdBuffer.tokenBuffer[0][0]);
     TEST_ASSERT_EQUAL_INT( 0, cmdBuffer.tokenBuffer[1][0]);

}
