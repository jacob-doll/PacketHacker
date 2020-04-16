#include "main.h"
#include "main_window.h"
#include "packet/packets.h"
#include "context.h"

IMPLEMENT_APP(PacketHackerApp)

bool PacketHackerApp::OnInit()
{
    using namespace PacketHacker;
    EthernetPacket *base = new EthernetPacket();
    uint8_t dstMac[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint8_t srcMac[] = {0xe0, 0xd5, 0x5e, 0x61, 0xb5, 0x7d};
    ArpPacket *arp = new ArpPacket();
    uint8_t emptyMac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t senderIp[] = {0xc0, 0xa8, 0x01, 0x0c};
    uint8_t targetIp[] = {0xc0, 0xa8, 0x01, 0x01};
    base->SetDstMac(dstMac);
    base->SetSrcMac(srcMac);
    base->SetType(0x0806);
    arp->SetHardwareType(0x0001);
    arp->SetProtocolType(0x0800);
    arp->SetHardwareSize(0x06);
    arp->SetProtocolSize(0x04);
    arp->SetOpcode(0x0001);
    arp->SetSenderMac(srcMac);
    arp->SetSenderIp(senderIp);
    arp->SetTargetMac(emptyMac);
    arp->SetTargetIp(targetIp);
    base->SetInnerPacket(arp);
    uint8_t *data = new uint8_t[base->Size()];
    char buffer[2];
    wxString bufferString;
    base->WriteToBuf(data, base->Size());
    for (int i = 0; i < base->Size(); i++)
    {
        itoa(data[i], buffer, 16);
        bufferString.append(wxString::Format("%02s", buffer));
    }
    wxLogMessage(bufferString);

    Context *context = new Context();
    context->SendPacket(data, base->Size());

    delete data;
    delete context;
    delete base;

    // MainWindow *window = new MainWindow();
    // window->Center();
    // window->Show();
    return true;
}