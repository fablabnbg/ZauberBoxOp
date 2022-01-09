/*
 * AtmMenuSetPoint.h
 *
 *  Created on: 08.01.2022
 *      Author: ian
 */

#pragma once

#include <HomieNode.hpp>
#include <SubMachine.h>

class Atm_MenuSetPoint: public SubMachine, public HomieNode {
public:
  enum { IDLE, SET_CTRL, SET_INC, SET_DEC }; // STATES
  enum { EVT_BUTTON1, EVT_BUTTON2, EVT_BUTTON3, EVT_BUTTON4, EVT_TIMEOUT, EVT_START, ELSE }; // EVENTS
  Atm_MenuSetPoint(AtmMenu& mm);
  virtual ~Atm_MenuSetPoint() {};
  Atm_MenuSetPoint& begin( void );
  Atm_MenuSetPoint& trace( Stream & stream );
  Atm_MenuSetPoint& trigger( int event );
  int state( void );
  Atm_MenuSetPoint& onSetchange( Machine& machine, int event = 0 );
  Atm_MenuSetPoint& onSetchange( atm_cb_push_t callback, int idx = 0 );
  Atm_MenuSetPoint& onWhite(atm_cb_lambda_t callback, int idx = 0);

  //SubMachine
  virtual Atm_MenuSetPoint& start() override;
  virtual Atm_MenuSetPoint& timeout() override;


protected:
  // HomieNode
  virtual void onReadyToOperate() override;
  virtual bool handleInput(const HomieRange& range, const String& property, const String& value) override;
  virtual void setup() override;

 private:
  enum { ENT_IDLE, ENT_SET_CTRL, ENT_SET_INC, ENT_SET_DEC }; // ACTIONS
  enum { ON_SETCHANGE, CONN_MAX }; // CONNECTORS
  atm_connector connectors[CONN_MAX];
  int event( int id );
  void action( int id );

  bool runOnce;
  int16_t deciTemp;
  int16_t deciTempAct;

};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_MenuSetPoint">
    <states>
      <IDLE index="0" sleep="1" on_enter="ENT_IDLE">
        <EVT_START>SET_CTRL</EVT_START>
      </IDLE>
      <SET_CTRL index="1" on_enter="ENT_SET_CTRL">
        <EVT_BUTTON1>SET_DEC</EVT_BUTTON1>
        <EVT_BUTTON3>SET_INC</EVT_BUTTON3>
        <EVT_BUTTON4>IDLE</EVT_BUTTON4>
        <EVT_TIMEOUT>IDLE</EVT_TIMEOUT>
      </SET_CTRL>
      <SET_INC index="2" on_enter="ENT_SET_INC">
        <ELSE>SET_CTRL</ELSE>
      </SET_INC>
      <SET_DEC index="3" on_enter="ENT_SET_DEC">
        <ELSE>SET_CTRL</ELSE>
      </SET_DEC>
    </states>
    <events>
      <EVT_BUTTON1 index="0" access="PRIVATE"/>
      <EVT_BUTTON2 index="1" access="PRIVATE"/>
      <EVT_BUTTON3 index="2" access="PRIVATE"/>
      <EVT_BUTTON4 index="3" access="PRIVATE"/>
      <EVT_TIMEOUT index="4" access="PRIVATE"/>
      <EVT_START index="5" access="PRIVATE"/>
    </events>
    <connectors>
      <SETCHANGE autostore="0" broadcast="0" dir="PUSH" slots="1"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/
