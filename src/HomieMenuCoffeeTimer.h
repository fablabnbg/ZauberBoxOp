/*
 * HomieMenuCoffeeTimer.h
 *
 *  Created on: 10.10.2021
 *      Author: ian
 */

#ifndef SRC_HOMIEMENUCOFFEETIMER_H_
#define SRC_HOMIEMENUCOFFEETIMER_H_

#include <HomieNode.hpp>
#include <Atm_CoffeeTimer.h>
#include <AtmMenuOnOff.h>

class HomieMenuCoffeeTimer: public HomieNode {
public:
	HomieMenuCoffeeTimer(AtmMenu& mm);

	SubMachine& getCtstm() {
		return ctstm;
	}
	SubMachine& getCstm() {
		return cstm;
	}

	bool toggleMainState();


protected:
	virtual void setup();
	//virtual void loop(); // no override needed
	//virtual void onReadyToOperate(); // no override needed
	virtual bool handleInput(const HomieRange &range, const String &property, const String &value);

private:
	Atm_CoffeeTimer ctstm;
	Atm_MenuOnOff   cstm;
	bool mainState;
};

#endif /* SRC_HOMIEMENUCOFFEETIMER_H_ */
