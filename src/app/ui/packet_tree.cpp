#include "packet_tree.h"

#include "app/main_window.h"
#include "packet/utils/adapter_utils.h"

namespace PacketHacker {
namespace UI {

  // HardwareAddressProperty
  // ------------------------------------------------------
  HardwareAddressProperty::HardwareAddressProperty(const wxString &label,
    const wxString &name,
    const HardwareAddress &value)
    : wxStringProperty(label, name, value.ToString())
  {
  }

  bool HardwareAddressProperty::ValidateValue(wxVariant &value,
    wxPGValidationInfo &validationInfo) const
  {
    return HardwareAddress::IsHardwareAddressValid(value.GetString().c_str().AsChar());
  }

  // ------------------------------------------------------

  // HardwareAddressProperty
  // ------------------------------------------------------
  IpAddressProperty::IpAddressProperty(const wxString &label,
    const wxString &name,
    const IPv4Address &value)
    : wxStringProperty(label, name, value.ToString())
  {
  }

  bool IpAddressProperty::ValidateValue(wxVariant &value,
    wxPGValidationInfo &validationInfo) const
  {
    return IPv4Address::IsIpv4AddressValid(value.GetString().c_str().AsChar());
  }

  // ------------------------------------------------------

  // PacketTree
  // ------------------------------------------------------
  PacketTree::PacketTree(Context *context,
    wxWindow *parent,
    wxWindowID winid,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name)
    : wxPanel(parent, winid, pos, size, style, name),
      m_pContext(context),
      m_pPropGrid()
  {
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    m_pPropGrid = new wxPropertyGrid(this, wxID_ANY);

    sizer->Add(m_pPropGrid, 1, wxEXPAND);

    this->Connect(wxEVT_PG_CHANGED, wxPropertyGridEventHandler(PacketTree::OnPropertyGridChanged));
    this->Connect(wxEVT_PG_RIGHT_CLICK, wxPropertyGridEventHandler(PacketTree::OnPropertyGridRightClicked));

    this->SetSizerAndFit(sizer);
  }

  void PacketTree::SetPacket(Packet *packet)
  {
    MainWindow *window = m_pContext->GetMainWindow();
    if (packet) {
      window->GetByteViewer()->SetSize(packet->Size());
      window->GetByteViewer()->Update(m_pContext->GetBasePacket());
      window->GetByteViewer()->Refresh();
    }

    m_pPropGrid->Clear();
    Packet *currentPacket = packet;
    while (currentPacket) {
      std::string name = currentPacket->GetName();
      wxPGProperty *currProp = m_pPropGrid->Append(new wxPropertyCategory(currentPacket->GetName()));
      for (HeaderField *field : currentPacket->GetFields()) {
        wxPGProperty *fieldProp;
        switch (field->GetType()) {
        case FieldType::FIELD_HARDWARE:
          fieldProp = new HardwareAddressProperty(field->GetName(), wxPG_LABEL, std::get<HardwareAddress>(field->GetCurrentVal()));
          break;
        case FieldType::FIELD_IP:
          fieldProp = new IpAddressProperty(field->GetName(), wxPG_LABEL, std::get<IPv4Address>(field->GetCurrentVal()));
          break;
        case FieldType::FIELD_INT8:
          fieldProp = new wxIntProperty(field->GetName(), wxPG_LABEL, std::get<uint8_t>(field->GetCurrentVal()));
          break;
        case FieldType::FIELD_INT16:
          fieldProp = new wxIntProperty(field->GetName(), wxPG_LABEL, std::get<uint16_t>(field->GetCurrentVal()));
          break;
        case FieldType::FIELD_INT32:
          fieldProp = new wxIntProperty(field->GetName(), wxPG_LABEL, std::get<uint32_t>(field->GetCurrentVal()));
          break;
        case FieldType::FIELD_INT64:
          fieldProp = new wxIntProperty(field->GetName(), wxPG_LABEL, std::get<uint64_t>(field->GetCurrentVal()));
          break;
        default:
          break;
        }
        fieldProp->Enable(field->IsEditable());
        m_pPropGrid->AppendIn(currProp, fieldProp);
      }

      currentPacket = currentPacket->GetInnerPacket();
    }
  }

  void PacketTree::Reload()
  {
    Packet *currentPacket = m_pContext->GetBasePacket();
    while (currentPacket) {
      for (HeaderField *field : currentPacket->GetFields()) {
        wxPGProperty *property = m_pPropGrid->GetProperty(field->GetName());
        // if (property->GetValue() != field->GetCurrentVal()) {
        //   property->SetValue(field->GetCurrentVal());
        // }
      }
      currentPacket = currentPacket->GetInnerPacket();
    }
  }

  void PacketTree::OnPropertyGridChanged(wxPropertyGridEvent &event)
  {
    std::string name = event.GetProperty()->GetName();
    std::string outerPropName = event.GetProperty()->GetParent()->GetName();
    MainWindow *window = m_pContext->GetMainWindow();
    const Packet *packet = m_pContext->GetBasePacket()->GetPacket(outerPropName);
    HeaderField *field = packet->GetField(name);

    switch (field->GetType()) {
    case FieldType::FIELD_HARDWARE:
      field->HandleData(HardwareAddress(event.GetValue().GetString().c_str().AsChar()));
      break;
    case FieldType::FIELD_IP:
      field->HandleData(IPv4Address(event.GetValue().GetString().c_str().AsChar()));
      break;
    case FieldType::FIELD_INT8:
      field->HandleData((uint8_t)event.GetValue().GetInteger());
      break;
    case FieldType::FIELD_INT16:
      field->HandleData((uint16_t)event.GetValue().GetInteger());
      break;
    case FieldType::FIELD_INT32:
      field->HandleData((uint32_t)event.GetValue().GetInteger());
      break;
    case FieldType::FIELD_INT64:
      field->HandleData((uint32_t)event.GetValue().GetInteger());
      break;
    default:
      break;
    }
    window->GetByteViewer()->Update(m_pContext->GetBasePacket());
    Reload();
  }

  void PacketTree::OnPopupClick(wxCommandEvent &event)
  {
    void *data = static_cast<wxMenu *>(event.GetEventObject())->GetClientData();
    wxPGProperty *property = static_cast<wxPGProperty *>(data);
    MainWindow *window = static_cast<MainWindow *>(this->GetParent());
    if (property->GetParent() != m_pPropGrid->GetRoot()) {
      property = property->GetParent();
    }

    if (event.GetId() == ID_REMOVE) {
      m_pContext->RemovePacket(property->GetName().ToStdString());
      this->SetPacket(m_pContext->GetBasePacket());
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

}// namespace UI
}// namespace PacketHacker
