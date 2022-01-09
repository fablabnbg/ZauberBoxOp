/*
 * HomieMenuLEDRGBW.cpp
 *
 *  Created on: 10.10.2021
 *      Author: ian
 */

#include <HomieMenuLEDRGBW.h>
#include <LoggerNode.h>

HomieMenuLEDRGBW::HomieMenuLEDRGBW(AtmMenu& mm):
	HomieNode("RGBWCtrl", "Menüsteuerung für RGB", "rgbw_ctrl"),
	ledstm(mm)
{

}

void HomieMenuLEDRGBW::setup() {
	advertise("rgb_cmd").setDatatype("enum").setFormat("ON:OFF:INC_B:DEC_B:INC_H:DEC_H:INC_V:DEC_V").setRetained(false).settable();
	advertise("rgb_state").setDatatype("color").setFormat("hsv").settable();
	advertise("w_cmd").setDatatype("enum").setFormat("ON:OFF:INCREASE:DECREASE:TOGGLE").setRetained(false);
	advertise("w_state").setDatatype("integer").setUnit("%").setFormat("0:100").settable();

	ledstm.begin();
	ledstm.trace(Serial);
	//sensor.setEnvDataCallback( [](float temp, float hrel, float pressure) -> bool {thermo.setActTemp(temp*10); });
	ledstm.onWhite([this]( int idx, int v, int up ) {handleStmCallback(idx, v, up);} , 0);
	//TODO: (in onReadyToOperatate()): Read current value of mainState
}

bool HomieMenuLEDRGBW::handleInput(const HomieRange &range, const String &property, const String &value) {
	if (property == "rgbstate") {
		return true;
	}

	return false;
}

void HomieMenuLEDRGBW::handleStmCallback(int idx, int v, int up) {
	LN.logf("HomieMenuLEDRGBW::handleStmCallback", LoggerNode::DEBUG, "StateChange [%x, %d, %d]\n", idx, v, up);
	String command;
	switch (idx) {
	case 0:  // White
		switch (up) {
		case -1:
			command = "DECREASE";
			break;
		case 0:
			command = "TOGGLE";
			break;
		case 1:
			command = "INCREASE";
			break;
		}
		setProperty("w_cmd").setRetained(false).send(command);
		break;
	default:
		LN.logf("HomieMenuLEDRGBW::handleStmCallback", LoggerNode::WARNING, "Unkown IDX (Property) %x\n", idx);
		break;
	}
}
