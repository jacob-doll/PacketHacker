#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/timer.h>

#include "app/context.h"


namespace PacketHacker {
namespace UI {


  class StreamPane : public wxPanel
  {
  public:
    StreamPane(Context *context,
      wxWindow *parent,
      wxWindowID winid = wxID_ANY,
      const wxPoint &pos = wxDefaultPosition,
      const wxSize &size = wxDefaultSize,
      long style = wxTAB_TRAVERSAL | wxNO_BORDER,
      const wxString &name = wxPanelNameStr);

    void OnPacketSent();

  private:
    Context *m_pContext;
    wxListBox *m_pList;
  };

}// namespace UI
}// namespace PacketHacker