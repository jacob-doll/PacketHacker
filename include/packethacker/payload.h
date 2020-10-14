#pragma once

#include <string>
#include <vector>

#include "common.h"

namespace PacketHacker {

/**
 * @brief Wrapper class for a payload that can be attached
 * to a packet.
 * 
 * Two types are currently supported: String data and Raw bytes.
 */
class Payload
{
public:
  /**
   * @brief The supported payload types.
   */
  enum PayloadType {
    STRING,
    RAW
  };

  /**
   * @brief Returns the payload type.
   * @return payload type
   */
  virtual const PayloadType type() const = 0;

  /**
   * @brief Returns the size of this payload in bytes.
   * @return size in bytes
   */
  virtual const SizeType payloadSize() const = 0;

  /**
   * @brief Writes the payload to a buffer.
   * @param buffer buffer to write to
   */
  virtual void write(DataType *buffer) = 0;
};

/**
 * @brief Wrapper for a payload of raw bytes.
 * @sa Payload
 */
class RawPayload : public Payload
{
public:
  /**
   * @brief Initializes an payload from byte data.
   * @param data data of payload
   * @param size size of payload in bytes
   */
  RawPayload(const DataType *data, SizeType size);

  /**
   * @brief Returns the payload type.
   * @return payload type
   * @sa Payload::type()
   */
  virtual const PayloadType type() const override { return PayloadType::RAW; }

  /**
   * @brief Returns the size of this payload in bytes.
   * @return size in bytes
   * @sa Payload::payloadSize()
   */
  virtual const SizeType payloadSize() const override;

  /**
   * @brief Writes the payload to a buffer.
   * @param buffer buffer to write to
   * @sa Payload::write()
   */
  virtual void write(DataType *buffer) override;

private:
  std::vector<DataType> m_payload;
};

/**
 * @brief Wrapper for a payload of string data.
 * @sa Payload
 */
class StringPayload : public Payload
{
public:
  /**
   * @brief Initializes a payload from string data.
   * @param payload string data
   */
  StringPayload(const std::string payload);

  /**
   * @brief Returns the payload type.
   * @return payload type
   * @sa Payload::type()
   */
  virtual const PayloadType type() const override { return PayloadType::STRING; }

  /**
   * @brief Returns the size of this payload in bytes.
   * @return size in bytes
   * @sa Payload::payloadSize()
   */
  virtual const SizeType payloadSize() const override;

  /**
   * @brief Writes the payload to a buffer.
   * @param buffer buffer to write to
   * @sa Payload::write()
   */
  virtual void write(DataType *buffer) override;

private:
  std::string m_payload;
};

}// namespace PacketHacker