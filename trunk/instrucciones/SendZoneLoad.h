#include <SharedStructs.h>

class SendZoneLoad : public Instruction {
  int mZone;
  double mLoad;

  public:
  SendZoneLoad() : Instruction("SendZoneLoad") {}
  void exec();
};
