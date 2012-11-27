#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include <string>

class Instruction {
	public:
	
	//Implementable function at the receiving side to apply any neccesary changes to the app's state
	virtual void exec() const throw() = 0;
};

#endif

