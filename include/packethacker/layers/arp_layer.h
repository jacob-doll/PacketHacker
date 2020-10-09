#pragma once

#include "hardware_address.h"
#include "ip_address.h"
#include "utils/buffer_utils.h"

#include "layer.h"

namespace PacketHacker {

/**
 * @brief Class representation of an ARP layer.
 */
class ArpLayer : public Layer
{
public:
  /**
   * Declares the layer type of this layer.
   */
  DECL_LAYER(LayerType::ARP);

  /**
   * @brief Default constructor.
   * 
   * Initializes header with zero values.
   */
  ArpLayer();

  /**
   * @brief Initializes ARP layer from a data buffer.
   * 
   * If size is less than the sizeof(ArpHeader) then an
   * error will be thrown. 
   * 
   * The layer will only read the first sizeof(ArpHeader) bytes
   * from the buffer, the rest is ignored.
   * @param data buffer to read from
   * @param size size of buffer
   */
  ArpLayer(const uint8_t *data, uint32_t size);

  /**
   * @brief Returns the hardware type.
   * @return hardware type
   */
  uint16_t hardwareType() { return BYTE_SWAP_16(m_header.hardwareType); }

  /**
   * @brief Returns the protocol type.
   * @return protocol type
   */
  uint16_t protocolType() { return BYTE_SWAP_16(m_header.protocolType); }

  /**
   * @brief Returns the hardware length.
   * @return hardware length
   */
  uint8_t hardwareLength() { return m_header.hardwareLength; }

  /**
   * @brief Returns the protocol length.
   * @return protocol length
   */
  uint8_t protocolLength() { return m_header.protocolLength; }

  /**
   * @brief Returns the operation code.
   * @return operation code
   */
  uint16_t opcode() { return BYTE_SWAP_16(m_header.opcode); }

  /**
   * @brief Returns the sender hardware address.
   * @return sender hardware address
   */
  HardwareAddress senderMac() { return HardwareAddress(m_header.senderMac); }

  /**
   * @brief Returns the sender IP address.
   * @return sender IP address
   */
  IPv4Address senderIp() { return IPv4Address(m_header.senderIp); }

  /**
   * @brief Returns the target hardware address.
   * @return target hardware address
   */
  HardwareAddress targetMac() { return HardwareAddress(m_header.targetMac); }

  /**
   * @brief Returns the target IP address.
   * @return target IP address
   */
  IPv4Address targetIp() { return IPv4Address(m_header.targetIp); }

  /**
   * @brief Sets the hardware type.
   * @param hardwareType hardware type
   */
  void hardwareType(const uint16_t hardwareType);

  /**
   * @brief Sets the protocol type.
   * @param protocolType protocol type
   */
  void protocolType(const uint16_t protocolType);

  /**
   * @brief Sets the hardware length.
   * @param hardwareLength hardware length
   */
  void hardwareLength(const uint8_t hardwareLength);

  /**
   * @brief Sets the protocol length.
   * @param protocolLength protocol length
   */
  void protocolLength(const uint8_t protocolLength);

  /**
   * @brief Sets the operation code.
   * @param opcode operation code
   */
  void opcode(const uint16_t opcode);

  /**
   * @brief Sets the sender hardware address.
   * @param senderMac sender hardware address
   */
  void senderMac(const HardwareAddress &senderMac);

  /**
   * @brief Sets the sender IP address
   * @param senderIp sender IP address
   */
  void senderIp(const IPv4Address &senderIp);

  /**
   * @brief Sets the target hardware address.
   * @param targetMac target hardware address
   */
  void targetMac(const HardwareAddress &targetMac);

  /**
   * @brief Sets the target IP address
   * @param targetIp target IP address
   */
  void targetIp(const IPv4Address &targetIp);

  /**
   * @brief Returns the layer's type.
   * @return LayerType::ARP
   * @sa Layer::type()
   */
  virtual const LayerType type() const override { return typeFlag; }

  /**
   * @brief Returns the header size of the layer.
   * @return sizeof(ArpHeader)
   * @sa Layer::headerSize()
   */
  virtual const uint32_t headerSize() const override { return sizeof(ArpHeader); }

  /**
   * @brief Returns if a buffer contains the reply to this layer.
   * 
   * Size must be at least sizeof(ArpHeader)
   * @param buffer data of the reply
   * @param size size of the reply buffer
   * @return true if the buffer contains a reply, false otherwise
   * @sa Layer::isReply()
   */
  virtual bool isReply(const uint8_t *buffer, uint32_t size) override;

  /**
   * @brief Writes the contents of this layer to the buffer.
   * @param buffer data buffer to write to
   * @sa Layer::write()
   */
  virtual void write(uint8_t *buffer) override;

private:
#pragma pack(push, 1)
  struct ArpHeader
  {
    uint16_t hardwareType;
    uint16_t protocolType;
    uint8_t hardwareLength;
    uint8_t protocolLength;
    uint16_t opcode;
    uint8_t senderMac[6];
    uint32_t senderIp;
    uint8_t targetMac[6];
    uint32_t targetIp;
  };
#pragma pack(pop)

  ArpHeader m_header;
};

}// namespace PacketHacker