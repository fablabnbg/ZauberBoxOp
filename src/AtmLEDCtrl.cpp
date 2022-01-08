#include "AtmLEDCtrl.h"
#include "AtmMenu.h"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_LEDCtrl& Atm_LEDCtrl::begin() {
  // clang-format off
	  const static state_t state_table[] PROGMEM = {
	    /*                         ON_ENTER    ON_LOOP         ON_EXIT  EVT_BUTTON1   EVT_BUTTON2  EVT_BUTTON3  EVT_BUTTON4  EVT_TIMEOUT   EVT_START        ELSE */
	    /*         IDLE */         ENT_IDLE, ATM_SLEEP,             -1,          -1,           -1,          -1,          -1,          -1, WHITE_CTRL,         -1,
	    /*   WHITE_CTRL */   ENT_WHITE_CTRL,        -1, EXT_WHITE_CTRL,   WHITE_DEC, WHITE_TOGGLE,   WHITE_INC,        IDLE,        IDLE,         -1,         -1,
	    /*    WHITE_INC */    ENT_WHITE_INC,        -1,             -1,          -1,           -1,          -1,          -1,          -1,         -1, WHITE_CTRL,
	    /*    WHITE_DEC */    ENT_WHITE_DEC,        -1,             -1,          -1,           -1,          -1,          -1,          -1,         -1, WHITE_CTRL,
	    /* WHITE_TOGGLE */ ENT_WHITE_TOGGLE,        -1,             -1,          -1,           -1,          -1,          -1,          -1,         -1, WHITE_CTRL,
	  };
  // clang-format on
  SubMachine::begin( state_table, ELSE );
  trace(Serial);
  return *this;
}

/* Add C++ code for each internally handled event (input)
 * The code must return 1 to trigger the event
 */

int Atm_LEDCtrl::event( int id ) {
  switch ( id ) {
    case EVT_BUTTON1:
      return 0;
    case EVT_BUTTON2:
      return 0;
    case EVT_BUTTON3:
      return 0;
    case EVT_BUTTON4:
      return 0;
    case EVT_TIMEOUT:
      return 0;
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_WHITE, 0, <v>, <up> );
 */

void Atm_LEDCtrl::action( int id ) {
  switch ( id ) {
    case ENT_IDLE:
	  if(runOnce) {
		  exit_machine();
	  } else {
		  runOnce = true;
	  }
      return;
    case ENT_WHITE_CTRL:
      return;
    case EXT_WHITE_CTRL:
      return;
    case ENT_WHITE_INC:
    	push(connectors, ON_WHITE, 0, 1, 1);
      return;
    case ENT_WHITE_DEC:
    	push(connectors, ON_WHITE, 0, 1, -1);
      return;
    case ENT_WHITE_TOGGLE:
    	push(connectors, ON_WHITE, 0, 1, 0);
      return;
  }
}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* Public event methods
 *
 */

SubMachine& Atm_LEDCtrl::start() {
  trigger( EVT_START );
  mainMenu.showIcon8x8(3, AtmMenu::sprite_light_below);
  mainMenu.showIcon8x8(1, AtmMenu::sprite_onoff);
  return *this;
}

/*
 * onWhite() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_LEDCtrl& Atm_LEDCtrl::onWhite( Machine& machine, int event ) {
  onPush( connectors, ON_WHITE, 0, 1, 1, machine, event );
  return *this;
}

Atm_LEDCtrl& Atm_LEDCtrl::onWhite( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_WHITE, 0, 1, 1, callback, idx );
  return *this;
}

Atm_LEDCtrl& Atm_LEDCtrl::onWhite(atm_cb_lambda_t callback, int idx) {
	onPush(connectors, ON_WHITE, 0, 1, 1, callback, idx);
	return *this;
}


/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_LEDCtrl& Atm_LEDCtrl::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "LEDCTRL\0EVT_BUTTON1\0EVT_BUTTON2\0EVT_BUTTON3\0EVT_BUTTON4\0EVT_TIMEOUT\0EVT_START\0ELSE\0IDLE\0WHITE_CTRL\0WHITE_INC\0WHITE_DEC\0WHITE_TOGGLE" );
  return *this;
}


const uint8_t Atm_LEDCtrl::sprite_up[9] = { 0x00,0x90,0x48,0x24,0x12,0x24,0x48,0x90,0};
const uint8_t Atm_LEDCtrl::sprite_dn[9] = { 0x12,0x24,0x48,0x90,0x48,0x24,0x12,0x00, 0};
