#include "stream_pane.h"

#include "packet/packet.h"

namespace PacketHacker {
namespace UI {

  StreamPane::StreamPane(Context *context,
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

    m_pList = new wxListBox(this, wxID_ANY);

    sizer->Add(m_pList, 1, wxEXPAND);

    this->SetSizerAndFit(sizer);
  }

  void StreamPane::OnPacketSent()
  {
    if (!m_pContext->BeginStream()) {
      return;
    }
    uint32_t size;
    const uint8_t *data;
    int counter = 0;
    int timeout = 5;

    while ((data = m_pContext->ReadNextPacket(&size))) {
      if (counter >= timeout) break;
      Packet *sent = m_pContext->GetBasePacket();
      if (sent->DoesReplyMatch(data, size)) {
        Packet *recieved = new EthernetPacket(data, size);

        while (recieved) {
          for (HeaderField *field : recieved->GetFields()) {
            m_pList->Append(field->GetCurrentVal());
          }
          recieved = recieved->GetInnerPacket();
        }
        delete recieved;
        break;
      }
      counter++;
    }

    m_pContext->EndStream();
  }

}// namespace UI
}// namespace PacketHacker