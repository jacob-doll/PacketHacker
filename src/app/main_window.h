#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "context.h"
#include "ui/byte_viewer.h"

namespace PacketHacker
{
class MainWindow : public wxFrame
{
public:
    MainWindow();
    ~MainWindow();

    void OnButtonPressed(wxCommandEvent &event);
    void OnAdapterSelected(wxCommandEvent &event);

private:
    Context *m_pContext = nullptr;
    wxBoxSizer *m_pSizer = nullptr;
    wxButton *m_pSendButton = nullptr;
    wxMenuBar *m_pMenuBar = nullptr;
    wxMenu *m_pAdapterMenu = nullptr;
    UI::ByteViewer *m_pByteViewer = nullptr;
};

const int ID_SENDBUTTON = 101;

} // namespace PacketHacker