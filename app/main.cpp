#include "main.h"
#include "app/main_window.h"

DECLARE_APP(PacketHackerApp);
IMPLEMENT_APP(PacketHackerApp);

bool PacketHackerApp::OnInit()
{
  using namespace PacketHacker;

  MainWindow *window = new MainWindow();
  SetTopWindow(window);
  window->Center();
  window->Show();

  return true;
}
