#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/propgrid/propgrid.h>

#include <map>
#include <string>
#include <memory>

#include "app/context.h"
#include "packet/packet.h"

#define ID_REMOVE 2001

namespace PacketHacker {
namespace UI {
  class PacketTree : public wxPanel
  {
  public:
    PacketTree(Context *context,
      wxWindow *parent,
      wxWindowID winid = wxID_ANY,
      const wxPoint &pos = wxDefaultPosition,
      const wxSize &size = wxDefaultSize,
      long style = wxTAB_TRAVERSAL | wxNO_BORDER,
      const wxString &name = wxPanelNameStr);

    void SetPacket(Packet *packet);

    void OnPropertyGridChanged(wxPropertyGridEvent &event);
    void OnPropertyGridRightClicked(wxPropertyGridEvent &event);
    void OnPopupClick(wxCommandEvent &event);

  private:
    wxPropertyGrid *m_pPropGrid;
    Context* m_pContext;
  };

}// namespace UI

}// namespace PacketHacker