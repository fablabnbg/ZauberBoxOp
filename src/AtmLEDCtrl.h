#pragma once

#include <Automaton.h>
#include <SubMachine.h>


class Atm_LEDCtrl: public SubMachine {

 public:
  enum { IDLE, WHITE_CTRL, WHITE_INC, WHITE_DEC, WHITE_TOGGLE }; // STATES
  enum { EVT_BUTTON1, EVT_BUTTON2, EVT_BUTTON3, EVT_BUTTON4, EVT_TIMEOUT, EVT_START, ELSE }; // EVENTS
  Atm_LEDCtrl( AtmMenu& mm ) : SubMachine(mm), runOnce(false) {};
  virtual ~Atm_LEDCtrl() {};

  Atm_LEDCtrl& begin( void );
  Atm_LEDCtrl& trace( Stream & stream );
//  Atm_LEDCtrl& trigger( int event );
//  int state( void );
  Atm_LEDCtrl& onWhite( Machine& machine, int event = 0 );
  Atm_LEDCtrl& onWhite( atm_cb_push_t callback, int idx = 0 );
  Atm_LEDCtrl& onWhite( atm_cb_lambda_t callback, int idx = 0 );

  SubMachine& start( void ) override;

 private:
  enum { ENT_IDLE, ENT_WHITE_CTRL, EXT_WHITE_CTRL, ENT_WHITE_INC, ENT_WHITE_DEC, ENT_WHITE_TOGGLE }; // ACTIONS
  enum { ON_WHITE, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id );
  void action( int id );

  bool runOnce;

  static const uint8_t sprite_up[9];
  static const uint8_t sprite_dn[9];

};


/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_LEDCtrl">
    <states>
      <IDLE index="0" sleep="1" on_enter="ENT_IDLE">
        <EVT_START>WHITE_CTRL</EVT_START>
      </IDLE>
      <WHITE_CTRL index="1" on_enter="ENT_WHITE_CTRL" on_exit="EXT_WHITE_CTRL">
        <EVT_BUTTON1>WHITE_DEC</EVT_BUTTON1>
        <EVT_BUTTON2>WHITE_TOGGLE</EVT_BUTTON2>
        <EVT_BUTTON3>WHITE_INC</EVT_BUTTON3>
        <EVT_BUTTON4>IDLE</EVT_BUTTON4>
        <EVT_TIMEOUT>IDLE</EVT_TIMEOUT>
      </WHITE_CTRL>
      <WHITE_INC index="2" on_enter="ENT_WHITE_INC">
        <ELSE>WHITE_CTRL</ELSE>
      </WHITE_INC>
      <WHITE_DEC index="3" on_enter="ENT_WHITE_DEC">
        <ELSE>WHITE_CTRL</ELSE>
      </WHITE_DEC>
      <WHITE_TOGGLE index="4" on_enter="ENT_WHITE_TOGGLE">
        <ELSE>WHITE_CTRL</ELSE>
      </WHITE_TOGGLE>
    </states>
    <events>
      <EVT_BUTTON1 index="0" access="PRIVATE"/>
      <EVT_BUTTON2 index="1" access="PRIVATE"/>
      <EVT_BUTTON3 index="2" access="PRIVATE"/>
      <EVT_BUTTON4 index="3" access="PRIVATE"/>
      <EVT_TIMEOUT index="4" access="PRIVATE"/>
      <EVT_START index="5" access="PUBLIC"/>
    </events>
    <connectors>
      <WHITE autostore="0" broadcast="0" dir="PUSH" slots="1"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/
