#pragma once

#include <string>
#include <vector>
#include <stdint.h>

#include "hardware_address.h"
#include "ip_address.h"

namespace PacketHacker {

/**
 * \brief Struct representation of an interface entry on a local interface table.
 * 
 * Provides information about the ip addressing of an interface
 * as well as its name and description. Passed into a PacketStream 
 * to send and receive packets on an interface.
 */
struct Interface
{
  uint32_t index; /**< Integer index of the interface on the local machine. */
  HardwareAddress address; /**< Hardware address of the interface. */
  IPv4Address unicastAddress; /**< IPv4 address of the interface. */
  IPv4Address anycastAddress; /**< Anycast address of the interface if it exists. */
  IPv4Address multicastAddress; /**< Multicast address of the interface if it exists. */
  IPv4Address dnsServerAddress; /**< DNS server address of the interface if it exists. */
  IPv4Address gatewayAddress; /**< Gatewat address of the interface if it exists. */
  std::string name; /**< String name of the interface. */
  std::wstring dnsSuffix; /**< DNS suffix of the interface if it exists. */
  std::wstring description; /**< String description of the interface. */
  std::wstring friendlyName; /**< Easier to read name of the interface. */
};

/**
 * \brief Class representation of the interface table on a local machine.
 * 
 * This allows for the access of all interfaces that are on the local
 * machine, including virtual interfaces.
 * 
 * This class should not and cannot be initialized in your 
 * code. There should only ever be one instance.
 * 
 * To access the interface table call:
 *   - InterfaceTable table = InterfaceTable::instance();
 */
class InterfaceTable
{
public:
  /**
   * \brief Retrieves all local interfaces and adds them to a vector.
   * 
   * Only realistically needs to be called once, yet the functionality
   * is here if needed.
   */
  void refreshTable();

  /**
   * \brief Returns the best interface to send packets to a specific IP.
   * @param address IPv4 address of the machine to be sent to
   * @return interface pointer of the best interface or nullptr.
   */
  Interface *bestInterface(IPv4Address &address);

  /**
   * \brief Returns the interface with the given index.
   * @param ifIndex index of the interface to be returned
   * @return interface of given index or nullptr if none found
   */
  Interface *getInterface(const uint32_t ifIndex);

  /**
   * \brief Returns the vector of local interfaces retrieved.
   * @return std::vector of interfaces
   */
  std::vector<Interface> &interfaces() { return m_interfaces; }

  /**
   * \brief Returns the static instance of this class.
   * @return InterfaceTable instance
   */
  static InterfaceTable &instance()
  {
    static InterfaceTable instance;
    return instance;
  }

private:
  InterfaceTable();
  std::vector<Interface> m_interfaces;
};

}// namespace PacketHacker