#pragma once

#include <stdint.h>
#include <vector>

#include "packet/packet.h"
#include "packet/utils/utils.h"

namespace PacketHacker
{

class Context
{
public:
    Context();
    ~Context();

    void SetAdapter(int index);
    AdapterInfo GetAdapter() { return m_CurrentAdapter; }
    bool IsAdapterSet() const { return m_AdapterSet; }

    void SetBasePacket(int index);
    void AddPacket(int index);
    void RemovePacket(std::string name);
    Packet* GetBasePacket() const { return m_pBasePacket; }

    bool SendPacket(uint8_t *data, int size);

    std::vector<PacketInfo>& GetPacketTypes() { return m_packetTypes; }
    std::vector<AdapterInfo>& GetAdapters() { return m_adapters; }

private:
    AdapterInfo m_CurrentAdapter;
    bool m_AdapterSet;

    std::vector<PacketInfo> m_packetTypes;
    std::vector<AdapterInfo> m_adapters;
    Packet *m_pBasePacket;
};

} // namespace PacketHacker