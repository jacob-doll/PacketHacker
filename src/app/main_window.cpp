#include <stdint.h>

#include "main_window.h"
#include "packet/utils/utils.h"

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
    for (int i = 0; i < 74; i++)
        m_pByteViewer->SetByte(i, 0x00);
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