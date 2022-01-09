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
#include <AtmMenuSetPoint.h>
#include <version.h>


LoggerNode LN;

// Menü
AtmMenu menu;
HomieMenuCoffeeTimer hctimer(menu);
HomieMenuLEDRGBW hledctrl_b(menu);		// LED Ctrl below
Atm_MenuSetPoint menuHeat(menu);

void setup(void) {
	Serial.begin(74880);
	Serial.println("Zauberbox Op - " VERSION);
	Serial.flush();
	menu.begin();
	menu.trace(Serial);
	menu.setSubMenu(AtmMenu::MenuState::COFFEE, &(hctimer.getCstm())); // TODO: HCMTimer (CtStm) is a Sub-Sub-Menu
	menu.setSubMenu(AtmMenu::MenuState::LED_B, &(hledctrl_b.getStm()));
	menu.setSubMenu(AtmMenu::MenuState::HEATER, &menuHeat);

	Homie_setFirmware("Zauberbox OpBox - IAKüche", VERSION_SHORT);
	Homie.disableLedFeedback();
	Homie.disableResetTrigger();
	Homie.setLoggingPrinter(&Serial);
	Homie.setup();
}


void loop(void) {
	automaton.run();
	Homie.loop();
}
