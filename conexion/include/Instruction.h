#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include <Connection.h>

class Instruction {
	public:
	
	//Implementable function at the receiving side to apply any neccesary changes to the app's state
	virtual void exec(Connection*) const throw() = 0;
};

#endif

