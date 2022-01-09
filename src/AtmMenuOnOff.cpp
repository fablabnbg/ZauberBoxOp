/*
 * AtmMenuOnOff.cpp
 *
 *  Created on: 08.01.2022
 *      Author: ian
 */

#include <AtmMenuOnOff.h>
#include "AtmMenu.h"


/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_MenuOnOff& Atm_MenuOnOff::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                       ON_ENTER    ON_LOOP  ON_EXIT  EVT_BUTTON1  EVT_BUTTON2  EVT_BUTTON3  EVT_BUTTON4  EVT_TIMEOUT  EVT_START  ELSE */
    /*        IDLE */        ENT_IDLE, ATM_SLEEP,      -1,          -1,          -1,          -1,          -1,          -1,      CTRL,   -1,
    /*        CTRL */        ENT_CTRL,        -1,      -1,          -1, CTRL_TOGGLE,          -1,        IDLE,        IDLE,        -1,   -1,
    /* CTRL_TOGGLE */ ENT_CTRL_TOGGLE,        -1,      -1,          -1,          -1,          -1,          -1,          -1,        -1, CTRL,
  };
  // clang-format on
  SubMachine::begin( state_table, ELSE );
  trace(Serial);
  return *this;
}

/* Add C++ code for each internally handled event (input)
 * The code must return 1 to trigger the event
 */

int Atm_MenuOnOff::event( int id ) {
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
 *   push( connectors, ON_CTRL, 0, <v>, <up> );
 */

void Atm_MenuOnOff::action( int id ) {
  switch ( id ) {
    case ENT_IDLE:
  	  if(runOnce) {
  		  exit_machine();
  	  } else {
  		  runOnce = true;
  	  }
      return;
    case ENT_CTRL:
      mainMenu.resetTimeout();
      return;
    case ENT_CTRL_TOGGLE:
      push(connectors, ON_CTRL, 0, 1, 1);
      return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_MenuOnOff& Atm_MenuOnOff::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_MenuOnOff::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_MenuOnOff& Atm_MenuOnOff::start() {
  trigger( EVT_START );
  mainMenu.showIcon8x8(3, AtmMenu::sprite_coffee);
  mainMenu.showIcon8x8(1, AtmMenu::sprite_onoff);
  mainMenu.showIcon8x8(0, sprite_clean);
  return *this;
}

/*
 * onCtrl() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_MenuOnOff& Atm_MenuOnOff::onCtrl( Machine& machine, int event ) {
  onPush( connectors, ON_CTRL, 0, 1, 1, machine, event );
  return *this;
}

Atm_MenuOnOff& Atm_MenuOnOff::onCtrl( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_CTRL, 0, 1, 1, callback, idx );
  return *this;
}


Atm_MenuOnOff& Atm_MenuOnOff::onCtrl(atm_cb_lambda_t callback, int idx) {
	onPush(connectors, ON_CTRL, 0, 1, 1, callback, idx);
	return *this;
}



/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_MenuOnOff& Atm_MenuOnOff::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "MENUONOFF\0EVT_BUTTON1\0EVT_BUTTON2\0EVT_BUTTON3\0EVT_BUTTON4\0EVT_TIMEOUT\0EVT_START\0ELSE\0IDLE\0CTRL\0CTRL_TOGGLE" );
  return *this;
}


const unsigned char Atm_MenuOnOff::sprite_clean[] = { 0x00, 0x70, 0x8c, 0xa2, 0x82, 0x8c, 0x70, 0x00, 0};

