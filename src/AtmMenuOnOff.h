/*
 * AtmMenuOnOff.h
 *
 *  Created on: 08.01.2022
 *      Author: ian
 */

#pragma once

#include <SubMachine.h>

class Atm_MenuOnOff: public SubMachine {
public:
	Atm_MenuOnOff(AtmMenu &mm) : SubMachine(mm), runOnce(false) {};
	virtual ~Atm_MenuOnOff() {}	;

	enum {
		IDLE, CTRL, CTRL_TOGGLE
	}; // STATES
	enum {
		EVT_BUTTON1,
		EVT_BUTTON2,
		EVT_BUTTON3,
		EVT_BUTTON4,
		EVT_TIMEOUT,
		EVT_START,
		ELSE
	}; // EVENTS

	Atm_MenuOnOff& begin(void);
	Atm_MenuOnOff& trace(Stream &stream);
	Atm_MenuOnOff& trigger(int event);
	int state(void);
	Atm_MenuOnOff& onCtrl(Machine &machine, int event = 0);
	Atm_MenuOnOff& onCtrl(atm_cb_push_t callback, int idx = 0);
	Atm_MenuOnOff& onCtrl(atm_cb_lambda_t callback, int idx);

	Atm_MenuOnOff& start(void);

private:
	enum { ENT_IDLE, ENT_CTRL, ENT_CTRL_TOGGLE }; // ACTIONS
	enum {	ON_CTRL, CONN_MAX	}; // CONNECTORS
	atm_connector connectors[CONN_MAX];
	bool runOnce;

	int event(int id);
	void action(int id);

	static const unsigned char sprite_clean[];
};


/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="Atm_MenuOnOff">
    <states>
      <IDLE index="0" sleep="1" on_enter="ENT_IDLE">
        <EVT_START>CTRL</EVT_START>
      </IDLE>
      <CTRL index="1" on_enter="ENT_CTRL">
        <EVT_BUTTON2>CTRL_TOGGLE</EVT_BUTTON2>
        <EVT_BUTTON4>IDLE</EVT_BUTTON4>
        <EVT_TIMEOUT>IDLE</EVT_TIMEOUT>
      </CTRL>
      <CTRL_TOGGLE index="2" on_enter="ENT_CTRL_TOGGLE">
        <ELSE>CTRL</ELSE>
      </CTRL_TOGGLE>
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
      <CTRL autostore="0" broadcast="0" dir="PUSH" slots="1"/>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/
