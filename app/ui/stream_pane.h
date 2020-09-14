#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/thread.h>
#include <wx/listctrl.h>

#include "app/context.h"
#include "packet/packet_queue.h"

namespace PacketHacker {
namespace UI {

  wxDECLARE_EVENT(myEVT_THREAD_UPDATE, wxThreadEvent);

  class StreamPane : public wxPanel
    , public wxThreadHelper
  {
  public:
    StreamPane(Context *context,
      wxWindow *parent,
      wxWindowID winid = wxID_ANY,
      const wxPoint &pos = wxDefaultPosition,
      const wxSize &size = wxDefaultSize,
      long style = wxTAB_TRAVERSAL | wxNO_BORDER,
      const wxString &name = wxPanelNameStr);

    void StartRead();
    void OnThreadUpdate(wxThreadEvent &evt);
    void OnClose(wxCloseEvent &evt);

  protected:
    virtual wxThread::ExitCode Entry();

    PacketQueue m_queue;
    wxCriticalSection m_queueCS;

  private:
    Context *m_pContext;
    wxListView *m_pListView;
  };

}// namespace UI
}// namespace PacketHacker