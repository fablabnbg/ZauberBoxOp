/*
 * HomieMenuCoffeeTimer.cpp
 *
 *  Created on: 10.10.2021
 *      Author: ian
 */

#include <HomieMenuCoffeeTimer.h>
#include <LoggerNode.h>


HomieMenuCoffeeTimer::HomieMenuCoffeeTimer(AtmMenu& mm):
	HomieNode("coffeetimer", "Menüsteuerung Küche IA: CafeTimer", "menu"),
	ctstm(mm),
	mainState(false)
{

}

void HomieMenuCoffeeTimer::setup() {
	advertise("coffeetimer_mainstate").setDatatype("bool").settable().setFormat("on:off").setName("Rocket Apartamento ON/OFF").setRetained(true);
	advertise("coffeetimer_cleanerstate").setDatatype("enum").setFormat("idle:prepare:clean:flush:empty").setName("Manuelle Reinigung");
	advertise("coffeetimer_heatup").setDatatype("enum").setFormat("off:firstheat:keep:reheat:cooldown").settable().setName("Wärme Boiler (Heuristik)");
	ctstm.begin();
	ctstm.trace(Serial);
	//sensor.setEnvDataCallback( [](float temp, float hrel, float pressure) -> bool {thermo.setActTemp(temp*10); });
	ctstm.onStatechange([this]( int idx, int v, int up ) {LN.logf("HMCT:onStateChange()", LoggerNode::DEBUG, "StateChange [%x, %x, %x]\n", idx, v, up);toggleMainState();} , 0);
	//TODO: (in onReadyToOperatate()): Read current value of mainState
}

bool HomieMenuCoffeeTimer::toggleMainState() {
	mainState = !mainState;
	setProperty("coffeetimer_mainstate").setRetained(true).send(mainState? "ON":"OFF"); //TODO: Probably that must be changed to "true":"false" due to Homie convention
	return mainState;
}

bool HomieMenuCoffeeTimer::handleInput(const HomieRange &range,	const String &property, const String &value) {
	if (property == "coffeetimer_mainstate") {
		// On / Off
		return true;
	}

	if (property == "coffeetimer_heatup") {
		return true;

	}
	return false; // unkown property
}

