/*
 * AtmMenu.cpp
 *
 *  Created on: 29.09.2021
 *      Author: ian
 */


#include "AtmMenu.h"

#include <fonts/3x5.h>

AtmMenu::AtmMenu(void):
		Machine(),
		Parola(HARDWARE_TYPE, PIN_SPI_MOSI, PIN_SPI_SCK, PIN_SPI_SS, MAX_DEVICES)
{

}

/* Add optional parameters for the state machine to begin()
 * Add extra initialization code
 */
AtmMenu& AtmMenu::begin() {
  // clang-format off
	  const static state_t state_table[] PROGMEM = {
	    /*                   ON_ENTER  ON_LOOP  ON_EXIT  EVT_BUTTON1  EVT_BUTTON2  EVT_BUTTON3  EVT_BUTTON4  EVT_TIMEOUT  EVT_ANI_STEP  EVT_ANI_FINISH  EVT_SUB_EXIT  ELSE */
	    /*      IDLE */      ENT_IDLE,      -1,      -1,      HEATER,       LED_B,    LED_CEIL,      COFFEE,          -1,        -1,            -1,           -1,   -1,
	    /*     LED_B */     ENT_LED_B,      -1,      -1,          -1,          -1,          -1,          -1,   LEAVE_SUB,        -1,            -1,    LEAVE_SUB,   -1,
	    /*  LED_CEIL */  ENT_LED_CEIL,      -1,      -1,          -1,          -1,          -1,          -1,   LEAVE_SUB,        -1,            -1,    LEAVE_SUB,   -1,
	    /*    HEATER */    ENT_HEATER,      -1,      -1,          -1,          -1,          -1,          -1,   LEAVE_SUB,        -1,            -1,    LEAVE_SUB,   -1,
	    /*    COFFEE */    ENT_COFFEE,      -1,      -1,          -1,          -1,          -1,          -1,   LEAVE_SUB,        -1,            -1,    LEAVE_SUB,   -1,
	    /* LEAVE_SUB */ ENT_LEAVE_SUB, LP_LEAVE_SUB, EXT_LEAVE_SUB,  -1,       -1,          -1,          -1,   -1,         LEAVE_SUB,          IDLE,           -1,   -1,
	  };

  // clang-format on
	Machine::begin(state_table, ELSE);
	but1.begin(D1).onPress(*this, EVT_BUTTON1);
	but2.begin(D2).onPress(*this, EVT_BUTTON2);
	but3.begin(D3).onPress(*this, EVT_BUTTON3);
	but4.begin(D4).onPress(*this, EVT_BUTTON4);


	Parola.begin();
	//Parola.displayText("Initialize", PA_CENTER, Parola.getSpeed(), 1000, PA_SPRITE, PA_SPRITE);
	//Parola.setSpriteData(rocket, W_ROCKET, F_ROCKET, rocket, W_ROCKET, F_ROCKET);
	//Parola.setSpriteData(sprite_coffee, 8, 1, sprite_coffee, 8, 1);
	return *this;
}

void AtmMenu::setSubMenu(uint_fast8_t idx, SubMachine *m) {
	sub_menu[idx] = m;
	m->onExit(*this, EVT_SUB_EXIT);

}

/* Add C++ code for each internally handled event (input)
 * The code must return 1 to trigger the event
 */

int AtmMenu::event( int id ) {
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
      return timer.expired(this);
    case EVT_ANI_FINISH:
    	return 0;
    case EVT_SUB_EXIT:
      return 0;
  }
  return 0;
}


/* Add C++ code for each action
 * This generates the 'output' for the state machine
 */

void AtmMenu::action(int id) {
	if (Parola.displayAnimate()) Parola.displayReset();
	switch (id) {
	case ENT_IDLE:
		timer.set(ATM_TIMER_OFF);
		animator.begin(0);
		Parola.displaySuspend(true);
		showIcon8x8(3, sprite_coffee);
		showIcon8x8(2, sprite_light_ceil);
		showIcon8x8(1, sprite_light_below);
		showIcon8x8(0, sprite_heater);
		return;
//	case EXT_IDLE:
//		Parola.displaySuspend(false);
//		timer.set(2000);
//		return;
	case ENT_LED_B:
		//timer.set(5000);
		timer.set(15000);
		Parola.displaySuspend(true);
		Parola.displayClear();
		sub_menu[LED_B]->start();
//		Parola.displayText("LED W", PA_CENTER, Parola.getSpeed(), 100, PA_SPRITE, PA_SPRITE);
//		Parola.setSpriteData(sprite_light, 8, 1, sprite_light, 8, 1);
		return;
	case ENT_LED_CEIL:
		timer.set(5000);
//		Parola.displayText("LED RGBV", PA_CENTER, Parola.getSpeed(), 100, PA_SPRITE, PA_SPRITE);
//		Parola.setSpriteData(sprite_light_ceil, 8, 1, sprite_light_ceil, 8, 1);
		return;
	case ENT_HEATER:
		timer.set(ATM_TIMER_OFF);
		Parola.displaySuspend(true);
		Parola.displayClear();
		sub_menu[HEATER]->start();
//
//		Parola.displayText("Thermo", PA_CENTER, Parola.getSpeed(), 100, PA_SPRITE, PA_SPRITE);
//		Parola.setSpriteData(sprite_heater, 8, 1, sprite_heater, 8, 1);
		return;
	case ENT_COFFEE:
		//timer.set(5000);
		timer.set(ATM_TIMER_OFF);
		Parola.displaySuspend(true);
		Parola.displayClear();
		sub_menu[COFFEE]->start();
//		Parola.displayText("Kaffee", PA_CENTER, Parola.getSpeed(), 1000, PA_SPRITE, PA_SPRITE);
//		Parola.setSpriteData(sprite_coffee, 8, 1, sprite_coffee, 8, 1);
		return;
	case ENT_LEAVE_SUB:
		if (animator.state() == Atm_timer::IDLE) animator.begin(100, 7).onTimer(*this, EVT_ANI_STEP).onFinish(*this, EVT_ANI_FINISH).start();
		return;
	case LP_LEAVE_SUB:
      return;
	case EXT_LEAVE_SUB:
		Parola.getGraphicObject()->transform(MD_MAX72XX::TSU);
		return;
	}
}

/* Optionally override the default trigger() method
 * Control how your machine processes triggers
 */

AtmMenu& AtmMenu::trigger(int event) {
	switch (event) {
	case EVT_BUTTON1:
		Serial.println("Button1");
		break;
	case EVT_BUTTON2:
		Serial.println("Button2");
		break;
	case EVT_BUTTON3:
		Serial.println("Button3");
		break;
	case EVT_BUTTON4:
		Serial.println("Button4");
		break;
	default:
		Serial.printf("Unhandled event %x.\n", event);
	}
	assert(state() >= 0 && state() < STATE_COUNT);
	if (sub_menu[state()]) {
		Serial.printf("Forwarding event %x to %s in state %x.\n", event, sub_menu[state()]->getName(), state());
		sub_menu[state()]->trigger(event); // --> Forward event to current sub_menu
	} else {
		Serial.printf("No sub menu active for %x.\n", state());
	}

	Machine::trigger(event);
	return *this;
}

/* Optionally override the default state() method
 * Control what the machine returns when another process requests its state
 */

int AtmMenu::state( void ) {
  return Machine::state();
}

/* Nothing customizable below this line
 ************************************************************************************************
*/

/* State trace method
 * Sets the symbol table and the default logging method for serial monitoring
 */

AtmMenu& AtmMenu::trace( Stream & stream ) {
	  Machine::setTrace( &stream, atm_serial_debug::trace,
	    "ATMMENU\0EVT_BUTTON1\0EVT_BUTTON2\0EVT_BUTTON3\0EVT_BUTTON4\0EVT_TIMEOUT\0EVT_ANI_STEP\0EVT_ANI_FINISH\0EVT_SUB_EXIT\0ELSE\0IDLE\0LED_B\0LED_CEIL\0HEATER\0COFFEE\0LEAVE_SUB" );
//  but1.trace(stream);
//  but2.trace(stream);
//  but3.trace(stream);
//  but4.trace(stream);
  return *this;
}



// PROGMEM can't be used here, because it is directly accesses in called library (MD_MAX72xx)
const uint8_t AtmMenu::sprite_coffee[9] = {0x1c, 0xa4, 0xbc, 0xc4, 0xc5, 0xc6, 0xc4, 0xbc, 0};
const uint8_t AtmMenu::sprite_light_ceil[9] = {0x01, 0x01, 0xfb, 0x0d, 0x0b, 0x0d, 0x09, 0x09,0};
const uint8_t AtmMenu::sprite_light_below[9] = {0x00, 0x47, 0x34, 0x4c, 0x34, 0x0c, 0x04, 0xfc, 0};
const uint8_t AtmMenu::sprite_heater[9] = { 0xa4, 0x92, 0x89, 0x89, 0x92, 0xa4, 0xa4,	0x92, 0 };
const uint8_t AtmMenu::sprite_onoff[9] = { 0x00,0x1c,0x22,0x40,0x47,0x40,0x22,0x1c,0};

const uint8_t AtmMenu::sprite_startPos[4] = { 7, 15, 23, 31 };


void AtmMenu::showIcon8x8(uint8_t pos, const uint8_t *icon_data) {
	assert (pos < 4);  // pos must be 0 <= pos <= 3
	// Unfortunately, MD_MAX72xx is not const correct. icon_data is read only in MD_MAX72xx so it could be const and so it is safe here to cast the const away
	Parola.getGraphicObject()->setBuffer(sprite_startPos[pos], 8, const_cast<uint8_t *>(icon_data));
}


void AtmMenu::showDeciInt(const int16_t dInt) {
	char s[4];
	String intStr(dInt);
	assert(intStr.length()>=3);
	s[0] = intStr.c_str()[0];
	s[1] = intStr.c_str()[1];
	s[2] = '.';
	s[3] = intStr.c_str()[2];

	Parola.getGraphicObject()->setFont(font3x5);
	uint16_t start_pos = 18;
	for (uint_fast8_t count=0; count<=3; count++) {
		start_pos -= Parola.getGraphicObject()->setChar(start_pos, s[count]);
		if (count == 0) start_pos--;
	}

}
void AtmMenu::showSmallText(const String& text) {
//	Parola.setFont(font3x5);
//	Parola.displayText(text.c_str(), PA_CENTER, 0, 0, PA_NO_EFFECT);
//	Parola.displayAnimate();
	Parola.getGraphicObject()->setFont(font3x5);
	Parola.getGraphicObject()->setChar(15, text.c_str()[0]);
	Parola.getGraphicObject()->setChar(8, '5');

}
//uint8_t AtmMenu::sprite_heater[9] = {0x0c,0x92,0x61,0x0c,0x92,0x61,0x00,0xff,0};
//uint8_t AtmMenu::sprite_light[9] = {0x18,0x24,0x42,0x42,0x42,0x24,0x18,0x18,0};
//uint8_t AtmMenu::sprite_heater[9] = {0x0c,0x92,0x61,0x0c,0x92,0x61,0x00,0xff,0};
//uint8_t AtmMenu::sprite_clock[9] =  {0x3c,0x42,0x81,0xf1,0x91,0x91,0x42,0x3c,0};




