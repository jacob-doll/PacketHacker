#include <stdint.h>
#include <wx/splitter.h>

#include "main_window.h"
#include "packet/packet.h"

namespace PacketHacker {
MainWindow::MainWindow()
  : wxFrame(nullptr, wxID_ANY, "Packet Hacker", wxDefaultPosition, wxSize(1280, 720))
{
  m_pContext = new Context(this);

  // Menu Bar
  m_pMenuBar = new wxMenuBar();
  m_pAdapterMenu = new wxMenu();
  int adapterId = 0;
  for (AdapterInfo info : m_pContext->GetAdapters())
    m_pAdapterMenu->Append(adapterId++, info.friendlyName);
  m_pMenuBar->Append(m_pAdapterMenu, "Adapters");

  m_pPacketMenu = new wxMenu();
  m_pPacketMenu->Append(PacketTypes::ARP, "ARP");
  m_pPacketMenu->Append(PacketTypes::ETHERNET, "ETHERNET");
  m_pMenuBar->Append(m_pPacketMenu, "Add");
  // End menu

  // Main panel
  wxBoxSizer *hBox = new wxBoxSizer(wxHORIZONTAL);
  wxSplitterWindow *splitter = new wxSplitterWindow(this);

  wxWindow *creationPane = new wxWindow(splitter, wxID_ANY);
  wxBoxSizer *vBox = new wxBoxSizer(wxVERTICAL);
  m_pPacketTree = new UI::PacketTree(m_pContext, creationPane);
  vBox->Add(m_pPacketTree, 2, wxEXPAND);
  m_pByteViewer = new UI::ByteViewer(creationPane);
  vBox->Add(m_pByteViewer, 2, wxEXPAND);
  m_pSendButton = new wxButton(creationPane, ID_SENDBUTTON, "Send");
  vBox->Add(m_pSendButton, 0, wxEXPAND);
  creationPane->SetSizer(vBox);

  m_pDetailsPane = new UI::DetailsPane(splitter);

  splitter->SplitVertically(m_pDetailsPane, creationPane);
  splitter->SetMinimumPaneSize(400);
  hBox->Add(splitter, 1, wxEXPAND);

  this->Connect(ID_SENDBUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainWindow::OnButtonPressed));
  for (int j = 0; j < adapterId; j++)
    this->Connect(j, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnAdapterSelected));

  this->Connect(PacketTypes::ARP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnPacketSelected));
  this->Connect(PacketTypes::ETHERNET, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnPacketSelected));

  this->CreateStatusBar(1);
  this->SetStatusText("Adapter not selected!", 0);

  this->SetMenuBar(m_pMenuBar);
  this->SetSizer(hBox);
}

MainWindow::~MainWindow()
{
  delete m_pContext;
}

void MainWindow::OnButtonPressed(wxCommandEvent &event)
{
  if (!m_pContext->SendPacket()) {
    wxLogMessage("Could not send data.");
  }
}

void MainWindow::OnAdapterSelected(wxCommandEvent &event)
{
  if (event.GetId() >= m_pContext->GetAdapters().size()) return;
  m_pContext->SetAdapter(event.GetId());
  m_pDetailsPane->SetAdapterInfo(m_pContext->GetAdapter());
  this->SetStatusText(wxString::Format("Selected: %s", m_pContext->GetAdapter().friendlyName));
}

void MainWindow::OnPacketSelected(wxCommandEvent &event)
{
  m_pContext->AddPacket(event.GetId());
  m_pPacketTree->SetPacket(m_pContext->GetBasePacket());
}

}// namespace PacketHacker