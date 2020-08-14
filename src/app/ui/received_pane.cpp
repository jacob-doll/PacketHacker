#include "received_pane.h"

#include "packet/packet.h"

namespace PacketHacker {
namespace UI {

  ReceivedPane::ReceivedPane(Context *context,
    wxWindow *parent,
    wxWindowID winid,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name)
    : wxPanel(parent, winid, pos, size, style, name),
      m_pContext(context)
  {

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    m_pPropGrid = new wxPropertyGrid(this, wxID_ANY);

    sizer->Add(m_pPropGrid, 1, wxEXPAND);

    this->SetSizerAndFit(sizer);
  }

  void ReceivedPane::SetPacket(Packet *packet)
  {
    m_pPropGrid->Clear();
    Packet *currentPacket = packet;
    while (currentPacket) {
      wxPGProperty *currProp = m_pPropGrid->Append(new wxPropertyCategory(currentPacket->GetName()));
      for (HeaderField *field : currentPacket->GetFields()) {
        m_pPropGrid->AppendIn(currProp, new wxStringProperty(field->GetName(), wxPG_LABEL, field->GetCurrentVal()))->ChangeFlag(wxPG_PROP_READONLY, true);
        ;
      }

      currentPacket = currentPacket->GetInnerPacket();
    }
  }


  void ReceivedPane::OnPacketSent()
  {
    // if (!m_pContext->BeginStream()) {
    //   return;
    // }
    uint32_t size;
    const uint8_t *data;
    int counter = 0;
    int timeout = 5;

    while ((data = m_pContext->ReadNextPacket(&size))) {
      // if (counter >= timeout) {
      //   wxLogMessage("Reply timed out!");
      //   break;
      // }
      Packet *sent = m_pContext->GetBasePacket();
      if (sent->DoesReplyMatch(data, size)) {
        Packet *received = new EthernetPacket(data, size);
        SetPacket(received);
        delete received;
        break;
      }
      counter++;
    }

    // m_pContext->EndStream();
  }

}// namespace UI
}// namespace PacketHacker