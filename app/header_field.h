#pragma once

enum FieldType {
  FIELD_HARDWARE,
  FIELD_IP,
  FIELD_INT8,
  FIELD_INT16,
  FIELD_INT32,
  FIELD_INT64
};

using FieldData = std::variant<HardwareAddress, IPv4Address, uint8_t, uint16_t, uint32_t, uint64_t>;

class HeaderField
{
public:
  HeaderField(Packet *packet,
    const std::string name,
    const FieldData &defaultVal,
    const bool editable,
    const FieldType type)
    : m_packet(packet), m_name(std::move(name)), m_currentVal(defaultVal), m_editable(editable), m_type(type)
  {
  }

  virtual ~HeaderField() {}

  virtual void HandleData(const FieldData &data) = 0;

  Packet *GetPacket() const { return m_packet; }
  const std::string &GetName() const { return m_name; }
  const FieldData &GetCurrentVal() const { return m_currentVal; }
  const bool IsEditable() const { return m_editable; }
  const FieldType GetType() const { return m_type; }

  void SetValue(const FieldData &value)
  {
    m_currentVal = value;
  }

protected:
  Packet *m_packet;
  const std::string m_name;
  FieldData m_currentVal;
  const bool m_editable;
  const FieldType m_type;
};

template<class T>
class HeaderFieldImpl : public HeaderField
{
public:
  typedef void (T::*HandlerFunctionPtr)(const FieldData &);

  HeaderFieldImpl(T *packet,
    const std::string name,
    const FieldData &defaultVal,
    FieldType type,
    bool editable,
    HandlerFunctionPtr function)
    : HeaderField(packet, name, defaultVal, editable, type), m_function(function)
  {
  }

  virtual void HandleData(const FieldData &data)
  {
    T *packet = static_cast<T *>(m_packet);
    (packet->*m_function)(data);
  }

private:
  HandlerFunctionPtr m_function;
};

#define HEADER_FIELD_HARDWARE(classname, name, defaultVal, editable, function) \
  (new PacketHacker::HeaderFieldImpl<classname>(this, name, defaultVal, FieldType::FIELD_HARDWARE, editable, &classname::function))
#define HEADER_FIELD_IPv4(classname, name, defaultVal, editable, function) \
  (new PacketHacker::HeaderFieldImpl<classname>(this, name, defaultVal, FieldType::FIELD_IP, editable, &classname::function))
#define HEADER_FIELD_INT8(classname, name, defaultVal, editable, function) \
  (new PacketHacker::HeaderFieldImpl<classname>(this, name, (uint8_t)defaultVal, FieldType::FIELD_INT8, editable, &classname::function))
#define HEADER_FIELD_INT16(classname, name, defaultVal, editable, function) \
  (new PacketHacker::HeaderFieldImpl<classname>(this, name, (uint16_t)defaultVal, FieldType::FIELD_INT16, editable, &classname::function))
#define HEADER_FIELD_INT32(classname, name, defaultVal, editable, function) \
  (new PacketHacker::HeaderFieldImpl<classname>(this, name, (uint32_t)defaultVal, FieldType::FIELD_INT32, editable, &classname::function))
#define HEADER_FIELD_INT64(classname, name, defaultVal, editable, function) \
  (new PacketHacker::HeaderFieldImpl<classname>(this, name, (uint64_t)defaultVal, FieldType::FIELD_INT64, editable, &classname::function))