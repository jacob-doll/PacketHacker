#pragma once

#include <string>
#include <vector>

#define PHYSICAL_ADDR_LEN 6

namespace PacketHacker
{

struct AdapterInfo
{
    std::string name;
    std::wstring friendlyName;
    uint8_t address[PHYSICAL_ADDR_LEN];
};

struct PacketInfo
{
    std::string name;
    std::string path;
};

namespace Utils
{

std::vector<AdapterInfo> GetAdapters();

uint64_t HardwareToLong(const char* hwAddress);
uint32_t IPv4ToLong(const char* ipAddress);

std::vector<PacketInfo> GetAvailablePackets();

} // namespace Utils
} // namespace PacketHacker