#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "context.h"
#include "packet/packet.h"
#include "ui/byte_viewer.h"
#include "ui/packet_tree.h"
#include "ui/details_pane.h"
#include "ui/stream_pane.h"

namespace PacketHacker {
class MainWindow : public wxFrame
{
public:
  MainWindow();
  ~MainWindow();

  void OnButtonPressed(wxCommandEvent &event);
  void OnAdapterSelected(wxCommandEvent &event);
  void OnPacketSelected(wxCommandEvent &event);

  UI::ByteViewer *GetByteViewer() const { return m_pByteViewer; }
  Context *GetContext() const { return m_pContext; }

private:
  Context *m_pContext;

  wxButton *m_pSendButton;
  wxMenuBar *m_pMenuBar;
  wxMenu *m_pAdapterMenu;
  wxMenu *m_pPacketMenu;

  UI::PacketTree *m_pPacketTree;
  UI::ByteViewer *m_pByteViewer;
  UI::DetailsPane *m_pDetailsPane;
  UI::StreamPane *m_pStreamPane;
};

const int ID_SENDBUTTON = 101;

}// namespace PacketHacker