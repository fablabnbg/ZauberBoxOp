/*
 * AtmMenu.h
 *
 *  Created on: 29.09.2021
 *      Author: ian
 */

#pragma once

#include <Automaton.h>
#include <SubMachine.h>
#include <Atm_button.hpp>
#include <Atm_timer.hpp>
#include <atm_timer_millis.hpp>
#include <atm_counter.hpp>
#include <MD_MAXPanel.h>
#include <MD_MAX72xx.h>
#include <SPI.h>


class AtmMenu: public Machine {

 public:
	enum MenuState {
		IDLE=0, LED_B, LED_CEIL, HEATER, COFFEE, LEAVE_SUB, STATE_COUNT
	} ; // STATES
	  enum { EVT_BUTTON1, EVT_BUTTON2, EVT_BUTTON3, EVT_BUTTON4, EVT_TIMEOUT, EVT_ANI_STEP, EVT_ANI_FINISH, EVT_SUB_EXIT, ELSE }; // EVENTS
  AtmMenu( void );
  virtual ~AtmMenu(void) {};

  AtmMenu& begin( void );
  AtmMenu& trace( Stream & stream );
  AtmMenu& trigger( int event );
  int state( void );

  void setSubMenu(uint_fast8_t idx, SubMachine *m);

  /* LED Matrix functions (that use MD_Parola/MD_MAX72xx libraries).
   *     The LED Matrix integration is tightly coupled to this statemachine. This is a design decision to avoid complexity.
   *     In future version there may be a loser coupling, e.g. by providing a own class for interaction with the LED matrix
   *
   *   These function can be called by the Submachines to access the LED matrix
   *
   */

  void showIcon8x8(uint8_t pos, const uint8_t* icon_data);
  void showSmallText(const String& txt);
  void showDeciInt(const int16_t dInt);


  /* Other functions for SubMenus
   *
   *
   *
   */

  void resetTimeout() {
	  /*if (!(timer.value == ATM_TIMER_OFF || timer.expired(this)))*/ timer.set(5000)); 	// Default Timeout 5s
  }


 private:
  enum { ENT_IDLE, EXT_IDLE, ENT_LED_B, ENT_LED_CEIL, ENT_HEATER, ENT_COFFEE, ENT_LEAVE_SUB, EXT_LEAVE_SUB }; // ACTIONS
  int event( int id );
  void action( int id );

  Atm_button but1;
  Atm_button but2;
  Atm_button but3;
  Atm_button but4;
  atm_timer_millis timer;
  Atm_timer animator;

  SubMachine* sub_menu[STATE_COUNT];

  // Define the number of devices we have in the chain and the hardware interface
  // NOTE: These pin numbers will probably not work with your hardware and may
  // need to be adapted
  #define HARDWARE_TYPE MD_MAX72XX::FC16_HW
  #define MAX_DEVICES 4
  MD_MAXPanel panel;

  static const uint8_t sprite_startPos[4];
public:
  static const uint8_t sprite_coffee[9];
  static const uint8_t sprite_heater[9];
  static const uint8_t sprite_light_ceil[9];
  static const uint8_t sprite_light_below[9];
  static const uint8_t sprite_onoff[9];




};

/*
Automaton::ATML::begin - Automaton Markup Language

<?xml version="1.0" encoding="UTF-8"?>
<machines>
  <machine name="AtmMenu">
    <states>
      <IDLE index="0" on_enter="ENT_IDLE">
        <EVT_BUTTON1>HEATER</EVT_BUTTON1>
        <EVT_BUTTON2>LED_B</EVT_BUTTON2>
        <EVT_BUTTON3>LED_CEIL</EVT_BUTTON3>
        <EVT_BUTTON4>COFFEE</EVT_BUTTON4>
      </IDLE>
      <LED_B index="1" on_enter="ENT_LED_B">
        <EVT_TIMEOUT>LEAVE_SUB</EVT_TIMEOUT>
        <EVT_SUB_EXIT>LEAVE_SUB</EVT_SUB_EXIT>
      </LED_B>
      <LED_CEIL index="2" on_enter="ENT_LED_CEIL">
        <EVT_TIMEOUT>LEAVE_SUB</EVT_TIMEOUT>
        <EVT_SUB_EXIT>LEAVE_SUB</EVT_SUB_EXIT>
      </LED_CEIL>
      <HEATER index="3" on_enter="ENT_HEATER">
        <EVT_TIMEOUT>LEAVE_SUB</EVT_TIMEOUT>
        <EVT_SUB_EXIT>LEAVE_SUB</EVT_SUB_EXIT>
      </HEATER>
      <COFFEE index="4" on_enter="ENT_COFFEE">
        <EVT_TIMEOUT>LEAVE_SUB</EVT_TIMEOUT>
        <EVT_SUB_EXIT>LEAVE_SUB</EVT_SUB_EXIT>
      </COFFEE>
      <LEAVE_SUB index="5" on_enter="ENT_LEAVE_SUB">
        <EVT_ANI_FINISH>IDLE</EVT_ANI_FINISH>
      </LEAVE_SUB>
    </states>
    <events>
      <EVT_BUTTON1 index="0" access="PRIVATE"/>
      <EVT_BUTTON2 index="1" access="PRIVATE"/>
      <EVT_BUTTON3 index="2" access="PRIVATE"/>
      <EVT_BUTTON4 index="3" access="PRIVATE"/>
      <EVT_TIMEOUT index="4" access="PRIVATE"/>
      <EVT_ANI_FINISH index="5" access="PRIVATE"/>
      <EVT_SUB_EXIT index="6" access="PRIVATE"/>
    </events>
    <connectors>
    </connectors>
    <methods>
    </methods>
  </machine>
</machines>

Automaton::ATML::end
*/
