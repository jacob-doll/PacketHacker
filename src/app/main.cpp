#include "main.h"
#include "main_window.h"

IMPLEMENT_APP(PacketHackerApp)

bool PacketHackerApp::OnInit()
{
    using namespace PacketHacker;

    MainWindow *window = new MainWindow();
    window->Center();
    window->Show();
    return true;
}