#include "stream_pane.h"


namespace PacketHacker {
namespace UI {

  int index = 0;

  wxDEFINE_EVENT(myEVT_THREAD_UPDATE, wxThreadEvent);

  StreamPane::StreamPane(Context *context,
    wxWindow *parent,
    wxWindowID winid,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name)
    : wxPanel(parent, winid, pos, size, style, name),
      m_pContext(context),
      m_queue(5)
  {
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    m_pListView = new wxListView(this, wxID_ANY);
    m_pListView->AppendColumn("Column 1");
    sizer->Add(m_pListView, 1, wxEXPAND);

    this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(StreamPane::OnClose));
    this->Connect(myEVT_THREAD_UPDATE, wxThreadEventHandler(StreamPane::OnThreadUpdate));

    this->SetSizerAndFit(sizer);
  }

  void StreamPane::StartRead()
  {
    if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) {
      wxLogError("Could not create the worker thread!");
      return;
    }
    if (GetThread()->Run() != wxTHREAD_NO_ERROR) {
      wxLogError("Could not run the worker thread!");
      return;
    }
  }

  wxThread::ExitCode StreamPane::Entry()
  {
    while (!GetThread()->TestDestroy()) {
      if (m_pContext->IsAdapterSet()) {
        uint32_t size;
        const uint8_t *data;
        if ((data = m_pContext->ReadNextPacket(&size))) {
          wxCriticalSectionLocker lock(m_queueCS);
          Packet *received = new EthernetPacket(data, size);
          m_queue.InsertPacket(received);
          wxQueueEvent(this, new wxThreadEvent(myEVT_THREAD_UPDATE));
        }
        GetThread()->Sleep(100);
      }
    }

    return (wxThread::ExitCode)0;
  }

  void StreamPane::OnClose(wxCloseEvent &)
  {
    if (GetThread() && GetThread()->IsRunning())
      GetThread()->Wait();
    Destroy();
  }

  void StreamPane::OnThreadUpdate(wxThreadEvent &evt)
  {
    wxCriticalSectionLocker lock(m_queueCS);
    Packet *packet = m_queue.GetPacket();
    if (packet) {
      m_pListView->InsertItem(index++, wxString::Format("%d", packet->Size()));
    }
    delete packet;
  }

}// namespace UI
}// namespace PacketHacker