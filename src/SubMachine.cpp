/*
 * SubMachine.cpp
 *
 *  Created on: 02.01.2022
 *      Author: ian
 */

#include <SubMachine.h>

SubMachine::SubMachine(AtmMenu& mm): Machine(), mainMenu(mm) {

}

SubMachine& SubMachine::onExit(Machine &machine, int event) {
	onPush(exit_connectors, 0, 0, 1, 1, machine, event );
	return *this;
}

void SubMachine::exit_machine() {
	push(exit_connectors, 0, 0, 0, 0);
}
