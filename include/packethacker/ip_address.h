#pragma once

#include <string>
#include <array>

namespace PacketHacker {
namespace Utils {
  uint32_t stringToIPv4(const std::string &ipv4Address);
}// namespace Utils

/**
 * @brief Class container for an IPv4 address.
 * 
 * This class encapuslates IP address creation and manipulation. Several
 * useful operations such as bit mask operators are supplied to help
 * with subnetting and masking.
 * 
 * There are several ways to instantiate an IPv4 address:@n
 *   - Integer parts: IPv4Address ip(192, 168, 1, 1);@n
 *   - std::array: IPv4Address ip = { 192, 168, 1, 1 };@n
 *   - From a string: IPv4Address ip = "192.168.1.1";
 *   - You can also instantiate an IP address from an integer.
 * 
 * When writing to a buffer the data() will return an integer value for the
 * IP address.
 */
class IPv4Address
{
public:
  /**
   * @brief Default constructor, initializes ip to 0.0.0.0.
   */
  IPv4Address();

  /**
   * @brief Initializes from integer parts.
   * @param b1 first byte
   * @param b2 second byte
   * @param b3 third byte
   * @param b4 fourth byte
   */
  IPv4Address(const uint8_t b1, const uint8_t b2, const uint8_t b3, const uint8_t b4);

  /**
   * @brief Initializes from std::array.
   * @param data 4 byte array of data
   */
  IPv4Address(const std::array<uint8_t, 4> &data);

  /**
   * @brief Initializes from std::string.
   * 
   * Must follow the format x.x.x.x
   * @param address string containing IP
   */
  IPv4Address(const std::string &address);

  /**
   * @brief Initializes from a 32-bit integer.
   * @param address integer value of IP
   */
  explicit IPv4Address(const uint32_t address);

  /**
   * @brief Returns the integer representation of the IP address.
   * @return 32-bit integer value of IP
   */
  const uint32_t data() const { return m_data; }

  /**
   * @brief Returns string representation of the IP address.
   * @return string value of IP
   */
  std::string toString() const;

  /**
   * @brief Returns whether an IPv4 address string is valid.
   * 
   * If the string follows the format x.x.x.x then true is returned.
   * @return true if format correct, false otherwise
   */
  static bool isIpv4AddressValid(const std::string &ipAddress);

  /**
   * @brief Returns if another IP address is less than this one.
   * @return true if less than, false otherwise
   */
  bool operator<(const IPv4Address &rhs) const
  {
    return m_data < rhs.m_data;
  }

  /**
   * @brief Returns if another IP address is less or equal than this one.
   * @return true if less or equal to, false otherwise
   */
  bool operator<=(const IPv4Address &rhs) const
  {
    return !operator>(rhs);
  }

  /**
   * @brief Returns if another IP address is greater than this one.
   * @return true if greater than, false otherwise
   */
  bool operator>(const IPv4Address &rhs) const
  {
    return m_data > rhs.m_data;
  }

  /**
   * @brief Returns if another IP address is greater or equal than this one.
   * @return true if greater or equal to, false otherwise
   */
  bool operator>=(const IPv4Address &rhs) const
  {
    return !operator<(rhs);
  }

  /**
   * @brief Returns whether another IP address is equal to this one.
   * @return true if equal, false otherwise
   */
  bool operator==(const IPv4Address &rhs) const
  {
    return m_data == rhs.m_data;
  }

  /**
   * @brief Returns whether another IP address is not equal to this one.
   * @return true if not equal, false otherwise
   */
  bool operator!=(const IPv4Address &rhs) const
  {
    return !((*this) == rhs);
  }

  /**
   * @brief Returns the bitwise and operation of this IP and a mask.
   * @return IP address that has been anded with mask
   */
  IPv4Address operator&(const IPv4Address &mask) const;

  /**
   * @brief Returns the bitwise or operation of this IP and a mask.
   * @return IP address that has been ored with mask
   */
  IPv4Address operator|(const IPv4Address &mask) const;

  /**
   * @brief Prints the IP address to a std::ostream.
   * @param output stream to print to
   * @param ipAddress address to print
   * @return stream that was printed to
   */
  friend std::ostream &operator<<(std::ostream &output, const IPv4Address &ipAddress);

private:
  union {
    struct
    {
      uint8_t m_b1;
      uint8_t m_b2;
      uint8_t m_b3;
      uint8_t m_b4;
    };
    uint32_t m_data;
  };
};


}// namespace PacketHacker