#pragma once

#include <string>
#include <array>

#define PHYSICAL_ADDR_LEN 6

namespace PacketHacker {
namespace Utils {
  void stringToHardwareAddress(const std::string &hwAddress, uint8_t *buf, size_t bufSize);
}// namespace Utils

/**
 * \brief Class container for hardware addresses.
 * 
 * This class encapsulates hardware address functionality,
 * mostly for the ease of converting multiple types to a
 * byte array. 
 * 
 * There are several ways to instantiate a hardware address:\n
 *   - Integer parts: HardwareAddress hw(0xff, 0xff, 0xff, 0xff, 0xff, 0xff);\n
 *   - std::array: HardwareAddress hw = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};\n
 *   - From a string: HardwareAddress hw = "ff:ff:ff:ff:ff:ff";
 * 
 * You can also instantiate a hardware address from a c byte array.
 * 
 * When writing to a buffer use the ptr() function to access the pointer
 * to the first element.
 */
class HardwareAddress
{
public:
  /**
   * \brief Default constructor, initializes array with zeroes.
   */
  HardwareAddress();

  /**
   * \brief Initializes from integer parts.
   * @param b1 first byte
   * @param b2 second byte
   * @param b3 third byte
   * @param b4 fourth byte
   * @param b5 fifth byte
   * @param b6 sixth byte
   */
  HardwareAddress(const uint8_t b1,
    const uint8_t b2,
    const uint8_t b3,
    const uint8_t b4,
    const uint8_t b5,
    const uint8_t b6);

  /**
   * \brief Initializes from std::array.
   * @param data std::array containing hardware address data
   */
  HardwareAddress(const std::array<uint8_t, PHYSICAL_ADDR_LEN> &data);

  /**
   * \brief Initializes from std::string.
   * 
   * Must follow the format hx:hx:hx:hx:hx:hx
   * @param address std::string hardware address
   */
  HardwareAddress(const std::string &address);

  /**
   * \brief Initializes from a c byte array.
   * 
   * The size of the array must be PHYSICAL_ADDR_LEN.
   * @param data array of data containing hardware address
   */
  explicit HardwareAddress(const uint8_t *data);

  /**
   * \brief Returns address of first byte.
   * 
   * Useful for when writing the hardware address to buffer.
   * @return pointer to first byte of hardware address
   */
  const uint8_t *ptr() const { return m_data.data(); }

  /**
   * \brief Returns the array containing bytes for hardware address.
   * @return std::array of data
   */
  const std::array<uint8_t, PHYSICAL_ADDR_LEN> &data() const { return m_data; }

  /**
   * \brief Returns string representation of hardware address.
   * @return string representation of hardware address
   */
  std::string toString() const;

  /**
   * \brief Returns whether a hardware address string is valid.
   * 
   * If the string does not follow the format hx:hx:hx:hx:hx:hx, or
   * it does not have 6 bytes, then the function returns false.
   * @return true if correct format, false otherwise
   */
  static bool isHardwareAddressValid(const std::string &hwAddress);

  /**
   * \brief Returns boolean equivalence between two hardware addresses.
   * @return true if addresses are equal, false otherwise 
   */
  bool operator==(const HardwareAddress &rhs) const
  {
    return m_data == rhs.m_data;
  }

  /**
   * \brief Returns boolean negation of equality operation.
   * @return true if addresses are not equal, false otherwise
   */
  bool operator!=(const HardwareAddress &rhs) const
  {
    return !((*this) == rhs);
  }

  /**
   * \brief Prints the Hardware address to a std::ostream.
   * @param output stream to print to
   * @param hwAddress address to print
   * @return stream that was printed to
   */
  friend std::ostream &operator<<(std::ostream &output, const HardwareAddress &hwAddress);

private:
  std::array<uint8_t, PHYSICAL_ADDR_LEN> m_data;
};


}// namespace PacketHacker