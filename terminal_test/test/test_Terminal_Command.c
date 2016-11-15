#include "unity.h"
#include "common.h"
#include "test_Terminal_Command.h"
#include "Terminal_Command.h"



void test_TerminalCommand_ParseTheBuffer_SetPortDefaultScenario() {
   TerminalCommand cmd;

   TerminalCommand_Initialise(&cmd);

   TEST_ASSERT_EQUAL_INT(0, cmd.port);
   TEST_ASSERT_EQUAL_INT(0, cmd.pin);
   TEST_ASSERT_EQUAL_INT(Terminal_Decimal_Type, cmd.dataType);
   TEST_ASSERT_EQUAL_STRING("", cmd.argument);

   Terminal_ReturnStates result;

   result = TerminalCommand_setPort( &cmd, 'A');

   TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
   TEST_ASSERT_EQUAL_INT('A', cmd.port);

}

void test_TerminalCommand_ParseTheBuffer_SetThePortWithBadValue() {
   TerminalCommand cmd;

   TerminalCommand_Initialise(&cmd);

   TEST_ASSERT_EQUAL_INT(0, cmd.port);
   TEST_ASSERT_EQUAL_INT(0, cmd.pin);
   TEST_ASSERT_EQUAL_INT(Terminal_Decimal_Type, cmd.dataType);
   TEST_ASSERT_EQUAL_STRING("", cmd.argument);

   Terminal_ReturnStates result;

   result = TerminalCommand_setPort( &cmd, 'X');

   TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_BadPort, result);
   TEST_ASSERT_EQUAL_INT(0, cmd.port);

}
