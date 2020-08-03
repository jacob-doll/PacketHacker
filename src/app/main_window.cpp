#include "main_window.h"

#include <stdint.h>

#define ADAPTER_OFFSET 3000

namespace PacketHacker {
MainWindow::MainWindow()
  : wxFrame(nullptr, wxID_ANY, _("Packet Hacker"), wxDefaultPosition, wxSize(1280, 720), wxDEFAULT_FRAME_STYLE)
{
  m_pContext = new Context(this);
  m_mgr.SetManagedWindow(this);

  // Menu Bar
  m_pMenuBar = new wxMenuBar();
  m_pAdapterMenu = new wxMenu();
  int adapterId = ADAPTER_OFFSET;
  for (AdapterInfo info : Adapter::GetAvailableAdapters())
    m_pAdapterMenu->Append(adapterId++, info.friendlyName);
  m_pMenuBar->Append(m_pAdapterMenu, "Adapters");

  m_pPacketMenu = new wxMenu();
  m_pPacketMenu->Append(PacketType::ARP, "ARP");
  m_pPacketMenu->Append(PacketType::ETHERNET, "ETHERNET");
  m_pPacketMenu->Append(PacketType::IP, "IP");
  m_pPacketMenu->Append(PacketType::ICMP, "ICMP");
  m_pPacketMenu->Append(PacketType::DATA, "DATA");
  m_pPacketMenu->Append(PacketType::UDP, "UDP");
  m_pMenuBar->Append(m_pPacketMenu, "Add");
  // End menu

  wxAuiToolBar *toolbar = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL);
  m_pSendButton = new wxButton(toolbar, ID_SENDBUTTON, "Send");
  toolbar->AddControl(m_pSendButton);
  m_mgr.AddPane(toolbar, wxAuiPaneInfo().Name("tb1").Caption("Big Toolbar").ToolbarPane().Top());

  m_pPacketTree = new UI::PacketTree(m_pContext, this);
  m_mgr.AddPane(m_pPacketTree, wxCENTER, wxT("Current Packet"));

  m_pDetailsPane = new UI::DetailsPane(this);
  m_mgr.AddPane(m_pDetailsPane, wxLEFT, wxT("Details Pane"));

  m_pStreamPane = new UI::StreamPane(m_pContext, this);
  m_mgr.AddPane(m_pStreamPane, wxRIGHT, wxT("Received Packet"));

  m_pByteViewer = new UI::ByteViewer(this);
  m_mgr.AddPane(m_pByteViewer, wxBOTTOM, wxT("Byte Viewer"));

  this->Connect(ID_SENDBUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnButtonPressed));
  for (int j = ADAPTER_OFFSET; j < adapterId; j++)
    this->Connect(j, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnAdapterSelected));

  this->Connect(PacketType::ARP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnPacketSelected));
  this->Connect(PacketType::ETHERNET, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnPacketSelected));
  this->Connect(PacketType::IP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnPacketSelected));
  this->Connect(PacketType::ICMP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnPacketSelected));
  this->Connect(PacketType::DATA, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnPacketSelected));
  this->Connect(PacketType::UDP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnPacketSelected));

  this->CreateStatusBar(1);
  this->SetStatusText("Adapter not selected!", 0);

  this->SetMenuBar(m_pMenuBar);

  m_mgr.Update();
}

MainWindow::~MainWindow()
{
  m_mgr.UnInit();
  delete m_pContext;
}

void MainWindow::OnButtonPressed(wxCommandEvent &event)
{
  if (!m_pContext->SendPacket()) {
    wxLogMessage("Could not send data.");
    return;
  }
  m_pStreamPane->OnPacketSent();
}

void MainWindow::OnAdapterSelected(wxCommandEvent &event)
{
  if (event.GetId() >= Adapter::GetAvailableAdapters().size() - ADAPTER_OFFSET) return;
  AdapterInfo info = Adapter::GetAvailableAdapters()[event.GetId() - ADAPTER_OFFSET];
  m_pContext->SetAdapter(info.name);
  m_pDetailsPane->SetAdapterInfo(info);
  this->SetStatusText(wxString::Format("Selected: %s", info.friendlyName));
}

void MainWindow::OnPacketSelected(wxCommandEvent &event)
{
  m_pContext->AddPacket(event.GetId());
  m_pPacketTree->SetPacket(m_pContext->GetBasePacket());
}

}// namespace PacketHacker