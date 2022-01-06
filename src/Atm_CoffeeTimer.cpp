#include "Atm_CoffeeTimer.h"

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */

Atm_CoffeeTimer& Atm_CoffeeTimer::begin() {
  // clang-format off
  const static state_t state_table[] PROGMEM = {
    /*                       ON_ENTER         ON_LOOP  ON_EXIT  EVT_START  EVT_CONTINUE  EVT_ABORT  EVT_COUNTER  EVT_TIMEOUT         ELSE */
    /*    IDLE_OFF */    ENT_IDLE_OFF,             -1,      -1,   PREPARE,           -1,        -1,          -1,          -1,          -1,
    /*     PREPARE */     ENT_PREPARE,             -1,      -1,        -1,   CLEAN_INIT,  IDLE_OFF, CLEAN_PRESS,          -1,          -1,
    /*  CLEAN_INIT */  ENT_CLEAN_INIT,             -1,      -1,        -1,           -1,        -1,          -1,          -1, CLEAN_PRESS,
    /* CLEAN_PRESS */ ENT_CLEAN_PRESS, LP_CLEAN_PRESS,      -1,        -1,           -1,  IDLE_OFF,  FLUSH_INIT,  CLEAN_OPEN,          -1,
    /*  CLEAN_OPEN */  ENT_CLEAN_OPEN,             -1,      -1,        -1,           -1,  IDLE_OFF,          -1, CLEAN_PRESS,          -1,
    /*  FLUSH_INIT */  ENT_FLUSH_INIT,             -1,      -1,        -1,           -1,        -1,          -1,          -1, FLUSH_PRESS,
    /* FLUSH_PRESS */ ENT_FLUSH_PRESS, LP_FLUSH_PRESS,      -1,        -1,           -1,  IDLE_OFF,       EMPTY,  FLUSH_OPEN,          -1,
    /*  FLUSH_OPEN */  ENT_FLUSH_OPEN,             -1,      -1,        -1,           -1,  IDLE_OFF,          -1, FLUSH_PRESS,          -1,
    /*       EMPTY */       ENT_EMPTY,             -1,      -1,        -1,           -1,  IDLE_OFF,          -1,      FINISH,          -1,
    /*      FINISH */      ENT_FINISH,             -1,      -1,        -1,     IDLE_OFF,  IDLE_OFF,          -1,    IDLE_OFF,          -1,
  };
  // clang-format on
  timer.set(ATM_TIMER_OFF);
  counter.set(ATM_COUNTER_OFF);
  Machine::begin( state_table, ELSE );
  return *this;          
}

/* Add C++ code for each internally handled event (input) 
 * The code must return 1 to trigger the event
 */

int Atm_CoffeeTimer::event( int id ) {
  switch ( id ) {
    case EVT_START:
      return 0;
    case EVT_CONTINUE:
      return 0;
    case EVT_ABORT:
      return 0;
    case EVT_COUNTER:
      return counter.expired();
    case EVT_TIMEOUT:
      return timer.expired(this);
  }
  return 0;
}

/* Add C++ code for each action
 * This generates the 'output' for the state machine
 *
 * Available connectors:
 *   push( connectors, ON_STATECHANGE, 0, <v>, <up> );
 */

void Atm_CoffeeTimer::action( int id ) {
  switch ( id ) {
    case ENT_IDLE_OFF:
    	  timer.set(ATM_TIMER_OFF);
    	  counter.set(ATM_COUNTER_OFF);
    	  return;
    case ENT_PREPARE:
		timer.set(ATM_TIMER_OFF);
		counter.set(ATM_COUNTER_OFF);
		push( connectors, ON_STATECHANGE, 0, state(), 0);
		return;
    case ENT_CLEAN_INIT:
    	counter.set(5);
    	Serial.printf("Counter set to %d\n", counter.value);
       return;
     case ENT_CLEAN_PRESS:
    	 timer.set(10000);
    	 counter.decrement(); //TODO: Move to Exit
    	 Serial.printf("Counter dec to %d\n", counter.value);
       return;
     case LP_CLEAN_PRESS:
    	 //TODO: Animation
       return;
     case ENT_CLEAN_OPEN:
    	 timer.set(1000);
       return;
     case ENT_FLUSH_INIT:
    	 counter.set(10);
       return;
     case ENT_FLUSH_PRESS:
    	 timer.set(5000);
    	 counter.decrement();	//TODO: Move to exit
       return;
     case LP_FLUSH_PRESS:
    	 //TODO: Animation
       return;
     case ENT_FLUSH_OPEN:
    	 timer.set(1000);
       return;
     case ENT_EMPTY:
    	 timer.set(15000);
       return;
     case ENT_FINISH:
    	 timer.set(5000);
    	 SubMachine::exit_machine();		// notify connected // TODO: This belongs to exit of last state (or enter of IDLE state - however ENT_IDLE_OFF crashes at startup as the Super Machine is not completely initialized yet)
       return;
  }
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

Atm_CoffeeTimer& Atm_CoffeeTimer::trigger( int event ) {
  Serial.printf("Trigger CT %x\n", event);
  Machine::trigger( event );
  return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int Atm_CoffeeTimer::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line                          
 ************************************************************************************************
*/

/*
 * onStatechange() push connector variants ( slots 1, autostore 0, broadcast 0 )
 */

Atm_CoffeeTimer& Atm_CoffeeTimer::onStatechange( Machine& machine, int event ) {
  onPush( connectors, ON_STATECHANGE, 0, 1, 1, machine, event );
  return *this;
}

Atm_CoffeeTimer& Atm_CoffeeTimer::onStatechange( atm_cb_push_t callback, int idx ) {
  onPush( connectors, ON_STATECHANGE, 0, 1, 1, callback, idx );
  return *this;
}

Atm_CoffeeTimer& Atm_CoffeeTimer::onStatechange(atm_cb_lambda_t callback, int idx) {
	onPush(connectors, ON_STATECHANGE, 0, 1, 1, callback, idx);
	return *this;
}


/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */
Atm_CoffeeTimer& Atm_CoffeeTimer::trace( Stream & stream ) {
  Machine::setTrace( &stream, atm_serial_debug::trace,
    "COFFEETIMER\0EVT_START\0EVT_CONTINUE\0EVT_ABORT\0EVT_COUNTER\0EVT_TIMEOUT\0ELSE\0IDLE_OFF\0PREPARE\0CLEAN_INIT\0CLEAN_PRESS\0CLEAN_OPEN\0FLUSH_INIT\0FLUSH_PRESS\0FLUSH_OPEN\0EMPTY\0FINISH" );
  return *this;
}



