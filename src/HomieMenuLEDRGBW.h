/*
 * HomieMenuLEDRGBW.h
 *
 *  Created on: 10.10.2021
 *      Author: ian
 */

#ifndef SRC_HOMIEMENULEDRGBW_H_
#define SRC_HOMIEMENULEDRGBW_H_

#include <HomieNode.hpp>
#include <AtmLEDCtrl.h>



class HomieMenuLEDRGBW: public HomieNode {

public:
	HomieMenuLEDRGBW(AtmMenu& mm);
	SubMachine& getStm() {
		return ledstm;
	}

protected:
	virtual void setup();
	//virtual void loop(); // no override needed
	//virtual void onReadyToOperate(); // no override needed
	virtual bool handleInput(const HomieRange &range, const String &property, const String &value);

private:
	Atm_LEDCtrl ledstm;

	void handleStmCallback(int idx, int v, int up );

};


#endif /* SRC_HOMIEMENULEDRGBW_H_ */
