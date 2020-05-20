#ifndef _WIN32
#else

#include <winsock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

#endif

#include "utils.h"

#include <filesystem>

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
    
    uint32_t IPv4ToLong(const char *ipAddress)
    {
        int result;
        uint32_t IPv4Identifier = 0;
        struct in_addr addr;
        result = inet_pton(AF_INET, ipAddress, &(addr));
        if (result != 0) 
        {
            IPv4Identifier = ntohl(*((uint32_t *)&(addr)));
        }
        return IPv4Identifier;
    }

    uint64_t HardwareToLong(const char *hwAddress)
    {
        unsigned char a[6];
        int last = -1;
        int rc = sscanf(hwAddress, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%n",
                        a + 0, a + 1, a + 2, a + 3, a + 4, a + 5,
                        &last);
        if (rc != 6 || strlen(hwAddress) != last)
            return 0;

        return uint64_t(a[0]) << 40 |
                uint64_t(a[1]) << 32 |
                (uint32_t(a[2]) << 24 |
                uint32_t(a[3]) << 16 |
                uint32_t(a[4]) << 8 |
                uint32_t(a[5]));
    }

    std::vector<PacketInfo> GetAvailablePackets()
    {
        namespace fs = std::experimental::filesystem;
        std::vector<PacketInfo> packets;
        for (const auto & entry : fs::directory_iterator("data"))
        {
            PacketInfo info;
            info.name = entry.path().filename().string();
            info.path = entry.path().string();
            packets.push_back(info);
        }
        return packets;
    }

} // namespace Utils
} // namespace PacketHacker
