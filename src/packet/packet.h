#pragma once
#include <stdint.h>
#include <memory>
#include <vector>
#include <map>
#include <string>

namespace PacketHacker
{
    enum ElementType
    {
        NONE = 0,
        MAC = 1,
        IPv4 = 2,
        IPv6 = 3,
        INT = 4
    };

    struct ElementValue
    {
        std::string name;
        uint32_t val;
    };

    struct HeaderElement
    {
        std::string name;
        std::string defaultVal;
        std::vector<ElementValue> values;
        uint32_t elementType;
        uint32_t size;
        uint32_t offset;
    };

    class Packet
    {
    public:
        Packet();
        ~Packet();

        uint32_t HeaderSize() const { return m_headerSize; }
        uint32_t PayloadSize() const;
        uint32_t Size() const;

        Packet* GetOuterPacket() const { return m_outerPacket; }
        Packet* GetInnerPacket() const { return m_innerPacket; }
        Packet* GetPacket(std::string name);
        void SetInnerPacket(Packet* inner);
        void RemoveInnerPacket();

        void WriteToBuf(uint8_t *buffer, uint32_t size, uint32_t offset = 0);
        void DoWriteToBuf(uint8_t *buffer, uint32_t &offset);

        std::string GetName() const { return m_name; }
        HeaderElement GetField(std::string name) const;
        std::vector<HeaderElement> GetFields() const { return m_headerElements; }

        bool LoadPacket(const char *filename);

    private:
        void SetOuterPacket(Packet* outer);

    private:
        Packet* m_outerPacket;
        Packet* m_innerPacket;
        uint32_t m_headerSize;
        std::string m_name;
        std::vector<HeaderElement> m_headerElements;
    };

} // namespace PacketHacker
