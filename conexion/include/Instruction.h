#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

class Connection;

class Instruction {
	public:
	
	//Implementable function at the receiving side to apply any neccesary changes to the app's state
	virtual void exec(Connection*) const throw() = 0;
	virtual bool gameInstruction() const throw() = 0;
	virtual int id() const throw() = 0;
};

#endif

