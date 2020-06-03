#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include <functional>

namespace PacketHacker {

enum PacketTypes {
  ARP = 1000,
  ETHERNET = 1001
};

class HeaderField;

class Packet
{
public:
  Packet();
  ~Packet();

  Packet *GetOuterPacket() const { return m_outerPacket; }
  Packet *GetInnerPacket() const { return m_innerPacket; }
  Packet *GetPacket(std::string name);
  void SetInnerPacket(Packet *inner);
  void RemoveInnerPacket();
  uint32_t Size() const;
  void Init();

  void Parse(uint8_t *buffer, uint32_t size);
  void WriteToBuf(uint8_t *buffer, uint32_t size, uint32_t offset = 0);

  virtual uint32_t HeaderSize() const = 0;
  virtual std::string GetName() const = 0;
  virtual std::string ToString() = 0;

  HeaderField *GetField(std::string name) const;
  std::vector<HeaderField *> GetFields() const { return m_fields; }

protected:
  virtual void DoParse(uint8_t *buffer) = 0;
  virtual void DoWriteToBuf(uint8_t *buffer, uint32_t &offset) = 0;

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
  HeaderField(Packet *packet, std::string name, std::string defaultVal)
    : m_packet(packet), m_name(name), m_defaultVal(defaultVal)
  {
  }

  virtual ~HeaderField() {}

  virtual void HandleData(const char *data) = 0;

  Packet *GetPacket() const { return m_packet; }
  std::string GetName() const { return m_name; }
  std::string GetDefaultVal() const { return m_defaultVal; }

protected:
  Packet *m_packet;
  std::string m_name;
  std::string m_defaultVal;
};

template<class T>
class HeaderFieldImpl : public HeaderField
{
public:
  typedef void (T::*HandlerFunctionPtr)(const char *);

  HeaderFieldImpl(T *packet, std::string name, std::string defaultVal, HandlerFunctionPtr function)
    : HeaderField(packet, name, defaultVal), m_function(function)
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
