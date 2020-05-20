#include "packet.h"

#include <tinyxml2.h>
#include <wx/wx.h>

namespace PacketHacker
{

    Packet::Packet()
        : m_innerPacket(), m_outerPacket(), m_headerSize(), m_name()
    {
    }

    Packet::~Packet()
    {
        delete m_innerPacket;
    }

    bool Packet::LoadPacket(const char *filename)
    {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(filename);
        if (!doc.Error())
        {
            tinyxml2::XMLNode *root = doc.FirstChild();
            if (root == nullptr)
            {
                wxLogMessage("First child was null");
                return false;
            }
            m_name = root->FirstChildElement("name")->GetText();
            tinyxml2::XMLElement *header = root->FirstChildElement("header");
            m_headerSize = std::stoi(header->Attribute("size"));
            tinyxml2::XMLElement *headerElement = header->FirstChildElement("element");
            uint32_t offset = 0;
            if (m_outerPacket != nullptr)
            {
                offset = m_outerPacket->HeaderSize();
            }
            while (headerElement != nullptr)
            {
                HeaderElement hElement;
                hElement.name = headerElement->Attribute("name");
                hElement.size = std::stoi(headerElement->Attribute("size"));
                hElement.offset = offset;

                const tinyxml2::XMLAttribute *defaultAttr = headerElement->FindAttribute("default");
                if (defaultAttr)
                {
                    hElement.defaultVal = defaultAttr->Value();
                }

                const tinyxml2::XMLAttribute *typeAttr = headerElement->FindAttribute("type");
                if (typeAttr)
                {
                    hElement.elementType = typeAttr->IntValue();
                }

                tinyxml2::XMLElement *typeElement = headerElement->FirstChildElement("type");
                while (typeElement != nullptr)
                {
                    const char *name = typeElement->Attribute("name");
                    uint32_t val = std::stoi(typeElement->Attribute("val"), 0, 16);
                    ElementValue type = {name, val};
                    hElement.values.emplace_back(type);
                    typeElement = typeElement->NextSiblingElement("type");
                }
                m_headerElements.emplace_back(hElement);
                offset += hElement.size;
                headerElement = headerElement->NextSiblingElement("element");
            }
        }
        else
        {
            wxLogMessage("Ran into error while reading %s: %s", filename, doc.ErrorName());
            return false;
        }
        return true;
    }

    uint32_t Packet::PayloadSize() const
    {
        return 0;
    }

    uint32_t Packet::Size() const
    {
        uint32_t size = HeaderSize() + PayloadSize();
        if (m_innerPacket)
        {
            size += m_innerPacket->Size();
        }
        return size;
    }

    Packet* Packet::GetPacket(std::string name) 
    {
        Packet* packet = this;
        while (packet) 
        {
            if (packet->GetName() == name)
            {
                return packet;
            }
            packet = packet->GetInnerPacket();
        }
        return nullptr;
    }

    void Packet::SetInnerPacket(Packet* inner)
    {
        delete m_innerPacket;
        m_innerPacket = inner;
        if (m_innerPacket)
        {
            m_innerPacket->SetOuterPacket(this);
        }
    }

    void Packet::RemoveInnerPacket() {
        delete m_innerPacket;
        m_innerPacket = nullptr;
    }

    void Packet::SetOuterPacket(Packet* outer)
    {
        m_outerPacket = outer;
    }

    HeaderElement Packet::GetField(std::string name) const
    {
        for (HeaderElement field : m_headerElements)
        {
            if (field.name == name)
            {
                return field;
            }
        }
        return {};
    }

    void Packet::WriteToBuf(uint8_t *buffer, uint32_t size, uint32_t offset)
    {
        uint32_t packetSize = this->Size();
        if (size < packetSize)
            return;
        uint32_t currentOffset = offset;
        DoWriteToBuf(buffer, currentOffset);
        if (m_innerPacket)
            m_innerPacket->WriteToBuf(buffer, packetSize, currentOffset);
    }

    void Packet::DoWriteToBuf(uint8_t *buffer, uint32_t &offset)
    {
    }

} // namespace PacketHacker