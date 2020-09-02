#include "packet/packet.h"
#include "packet/packets.h"
#include "packet/interface.h"

int main()
{
  using namespace PacketHacker;
  IPv4Address a1;
  IPv4Address a2(1, 1, 1, 1);
  IPv4Address a3(std::array<uint8_t, 4>{});
  IPv4Address a4 = { 1, 1, 1, 1 };
  IPv4Address a5("1.1.1.1");
  IPv4Address a6 = "1.1.1.1";
  IPv4Address a7(a1.GetData());

  IPv4Address mask(255, 0, 0, 0);
  IPv4Address a2mask = a2 & mask;

  return 0;
}
