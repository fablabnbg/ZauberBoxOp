/*
 * SubMachine.h
 *
 *  Created on: 02.01.2022
 *      Author: ian
 */

#pragma once

#include <Automaton.h>
#include <Machine.hpp>

class AtmMenu;  // forward reference to class to avoid circular header dependencies

class SubMachine: public Machine {
public:
	SubMachine(AtmMenu& mm);
	virtual SubMachine& onExit( Machine& machine, int event = 0 );
	virtual SubMachine& start() {return *this;};
protected:
	void exit_machine();
    AtmMenu& mainMenu;
private:
    atm_connector exit_connectors[1];

};
