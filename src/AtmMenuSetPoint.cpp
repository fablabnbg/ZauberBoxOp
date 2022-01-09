
#include "AtmMenuSetPoint.h"
#include "AtmMenu.h"
#include <LoggerNode.h>


 Atm_MenuSetPoint::Atm_MenuSetPoint(AtmMenu& mm):
 	 SubMachine(mm),
	 HomieNode("heatSet", "Solltemperatur", "setpoint"),
	 runOnce(false),
	 deciTemp(180)
 {
 }


 void Atm_MenuSetPoint::setup() {
	 advertise("setTemp").setRetained(false).setDatatype("number").setName("Solltemperatur").setUnit("°C").settable();
	 advertise("setTempCmd").setRetained(false).setDatatype("enum").setName("CMD Solltemperatur").setFormat("INCREMENT:DECREMENT:DEFAULT:READ");
	 begin();
 }

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_MenuSetPoint& Atm_MenuSetPoint::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                 ON_ENTER    ON_LOOP  ON_EXIT  EVT_BUTTON1  EVT_BUTTON2  EVT_BUTTON3  EVT_BUTTON4  EVT_TIMEOUT  EVT_START      ELSE */
    /*     IDLE */     ENT_IDLE, ATM_SLEEP,      -1,          -1,          -1,          -1,          -1,          -1,  SET_CTRL,       -1,
    /* SET_CTRL */ ENT_SET_CTRL,        -1,      -1,     SET_DEC,          -1,     SET_INC,        IDLE,        IDLE,        -1,       -1,
    /*  SET_INC */  ENT_SET_INC,        -1,      -1,          -1,          -1,          -1,          -1,          -1,        -1, SET_CTRL,
    /*  SET_DEC */  ENT_SET_DEC,        -1,      -1,          -1,          -1,          -1,          -1,          -1,        -1, SET_CTRL,
  };
  // clang-format on
  Machine::begin( state_table, ELSE );
  trace(Serial);
  return *this;
}

/* Add C++ code for each internally handled event (input)
 * The code must return 1 to trigger the event
 */

int Atm_MenuSetPoint::event( int id ) {
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
    case EVT_START:
      return 0;
  }
  return 0;
}

void Atm_MenuSetPoint::onReadyToOperate() {
	HomieNode::setProperty("setTempCmd").setRetained(false).send("READ");
}

bool Atm_MenuSetPoint::handleInput(const HomieRange &range,	const String &property, const String &value) {
	LN.logf("MenuSetPoint::handleInput", LoggerNode::DEBUG, "Incoming MQTT message for property %s with string \"%s\"\n", property.c_str(), value.c_str() );
	if (property.equals("setTemp")) {
		deciTemp = static_cast<int16_t>(value.toFloat() * 10);
		if (state() == SET_CTRL) mainMenu.showDeciInt(deciTemp);
		return true;
	}
	return false;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_SETCHANGE, 0, <v>, <up> );
 */

void Atm_MenuSetPoint::action( int id ) {
  switch ( id ) {
    case ENT_IDLE:
  	  if(runOnce) {
  		  exit_machine();
  	  } else {
  		  runOnce = true;
  	  }
      return;
    case ENT_SET_CTRL:
      mainMenu.resetTimeout();
      return;
    case ENT_SET_INC:
        push(connectors, ON_SETCHANGE, 0, 1, 1);
        HomieNode::setProperty("setTempCmd").setRetained(false).send("INCREMENT");
      return;
    case ENT_SET_DEC:
        push(connectors, ON_SETCHANGE, 0, 1, -1);
        HomieNode::setProperty("setTempCmd").setRetained(false).send("DECREMENT");
      return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_MenuSetPoint& Atm_MenuSetPoint::trigger( int event ) {
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_MenuSetPoint::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* Public event methods
 *
 */

Atm_MenuSetPoint& Atm_MenuSetPoint::start() {
  trigger( EVT_START );
  mainMenu.showIcon8x8(3, AtmMenu::sprite_heater);
  mainMenu.showDeciInt(deciTemp);
  return *this;
}

/*
 * onSetchange() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_MenuSetPoint& Atm_MenuSetPoint::onSetchange( Machine& machine, int event ) {
  onPush( connectors, ON_SETCHANGE, 0, 1, 1, machine, event );
  return *this;
}

Atm_MenuSetPoint& Atm_MenuSetPoint::onSetchange( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_SETCHANGE, 0, 1, 1, callback, idx );
  return *this;
}

Atm_MenuSetPoint& Atm_MenuSetPoint::onWhite(atm_cb_lambda_t callback, int idx) {
	onPush(connectors, ON_SETCHANGE, 0, 1, 1, callback, idx);
	return *this;
}

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

Atm_MenuSetPoint& Atm_MenuSetPoint::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "MENUSETPOINT\0EVT_BUTTON1\0EVT_BUTTON2\0EVT_BUTTON3\0EVT_BUTTON4\0EVT_TIMEOUT\0EVT_START\0ELSE\0IDLE\0SET_CTRL\0SET_INC\0SET_DEC" );
  return *this;
}

