// Program to exercise the MD_Parola library
//
// Demonstrates minimum required for sprite animated text.
//
// MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
//


#include <Arduino.h>
#include "AtmMenu.h"
#include <Homie.h>
#include <LoggerNode.h>
#include <HomieMenuCoffeeTimer.h>
#include <HomieMenuLEDRGBW.h>

LoggerNode LN;

// Menü
AtmMenu menu;
HomieMenuCoffeeTimer hctimer;
HomieMenuLEDRGBW hledctrl_b;		// LED Ctrl below

void setup(void) {
	Serial.begin(74880);
	Serial.println("Zauberbox OpBox");
	Serial.flush();
	menu.begin();
	menu.trace(Serial);
	menu.setSubMenu(4, &(hctimer.getCtstm())); // TODO: HCMTimer is a Sub-Sub-Menu
	menu.setSubMenu(1, &(hledctrl_b.getStm()));

	Homie_setFirmware("Zauberbox OpBox - IAKüche", "0.0.2");
	Homie.disableLedFeedback();
	Homie.disableResetTrigger();
	Homie.setLoggingPrinter(&Serial);
	Homie.setup();
}


void loop(void) {
	automaton.run();
	Homie.loop();
}
