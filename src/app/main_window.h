#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "packet/packet.h"
#include "context.h"
#include "ui/byte_viewer.h"
#include "ui/packet_tree.h"

namespace PacketHacker
{
class MainWindow : public wxFrame
{
public:
    MainWindow();
    ~MainWindow();

    void OnButtonPressed(wxCommandEvent &event);
    void OnAdapterSelected(wxCommandEvent &event);
    void OnPacketSelected(wxCommandEvent &event);

    UI::ByteViewer *GetByteViewer() const { return m_pByteViewer; }
    Context *GetContext() const { return m_pContext; }

private:
    Context *m_pContext = nullptr;

    wxBoxSizer *m_pSizer = nullptr;
    wxButton *m_pSendButton = nullptr;
    wxMenuBar *m_pMenuBar = nullptr;
    wxMenu *m_pAdapterMenu = nullptr;
    wxMenu *m_pPacketMenu = nullptr;

    UI::PacketTree *m_pPacketTree = nullptr;
    UI::ByteViewer *m_pByteViewer = nullptr;
};

const int ID_SENDBUTTON = 101;

} // namespace PacketHacker