#include <stdint.h>

#include "main_window.h"
#include "packet/packet.h"

namespace PacketHacker
{
    MainWindow::MainWindow()
        : wxFrame(nullptr, wxID_ANY, "Packet Hacker", wxDefaultPosition, wxSize(1280, 720))
    {
        m_pContext = new Context();

        // Menu Bar
        m_pMenuBar = new wxMenuBar();
        m_pAdapterMenu = new wxMenu();
        int adapterId = 0;
        for (AdapterInfo info : m_pContext->GetAdapters())
            m_pAdapterMenu->Append(adapterId++, info.friendlyName);
        m_pMenuBar->Append(m_pAdapterMenu, "Adapters");

        m_pPacketMenu = new wxMenu();
        int packetId = adapterId;
        for (PacketInfo info : m_pContext->GetPacketTypes())
            m_pPacketMenu->Append(packetId++, info.name);
        m_pMenuBar->Append(m_pPacketMenu, "Add");
        // End menu 

        // Main panel
        m_pSizer = new wxBoxSizer(wxVERTICAL);

        m_pPacketTree = new UI::PacketTree(this);
        m_pSizer->Add(m_pPacketTree, 2, wxEXPAND);

        m_pByteViewer = new UI::ByteViewer(this);
        m_pSizer->Add(m_pByteViewer, 2, wxEXPAND);

        m_pSendButton = new wxButton(this, ID_SENDBUTTON, "Send");
        m_pSizer->Add(m_pSendButton, 0, wxEXPAND);

        this->Connect(ID_SENDBUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnButtonPressed));
        for (int j = 0; j < adapterId; j++)
            this->Connect(j, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnAdapterSelected));

        for (int j = adapterId; j < packetId + adapterId; j++) 
            this->Connect(j, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnPacketSelected));

        this->CreateStatusBar(1);
        this->SetStatusText("Adapter not selected!", 0);

        this->SetMenuBar(m_pMenuBar);
        this->SetSizer(m_pSizer);
    }

    MainWindow::~MainWindow()
    {
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
        if (event.GetId() >= m_pContext->GetAdapters().size()) return;
        m_pContext->SetAdapter(event.GetId());
        this->SetStatusText(wxString::Format("Selected: %s", m_pContext->GetAdapter().friendlyName));
    }

    void MainWindow::OnPacketSelected(wxCommandEvent &event)
    {
        int index = event.GetId() - m_pContext->GetAdapters().size();
        m_pContext->AddPacket(index);
        m_pPacketTree->SetPacket(m_pContext->GetBasePacket());
    }

} // namespace PacketHacker