#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/propgrid/propgrid.h>

#include "app/context.h"


namespace PacketHacker {
namespace UI {


  class ReceivedPane : public wxPanel
  {
  public:
    ReceivedPane(Context *context,
      wxWindow *parent,
      wxWindowID winid = wxID_ANY,
      const wxPoint &pos = wxDefaultPosition,
      const wxSize &size = wxDefaultSize,
      long style = wxTAB_TRAVERSAL | wxNO_BORDER,
      const wxString &name = wxPanelNameStr);

    void SetPacket(Packet *packet);
    void OnPacketSent();

  private:
    Context *m_pContext;
    wxPropertyGrid *m_pPropGrid;
  };

}// namespace UI
}// namespace PacketHacker