#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include <functional>

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
  Packet *GetPacket(std::string name);
  void SetInnerPacket(Packet *inner);
  void RemoveInnerPacket();
  uint32_t Size() const;
  void Init();

  void WriteToBuf(uint8_t *buffer, uint32_t size);

  virtual PacketType GetPacketType() const = 0;
  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) = 0;
  virtual uint32_t HeaderSize() const = 0;
  virtual std::string GetName() const = 0;

  HeaderField *GetField(std::string name) const;
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
  HeaderField(Packet *packet, std::string name, std::string defaultVal, bool editable, FieldType type)
    : m_packet(packet), m_name(name), m_defaultVal(defaultVal), m_currentVal(defaultVal), m_editable(editable), m_type(type)
  {
  }

  virtual ~HeaderField() {}

  virtual void HandleData(const char *data) = 0;

  Packet *GetPacket() const { return m_packet; }
  std::string GetName() const { return m_name; }
  std::string GetDefaultVal() const { return m_defaultVal; }
  std::string GetCurrentVal() const { return m_currentVal; }
  bool IsEditable() { return m_editable; }
  FieldType GetType() { return m_type; }

  void SetValue(const char *value)
  {
    m_currentVal = value;
  }

protected:
  Packet *m_packet;
  std::string m_name;
  std::string m_currentVal;
  std::string m_defaultVal;
  bool m_editable;
  FieldType m_type;
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
