#ifndef _WIN32
#else

#include <winsock2.h>
#include <iphlpapi.h>

#endif

#include "utils.h"

namespace PacketHacker
{
namespace Utils
{
#ifdef _WIN32
std::vector<AdapterInfo> GetAdapters()
{
    std::vector<AdapterInfo> output;
    ULONG size = 0;
    PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
    PIP_ADAPTER_ADDRESSES pCurrAddresses = nullptr;

    ::GetAdaptersAddresses(AF_INET, 0, nullptr, nullptr, &size);
    pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(size);
    if (::GetAdaptersAddresses(AF_INET, 0, nullptr, pAddresses, &size) == NO_ERROR)
    {
        pCurrAddresses = pAddresses;
        while (pCurrAddresses)
        {
            AdapterInfo info{};
            info.name = "\\Device\\NPF_" + std::string(pCurrAddresses->AdapterName);
            info.friendlyName = pCurrAddresses->FriendlyName;
            memcpy(info.address, pCurrAddresses->PhysicalAddress, pCurrAddresses->PhysicalAddressLength);
            output.emplace_back(info);
            pCurrAddresses = pCurrAddresses->Next;
        }
    }

    if (pAddresses)
    {
        free(pAddresses);
    }
    return output;
}
#endif

} // namespace Utils
} // namespace PacketHacker
