#pragma once
#include <stdint.h>
#include <vector>
#include <string>

#include "hardware_address.h"
#include "ip_address.h"

namespace PacketHacker {

enum PacketType {
  ARP = 1000,
  ETHERNET,
  IP,
  ICMP,
  DATA,
  UDP
};

enum FieldType {
  FIELD_HARDWARE,
  FIELD_IP,
  FIELD_INT
};

class HeaderField;

class Packet
{
public:
  Packet();
  virtual ~Packet();

  Packet *GetOuterPacket() const { return m_outerPacket; }
  Packet *GetInnerPacket() const { return m_innerPacket; }
  Packet *GetPacket(const std::string &name);
  void SetInnerPacket(Packet *inner);
  void RemoveInnerPacket();
  const uint32_t Size() const;
  void Init();

  void WriteToBuf(uint8_t *buffer, const uint32_t size);

  virtual PacketType GetPacketType() const = 0;
  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) = 0;
  virtual uint32_t HeaderSize() const = 0;
  virtual std::string GetName() const = 0;

  HeaderField *GetField(const std::string &name) const;
  std::vector<HeaderField *> GetFields() const { return m_fields; }

  std::string ToString() const;

protected:
  virtual void DoWriteToBuf(uint8_t *buffer) = 0;

private:
  void SetOuterPacket(Packet *outer);

protected:
  std::vector<HeaderField *> m_fields;

private:
  Packet *m_outerPacket;
  Packet *m_innerPacket;
};

class HeaderField
{
public:
  HeaderField(Packet *packet, const std::string name, const std::string defaultVal, const bool editable, const FieldType type)
    : m_packet(packet), m_name(std::move(name)), m_defaultVal(defaultVal), m_currentVal(defaultVal), m_editable(editable), m_type(type)
  {
  }

  virtual ~HeaderField() {}

  virtual void HandleData(const char *data) = 0;

  Packet *GetPacket() const { return m_packet; }
  const std::string &GetName() const { return m_name; }
  const std::string &GetDefaultVal() const { return m_defaultVal; }
  const std::string &GetCurrentVal() const { return m_currentVal; }
  const bool IsEditable() const { return m_editable; }
  const FieldType GetType() const { return m_type; }

  void SetValue(const char *value)
  {
    m_currentVal = value;
  }

protected:
  Packet *m_packet;
  const std::string m_name;
  const std::string m_defaultVal;
  std::string m_currentVal;
  const bool m_editable;
  const FieldType m_type;
};

template<class T>
class HeaderFieldImpl : public HeaderField
{
public:
  typedef void (T::*HandlerFunctionPtr)(const char *);

  HeaderFieldImpl(T *packet, std::string name, std::string defaultVal, HandlerFunctionPtr function, bool editable = true, FieldType type = FieldType::FIELD_INT)
    : HeaderField(packet, name, defaultVal, editable, type), m_function(function)
  {
  }

  virtual void HandleData(const char *data)
  {
    T *packet = static_cast<T *>(m_packet);
    (packet->*m_function)(data);
  }

private:
  HandlerFunctionPtr m_function;
};

}// namespace PacketHacker
