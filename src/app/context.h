#pragma once

#include <stdint.h>
#include <vector>

#include "packet/packet.h"
#include "packet/packets.h"
#include "packet/utils/adapter_utils.h"

namespace PacketHacker {

class Adapter;
class Context
{
  friend class MainWindow;

public:
  Context(MainWindow *window);
  ~Context();

  void SetAdapter(std::string name);
  Adapter *GetAdapter() { return m_CurrentAdapter; }
  bool IsAdapterSet() const { return m_AdapterSet; }

  void SetBasePacket(int packetId);
  void AddPacket(int packetId);
  void RemovePacket(std::string name);
  Packet *GetBasePacket() const { return m_pBasePacket; }

  bool SendPacket();

  bool BeginStream();
  void EndStream();
  const uint8_t *ReadNextPacket(uint32_t *size);


  std::vector<AdapterInfo> &GetAdapters() { return m_adapters; }

  MainWindow *GetMainWindow() const { return m_MainWindow; }

private:
  MainWindow *m_MainWindow;

  std::vector<AdapterInfo> m_adapters;
  Adapter *m_CurrentAdapter;
  bool m_AdapterSet;

  Packet *m_pBasePacket;
};

}// namespace PacketHacker