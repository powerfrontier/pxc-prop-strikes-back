#include <SharedStructs.h>

class SendServerLoad : public Instruction {

  struct server_carga carga
  server* s

  public:
  SendServerLoad() : Instruction("SendServerLoad") {}
  void exec();
};

