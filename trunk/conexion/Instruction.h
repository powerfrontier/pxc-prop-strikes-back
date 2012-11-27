#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include <string>

class Instruction {
	public:
	
	virtual void exec() const throw() = 0;
};

#endif

