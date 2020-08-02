#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/propgrid/propgrid.h>

#include "packet/utils/adapter_utils.h"

namespace PacketHacker {
namespace UI {

  class DetailsPane : public wxPanel
  {
  public:
    DetailsPane(wxWindow *parent,
      wxWindowID winid = wxID_ANY,
      const wxPoint &pos = wxDefaultPosition,
      const wxSize &size = wxDefaultSize,
      long style = wxTAB_TRAVERSAL | wxNO_BORDER,
      const wxString &name = wxPanelNameStr);

    void SetAdapterInfo(AdapterInfo &info);

  private:
    wxPropertyGrid *m_pPropGrid;
  };

}// namespace UI
}// namespace PacketHacker