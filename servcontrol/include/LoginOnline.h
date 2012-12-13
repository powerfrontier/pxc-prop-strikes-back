//Instruccio per advertir al servidor de login que control es online y que agafi la nostra conexio

#include <Datagram.h>
#include <Instruction.h>

class LoginOnlineSend : public Datagram<LoginOnlineSend> {
public:
	LoginOnlineSend(): Datagram<LoginOnlineSend>("LoginOnlineSend")  {};
};
