#include "details_pane.h"

namespace PacketHacker {
namespace UI {
  DetailsPane::DetailsPane(wxWindow *parent,
    wxWindowID winid,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name)
    : wxPanel(parent, winid, pos, size, style, name)
  {
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    m_pPropGrid = new wxPropertyGrid(this, wxID_ANY);
    sizer->Add(m_pPropGrid, 1, wxEXPAND);

    wxPGProperty *adapterProp = m_pPropGrid->Append(new wxPropertyCategory("Current Adapter"));
    m_pPropGrid->AppendIn(adapterProp, new wxStringProperty("Index", wxPG_LABEL, "null"))->ChangeFlag(wxPG_PROP_READONLY, true);
    m_pPropGrid->AppendIn(adapterProp, new wxStringProperty("Name", wxPG_LABEL, "null"))->ChangeFlag(wxPG_PROP_READONLY, true);
    m_pPropGrid->AppendIn(adapterProp, new wxStringProperty("Unicast address", wxPG_LABEL, "null"))->ChangeFlag(wxPG_PROP_READONLY, true);
    m_pPropGrid->AppendIn(adapterProp, new wxStringProperty("Anycast address", wxPG_LABEL, "null"))->ChangeFlag(wxPG_PROP_READONLY, true);
    m_pPropGrid->AppendIn(adapterProp, new wxStringProperty("Multicast address", wxPG_LABEL, "null"))->ChangeFlag(wxPG_PROP_READONLY, true);
    m_pPropGrid->AppendIn(adapterProp, new wxStringProperty("DnsServer address", wxPG_LABEL, "null"))->ChangeFlag(wxPG_PROP_READONLY, true);
    m_pPropGrid->AppendIn(adapterProp, new wxStringProperty("Gateway address", wxPG_LABEL, "null"))->ChangeFlag(wxPG_PROP_READONLY, true);
    m_pPropGrid->AppendIn(adapterProp, new wxStringProperty("DnsSuffix", wxPG_LABEL, "null"))->ChangeFlag(wxPG_PROP_READONLY, true);
    m_pPropGrid->AppendIn(adapterProp, new wxStringProperty("Description", wxPG_LABEL, "null"))->ChangeFlag(wxPG_PROP_READONLY, true);
    m_pPropGrid->AppendIn(adapterProp, new wxStringProperty("FriendlyName", wxPG_LABEL, "null"))->ChangeFlag(wxPG_PROP_READONLY, true);
    m_pPropGrid->AppendIn(adapterProp, new wxStringProperty("Physical Address", wxPG_LABEL, "null"))->ChangeFlag(wxPG_PROP_READONLY, true);

    this->SetSizerAndFit(sizer);
  }

  void DetailsPane::SetAdapterInfo(AdapterInfo &info)
  {
    m_pPropGrid->GetProperty("Index")->SetValue((long)info.index);
    m_pPropGrid->GetProperty("Name")->SetValue(info.name);
    m_pPropGrid->GetProperty("Unicast address")->SetValue(info.unicastAddress.ToString());
    m_pPropGrid->GetProperty("Anycast address")->SetValue(info.anycastAddress.ToString());
    m_pPropGrid->GetProperty("Multicast address")->SetValue(info.multicastAddress.ToString());
    m_pPropGrid->GetProperty("DnsServer address")->SetValue(info.dnsServerAddress.ToString());
    m_pPropGrid->GetProperty("Gateway address")->SetValue(info.gatewayAddress.ToString());
    m_pPropGrid->GetProperty("DnsSuffix")->SetValue(info.dnsSuffix);
    m_pPropGrid->GetProperty("Description")->SetValue(info.description);
    m_pPropGrid->GetProperty("FriendlyName")->SetValue(info.friendlyName);
    m_pPropGrid->GetProperty("Physical Address")->SetValue(info.address.ToString());
  }

}// namespace UI
}// namespace PacketHacker