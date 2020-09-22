#pragma once

#include "hardware_address.h"
#include "ip_address.h"

#include <string>
#include <vector>

namespace PacketHacker {

/**
 * \brief Struct representation of an ARP entry on an local ARP table.
 * 
 * The arp table maps hardware addresses to there IP address so
 * that ethernet frames can be created.
 */
struct ArpEntry
{
  HardwareAddress hwAddress; /**< Hardware address of the entry. */
  IPv4Address ipAddress; /**< IPv4 address of the entry. */
  uint32_t ifIndex; /**< Interface index that the entry applies to. */
};

/**
 * \brief Class representation of the ARP table of the local machine.
 * 
 * This class allows for manipulation of the ARP table on the local
 * machine. Sufficient priviledges are needed for flushTable(),
 * addEntry(), and deleteEntry().
 * 
 * This class should not and cannot be initialized in your code. There
 * should only ever be one instance.
 * 
 * To access the ARP table call:
 *   - ArpTable table = ArpTable::instance();
 * 
 */
class ArpTable
{
public:
  /**
   * \brief Retrieves the ARP table from the local machine
   * and stores it in a vector.
   */
  void refreshTable();

  /**
   * \brief Removes all ARP entries from a given interface.
   * @param ifIndex index of interface to flush
   */
  void flushTable(uint32_t ifIndex);

  /**
   * \brief Adds an entry to the ARP table.
   * 
   * \warning This function requires sufficient priviledges when executed.
   * @param ipAddress IP address of entry
   * @param hwAddress hardware address of entry
   * @param ifIndex index of interface to add entry to
   */
  void addEntry(const IPv4Address &ipAddress, const HardwareAddress &hwAddress, uint32_t ifIndex);

  /**
   * \brief Deletes an entry from the ARP table.
   * Does not need the hardware address to be defined as the
   * table can be looked up by just the IP address.
   * 
   * \warning This function requires sufficient priviledges when executed.
   * @param ipAddress IP address of entry
   * @param ifIndex index of interface to add entry to
   */
  void deleteEntry(const IPv4Address &ipAddress, uint32_t ifIndex);

  /**
   * \brief Retrieves an entry on the ARP table given an IP address.
   * @param ipAddress IP address of entry to look up
   * @return requested entry or nullptr if not found
   */
  ArpEntry *getEntry(const IPv4Address &ipAddress);

  /**
   * \brief Returns the entries that have been retrieved from the local machine.
   * @return std::vector of ARP entries
   */
  std::vector<ArpEntry> &entries() { return m_entries; }

  /**
   * \brief Returns the static instance of this class.
   * @return ArpTable instance
   */
  static ArpTable &instance()
  {
    static ArpTable instance;
    return instance;
  }

private:
  ArpTable();
  std::vector<ArpEntry> m_entries;
};

}// namespace PacketHacker