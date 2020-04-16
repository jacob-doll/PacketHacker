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
    u_char address[PHYSICAL_ADDR_LEN];
};
namespace Utils
{

std::vector<AdapterInfo> GetAdapters();

} // namespace Utils
} // namespace PacketHacker