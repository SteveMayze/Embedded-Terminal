#include "unity.h"
#include "common.h"
#include "test_Terminal_Command.h"
#include "Terminal_Command.h"



void test_TerminalCommand_ParseTheBuffer_SetPortDefaultScenario() {
   TerminalCommand cmd;

   TerminalCommand_Initialise(&cmd);

   TEST_ASSERT_EQUAL_INT(0, cmd.port);
   TEST_ASSERT_EQUAL_INT(0, cmd.pin);
   TEST_ASSERT_EQUAL_INT(Terminal_Null, cmd.dataType);
   TEST_ASSERT_EQUAL_STRING("", cmd.argument);

   Terminal_ReturnStates result;

   result = TerminalCommand_setPort( &cmd, 'A');

   TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
   TEST_ASSERT_EQUAL_INT('A', cmd.port);

}

void test_TerminalCommand_ParseTheBuffer_SetThePortWithInvalidValue() {
   TerminalCommand cmd;

   TerminalCommand_Initialise(&cmd);

   TEST_ASSERT_EQUAL_INT(0, cmd.port);
   TEST_ASSERT_EQUAL_INT(0, cmd.pin);
   TEST_ASSERT_EQUAL_INT(Terminal_Null, cmd.dataType);
   TEST_ASSERT_EQUAL_STRING("", cmd.argument);

   Terminal_ReturnStates result;

   result = TerminalCommand_setPort( &cmd, 'X');

   TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_InvalidPort, result);
   TEST_ASSERT_EQUAL_INT(0, cmd.port);

}

void test_TerminalCommand_ParseTheBuffer_SetPinDefaultScenario() {
   TerminalCommand cmd;

   TerminalCommand_Initialise(&cmd);

   TEST_ASSERT_EQUAL_INT(0, cmd.port);
   TEST_ASSERT_EQUAL_INT(0, cmd.pin);
   TEST_ASSERT_EQUAL_INT(Terminal_Null, cmd.dataType);
   TEST_ASSERT_EQUAL_STRING("", cmd.argument);

   Terminal_ReturnStates result;

   result = TerminalCommand_setPin( &cmd, 1);

   TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
   TEST_ASSERT_EQUAL_INT(1, cmd.pin);

}

void test_TerminalCommand_ParseTheBuffer_SetThePinWithInvalidValue() {
   TerminalCommand cmd;

   TerminalCommand_Initialise(&cmd);

   TEST_ASSERT_EQUAL_INT(0, cmd.port);
   TEST_ASSERT_EQUAL_INT(0, cmd.pin);
   TEST_ASSERT_EQUAL_INT(Terminal_Null, cmd.dataType);
   TEST_ASSERT_EQUAL_STRING("", cmd.argument);

   Terminal_ReturnStates result;

   result = TerminalCommand_setPin( &cmd, 25);

   TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_InvalidPin, result);
   TEST_ASSERT_EQUAL_INT(0, cmd.pin);

}



void test_TerminalCommand_ParseTheBuffer_SetTypeDefaultScenario() {
   TerminalCommand cmd;

   TerminalCommand_Initialise(&cmd);

   TEST_ASSERT_EQUAL_INT(0, cmd.port);
   TEST_ASSERT_EQUAL_INT(0, cmd.pin);
   TEST_ASSERT_EQUAL_INT(Terminal_Null, cmd.dataType);
   TEST_ASSERT_EQUAL_STRING("", cmd.argument);

   Terminal_ReturnStates result;

   result = TerminalCommand_setType( &cmd, Terminal_Binary_Type);

   TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
   TEST_ASSERT_EQUAL_INT(Terminal_Binary_Type, cmd.dataType);

}

void test_TerminalCommand_ParseTheBuffer_SetTheTypeWithInvalidValue() {
   TerminalCommand cmd;

   TerminalCommand_Initialise(&cmd);

   TEST_ASSERT_EQUAL_INT(0, cmd.port);
   TEST_ASSERT_EQUAL_INT(0, cmd.pin);
   TEST_ASSERT_EQUAL_INT(Terminal_Null, cmd.dataType);
   TEST_ASSERT_EQUAL_STRING("", cmd.argument);

   Terminal_ReturnStates result;

   result = TerminalCommand_setType( &cmd, 25);

   TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_InvalidType, result);
   TEST_ASSERT_EQUAL_INT(0, cmd.dataType);

}

void test_TerminalCommand_AddValueElementBeforeTypeWillReturnError(){
      TerminalCommand cmd;

      TerminalCommand_Initialise( &cmd );

      TEST_ASSERT_EQUAL_INT(0, cmd.port);
      TEST_ASSERT_EQUAL_INT(0, cmd.pin);
      TEST_ASSERT_EQUAL_INT(Terminal_Null, cmd.dataType);
      TEST_ASSERT_EQUAL_STRING("", cmd.argument);

      Terminal_ReturnStates result;

      result = TerminalCommand_addValueElement( &cmd, '1');

      TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_TypeNotSet, result);
      TEST_ASSERT_EQUAL_INT(0, cmd.dataType);

}

void test_TerminalCommand_AddValueElementDefaultPath(){
      TerminalCommand cmd;

      TerminalCommand_Initialise( &cmd );

      TEST_ASSERT_EQUAL_INT(0, cmd.port);
      TEST_ASSERT_EQUAL_INT(0, cmd.pin);
      TEST_ASSERT_EQUAL_INT(Terminal_Null, cmd.dataType);
      TEST_ASSERT_EQUAL_STRING("", cmd.argument);

      Terminal_ReturnStates result;

      result = TerminalCommand_setType(&cmd, Terminal_Decimal_Type);
      result = TerminalCommand_addValueElement( &cmd, '1');

      TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
      TEST_ASSERT_EQUAL_INT(Terminal_Decimal_Type, cmd.dataType);
      TEST_ASSERT_EQUAL_STRING("1", cmd.argument);

}

void test_TerminalCommand_AddValueElementDigitsAreAddedLSB(){
      TerminalCommand cmd;

      TerminalCommand_Initialise( &cmd );

      TEST_ASSERT_EQUAL_INT(0, cmd.port);
      TEST_ASSERT_EQUAL_INT(0, cmd.pin);
      TEST_ASSERT_EQUAL_INT(Terminal_Null, cmd.dataType);
      TEST_ASSERT_EQUAL_STRING("", cmd.argument);

      Terminal_ReturnStates result;

      result = TerminalCommand_setType(&cmd, Terminal_Decimal_Type);
      result = TerminalCommand_addValueElement( &cmd, '1');
      result = TerminalCommand_addValueElement( &cmd, '2');
      result = TerminalCommand_addValueElement( &cmd, '3');

      TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
      TEST_ASSERT_EQUAL_INT(Terminal_Decimal_Type, cmd.dataType);
      TEST_ASSERT_EQUAL_STRING("321", cmd.argument);

}



void test_TerminalCommand_AddValueElementBinaryCanOnlyBe1and0(){
      TerminalCommand cmd;

      TerminalCommand_Initialise( &cmd );

      TEST_ASSERT_EQUAL_INT(0, cmd.port);
      TEST_ASSERT_EQUAL_INT(0, cmd.pin);
      TEST_ASSERT_EQUAL_INT(Terminal_Null, cmd.dataType);
      TEST_ASSERT_EQUAL_STRING("", cmd.argument);

      Terminal_ReturnStates result;

      result = TerminalCommand_setType(&cmd, Terminal_Binary_Type);
      result = TerminalCommand_addValueElement( &cmd, '1');
      TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
      result = TerminalCommand_addValueElement( &cmd, '0');
      TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_OK, result);
      result = TerminalCommand_addValueElement( &cmd, '3');
      TEST_ASSERT_EQUAL_INT(Terminal_ReturnState_InvalidValue, result);

      TEST_ASSERT_EQUAL_INT(Terminal_Binary_Type, cmd.dataType);
      TEST_ASSERT_EQUAL_STRING("01", cmd.argument);

}
