#include "main.h"
#include "app/main_window.h"
#include "packet/packet.h"
#include <cstdio>

IMPLEMENT_APP(PacketHackerApp)

bool PacketHackerApp::OnInit()
{
  using namespace PacketHacker;

  MainWindow *window = new MainWindow();
  window->Center();
  window->Show();


  return true;
}