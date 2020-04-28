#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/propgrid/propgrid.h>
#include "packet/packet.h"

namespace PacketHacker
{
namespace UI
{
class PacketTree : public wxPanel
{
public:
    PacketTree(wxWindow *parent,
               wxWindowID winid = wxID_ANY,
               const wxPoint &pos = wxDefaultPosition,
               const wxSize &size = wxDefaultSize,
               long style = wxTAB_TRAVERSAL | wxNO_BORDER,
               const wxString &name = wxPanelNameStr);

    void SetPacket(Packet *packet);

private:
    wxPropertyGrid *m_pPropGrid = nullptr;
    Packet *m_pBasePacket = nullptr;
};

} // namespace UI

} // namespace PacketHacker