#pragma once

#include <stdint.h>
#include <vector>

#include "packet/packet.h"
#include "packet/packets.h"
#include "packet/utils/utils.h"

namespace PacketHacker {

class Context
{
  friend class MainWindow;

public:
  Context(MainWindow *window);
  ~Context();

  void SetAdapter(int index);
  AdapterInfo GetAdapter() { return m_CurrentAdapter; }
  bool IsAdapterSet() const { return m_AdapterSet; }

  void SetBasePacket(int packetId);
  void AddPacket(int packetId);
  void RemovePacket(std::string name);
  Packet *GetBasePacket() const { return m_pBasePacket; }

  bool SendPacket();

  std::vector<AdapterInfo> &GetAdapters() { return m_adapters; }

  MainWindow *GetMainWindow() const { return m_MainWindow; }

private:
  MainWindow *m_MainWindow;

  AdapterInfo m_CurrentAdapter;
  bool m_AdapterSet;

  std::vector<AdapterInfo> m_adapters;
  Packet *m_pBasePacket;
};

}// namespace PacketHacker