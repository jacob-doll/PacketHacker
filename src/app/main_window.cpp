#include <stdint.h>

#include "main_window.h"
#include "packet/utils/utils.h"
#include "packet/packets.h"

namespace PacketHacker
{
MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "Packet Hacker")
{
    m_pContext = new Context();
    m_pMenuBar = new wxMenuBar();
    // Adapter Menu
    m_pAdapterMenu = new wxMenu();
    int i = 0;
    for (AdapterInfo info : Utils::GetAdapters())
        m_pAdapterMenu->Append(i++, info.friendlyName);
    m_pMenuBar->Append(m_pAdapterMenu, "Adapters");

    m_pSizer = new wxBoxSizer(wxVERTICAL);

    m_pByteViewer = new UI::ByteViewer(this);

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
    base->WriteToBuf(data, base->Size());

    for (int i = 0; i < base->Size(); i++)
    {
        m_pByteViewer->SetByte(i, data[i]);
    }

    delete data;
    delete base;

    m_pSizer->Add(m_pByteViewer, 2, wxEXPAND);

    m_pSendButton = new wxButton(this, ID_SENDBUTTON, "Send");
    m_pSizer->Add(m_pSendButton, 0, wxEXPAND);

    this->Connect(ID_SENDBUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnButtonPressed));
    for (int j = 0; j < i; j++)
        this->Connect(j, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnAdapterSelected));

    this->CreateStatusBar(1);
    this->SetStatusText("Adapter not selected!", 0);

    this->SetMenuBar(m_pMenuBar);
    this->SetSizer(m_pSizer);
}

MainWindow::~MainWindow()
{
    if (m_pContext)
        delete m_pContext;
}

void MainWindow::OnButtonPressed(wxCommandEvent &event)
{
    if (!m_pContext->IsAdapterSet())
    {
        wxLogMessage("Adapter is not set.");
        return;
    }
    std::vector<uint8_t> data = m_pByteViewer->GetData();
    if (!m_pContext->SendPacket(data.data(), data.size()))
        wxLogMessage("Could not send data.");
}

void MainWindow::OnAdapterSelected(wxCommandEvent &event)
{
    AdapterInfo info = Utils::GetAdapters()[event.GetId()];
    m_pContext->SetAdapter(info);
    for (int i = 0; i < 6; i++)
        m_pByteViewer->SetByte(6 + i, info.address[i]);
    this->SetStatusText(wxString::Format("Selected: %s", info.friendlyName), 0);
}

} // namespace PacketHacker