#include "packet_tree.h"
#include "../main_window.h"

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
        : wxPanel(parent, winid, pos, size, style, name),
          m_pPropGrid()
    {
        wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

        m_pPropGrid = new wxPropertyGrid(this, wxID_ANY);

        sizer->Add(m_pPropGrid, 1, wxEXPAND);

        this->Connect(wxEVT_PG_CHANGED, wxPropertyGridEventHandler(PacketTree::OnPropertyGridChanged));
        this->Connect(wxEVT_PG_RIGHT_CLICK, wxPropertyGridEventHandler(PacketTree::OnPropertyGridRightClicked));

        this->SetSizerAndFit(sizer);
    }

    void PacketTree::SetPacket(Packet* packet)
    {
        MainWindow *window = static_cast<MainWindow *>(this->GetParent());
        if (packet) window->GetByteViewer()->SetSize(packet->Size());

        m_pPropGrid->Clear();
        Packet *currentPacket = packet;
        while (currentPacket)
        {
            wxPGProperty *currProp = m_pPropGrid->Append(new wxPropertyCategory(currentPacket->GetName()));
            for (HeaderElement field : currentPacket->GetFields())
            {
                if (field.values.size() > 0)
                {
                    wxArrayString arrTypes;
                    wxArrayInt arrVals;
                    for (ElementValue value : field.values)
                    {
                        arrTypes.Add(value.name);
                        arrVals.Add(value.val);
                    }
                    wxEnumProperty *enumProp = new wxEnumProperty(field.name, wxPG_LABEL, arrTypes, arrVals, field.values[0].val);
                    m_pPropGrid->AppendIn(currProp, enumProp);
                    window->GetByteViewer()->WriteInt(field.offset, field.values[0].val, field.size);
                }
                else
                {
                    uint64_t value = 0;
                    switch (field.elementType)
                    {
                    case ElementType::MAC:
                        value = Utils::HardwareToLong(field.defaultVal.c_str());
                        break;
                    case ElementType::IPv4:
                        value = Utils::IPv4ToLong(field.defaultVal.c_str());
                    }

                    m_pPropGrid->AppendIn(currProp, new wxStringProperty(field.name, wxPG_LABEL, field.defaultVal));
                    window->GetByteViewer()->WriteInt(field.offset, value, field.size);
                }
            }

            currentPacket = currentPacket->GetInnerPacket();
        }
    }

    void PacketTree::OnPropertyGridChanged(wxPropertyGridEvent &event)
    {
        std::string name = event.GetProperty()->GetName();
        std::string outerPropName = event.GetProperty()->GetParent()->GetName();
        MainWindow *window = static_cast<MainWindow *>(this->GetParent());
        const Packet* packet = window->GetContext()->GetBasePacket()->GetPacket(outerPropName);
        HeaderElement element = packet->GetField(name);

        uint64_t value = 0;
        switch (element.elementType)
        {
        case ElementType::NONE:
            value = event.GetValue().GetInteger();
            break;
        case ElementType::MAC:
            value = Utils::HardwareToLong(event.GetValue().GetString().c_str());
            break;
        case ElementType::IPv4:
            value = Utils::IPv4ToLong(event.GetValue().GetString().c_str());
        }

        window->GetByteViewer()->WriteInt(element.offset, value, element.size);
    }

    void PacketTree::OnPopupClick(wxCommandEvent &event)
    {
        void *data = static_cast<wxMenu *>(event.GetEventObject())->GetClientData();
        wxPGProperty *property = static_cast<wxPGProperty *>(data);
        MainWindow *window = static_cast<MainWindow *>(this->GetParent());
        if (property->GetParent() != m_pPropGrid->GetRoot())
        {
            property = property->GetParent();
        }

        if (event.GetId() == ID_REMOVE)
        {
            window->GetContext()->RemovePacket(property->GetName().ToStdString());
            this->SetPacket(window->GetContext()->GetBasePacket());
        }
    }

    void PacketTree::OnPropertyGridRightClicked(wxPropertyGridEvent &event)
    {
        void *data = reinterpret_cast<void *>(event.GetProperty());
        wxMenu mnu;
        mnu.SetClientData(data);
        mnu.SetTitle("Edit");
        mnu.Append(ID_REMOVE, "Remove");
        mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(PacketTree::OnPopupClick), NULL, this);
        PopupMenu(&mnu);
    }

} // namespace UI
} // namespace PacketHacker
