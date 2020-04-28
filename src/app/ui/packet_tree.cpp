#include "packet_tree.h"

namespace PacketHacker
{
namespace UI
{

PacketTree::PacketTree(wxWindow *parent,
                       wxWindowID winid,
                       const wxPoint &pos,
                       const wxSize &size,
                       long style,
                       const wxString &name)
    : wxPanel(parent, winid, pos, size, style, name)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    m_pPropGrid = new wxPropertyGrid(this, wxID_ANY);

    // One way to add category (similar to how other properties are added)
    // m_pPropGrid->Append(new wxPropertyCategory("Main"));
    // // All these are added to "Main" category
    // m_pPropGrid->Append(new wxStringProperty("Name"));
    // m_pPropGrid->Append(new wxIntProperty("Age", wxPG_LABEL, 25));
    // m_pPropGrid->Append(new wxIntProperty("Height", wxPG_LABEL, 180));
    // m_pPropGrid->Append(new wxIntProperty("Weight"));
    // // Another one
    // m_pPropGrid->Append(new wxPropertyCategory("Attributes"));
    // // All these are added to "Attributes" category
    // m_pPropGrid->Append(new wxIntProperty("Intelligence"));
    // m_pPropGrid->Append(new wxIntProperty("Agility"));
    // m_pPropGrid->Append(new wxIntProperty("Strength"));

    sizer->Add(m_pPropGrid, 1, wxEXPAND);

    this->SetSizerAndFit(sizer);
}

void PacketTree::SetPacket(Packet *packet)
{
    delete m_pBasePacket;
    m_pBasePacket = packet;
    if (!m_pBasePacket)
        return;

    m_pPropGrid->Clear();
    Packet *currentPacket = m_pBasePacket;
    while (currentPacket)
    {
        wxPGProperty *currProp = m_pPropGrid->Append(new wxPropertyCategory(currentPacket->GetName()));

        for (std::string field : currentPacket->GetFields())
        {
            m_pPropGrid->AppendIn(currProp, new wxStringProperty(field, wxPG_LABEL, "TEST"));
        }

        currentPacket = currentPacket->GetInnerPacket();
        // wxLogMessage("Current packet:%d", (int)currentPacket);
    }
}

} // namespace UI
} // namespace PacketHacker
