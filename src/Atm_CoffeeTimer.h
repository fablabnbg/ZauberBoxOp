#pragma once

#include <Automaton.h>
#include <SubMachine.h>

class Atm_CoffeeTimer: public SubMachine {

 public:
  enum { IDLE_OFF, PREPARE, CLEAN_INIT, CLEAN_PRESS, CLEAN_OPEN, FLUSH_INIT, FLUSH_PRESS, FLUSH_OPEN, EMPTY, FINISH }; // STATES
  enum { EVT_START, EVT_CONTINUE, EVT_ABORT, EVT_COUNTER, EVT_TIMEOUT, ELSE }; // EVENTS
  Atm_CoffeeTimer( AtmMenu& mm ) : SubMachine(mm) {};
  Atm_CoffeeTimer& begin( void );
  Atm_CoffeeTimer& trace( Stream & stream );
  Atm_CoffeeTimer& trigger( int event );
  int state( void );

  Atm_CoffeeTimer& onStatechange( Machine& machine, int event = 0 );
  Atm_CoffeeTimer& onStatechange( atm_cb_push_t callback, int idx = 0 );
  Atm_CoffeeTimer& onStatechange( atm_cb_lambda_t callback, int idx = 0 );

 private:
  enum { ENT_IDLE_OFF, ENT_PREPARE, ENT_CLEAN_INIT, ENT_CLEAN_PRESS, LP_CLEAN_PRESS, ENT_CLEAN_OPEN, ENT_FLUSH_INIT, ENT_FLUSH_PRESS, LP_FLUSH_PRESS, ENT_FLUSH_OPEN, ENT_EMPTY, ENT_FINISH }; // ACTIONS
  enum { ON_STATECHANGE, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id ); 
  void action( int id ); 

  atm_timer_millis timer;
  atm_counter counter;

};
/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_CoffeeTimer">
    <states>
      <IDLE_OFF index="0" on_enter="ENT_IDLE_OFF">
        <EVT_START>PREPARE</EVT_START>
      </IDLE_OFF>
      <PREPARE index="1" on_enter="ENT_PREPARE">
        <EVT_CONTINUE>CLEAN_PRESS</EVT_CONTINUE>
        <EVT_ABORT>IDLE_OFF</EVT_ABORT>
        <EVT_COUNTER>CLEAN_PRESS</EVT_COUNTER>
      </PREPARE>
      <CLEAN_INIT index="2" on_enter="ENT_CLEAN_INIT">
        <ELSE>CLEAN_PRESS</ELSE>
      </CLEAN_INIT>
      <CLEAN_PRESS index="3" on_enter="ENT_CLEAN_PRESS" on_loop="LP_CLEAN_PRESS">
        <EVT_ABORT>IDLE_OFF</EVT_ABORT>
        <EVT_COUNTER>FLUSH_PRESS</EVT_COUNTER>
        <EVT_TIMEOUT>CLEAN_OPEN</EVT_TIMEOUT>
      </CLEAN_PRESS>
      <CLEAN_OPEN index="4" on_enter="ENT_CLEAN_OPEN">
        <EVT_ABORT>IDLE_OFF</EVT_ABORT>
        <EVT_TIMEOUT>CLEAN_PRESS</EVT_TIMEOUT>
      </CLEAN_OPEN>
      <FLUSH_INIT index="5" on_enter="ENT_FLUSH_INIT">
        <ELSE>FLUSH_PRESS</ELSE>
      </FLUSH_INIT>
      <FLUSH_PRESS index="6" on_enter="ENT_FLUSH_PRESS" on_loop="LP_FLUSH_PRESS">
        <EVT_ABORT>IDLE_OFF</EVT_ABORT>
        <EVT_COUNTER>EMPTY</EVT_COUNTER>
        <EVT_TIMEOUT>FLUSH_OPEN</EVT_TIMEOUT>
      </FLUSH_PRESS>
      <FLUSH_OPEN index="7" on_enter="ENT_FLUSH_OPEN">
        <EVT_ABORT>IDLE_OFF</EVT_ABORT>
        <EVT_TIMEOUT>FLUSH_PRESS</EVT_TIMEOUT>
      </FLUSH_OPEN>
      <EMPTY index="8" on_enter="ENT_EMPTY">
        <EVT_ABORT>IDLE_OFF</EVT_ABORT>
        <EVT_TIMEOUT>FINISH</EVT_TIMEOUT>
      </EMPTY>
      <FINISH index="9" on_enter="ENT_FINISH">
        <EVT_CONTINUE>CLEAN_PRESS</EVT_CONTINUE>
        <EVT_ABORT>IDLE_OFF</EVT_ABORT>
        <EVT_TIMEOUT>IDLE_OFF</EVT_TIMEOUT>
      </FINISH>
    </states>
    <events>
      <EVT_START index="0" access="PRIVATE"/>
      <EVT_CONTINUE index="1" access="PRIVATE"/>
      <EVT_ABORT index="2" access="PRIVATE"/>
      <EVT_COUNTER index="3" access="PRIVATE"/>
      <EVT_TIMEOUT index="4" access="PRIVATE"/>
    </events>
    <connectors>
      <STATECHANGE autostore="0" broadcast="0" dir="PUSH" slots="1"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/
