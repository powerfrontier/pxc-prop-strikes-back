#include <SharedStructs.h>

class SendServerLoad : public Instruction {

  public:
  SendServerLoad() : Instruction("SendServerLoad") {}
  void exec();
};

