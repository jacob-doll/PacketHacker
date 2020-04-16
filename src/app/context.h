#pragma once

#include <stdint.h>

#include "packet/utils/utils.h"

namespace PacketHacker
{

class Context
{
public:
    Context();

    void SetAdapter(AdapterInfo &info);
    AdapterInfo GetAdapter() { return m_CurrentAdapter; }

    bool IsAdapterSet() const { return m_AdapterSet; }

    bool SendPacket(uint8_t *data, int size);

private:
    AdapterInfo m_CurrentAdapter;
    bool m_AdapterSet = false;
};

} // namespace PacketHacker