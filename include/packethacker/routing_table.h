#pragma once

#include "ip_address.h"

#include <vector>

namespace PacketHacker {

/**
 * \brief Struct representation of a route on a local routing table.
 * 
 * Routes map which interface to use depending on the network
 * destination. Given a supplied IP address bit-wise anded with 
 * the netmask, if that equals the networkDest of the route then
 * the packet it forwarded to the next hop.
 */
struct RouteEntry
{
  IPv4Address networkDest; /**< Network destination of the route. */
  IPv4Address netmask; /**< Network mask used in route calculation. */
  IPv4Address nextHop; /**< IP address to forward the packet to. */
  uint32_t metric; /**< Metric value of the route. */
  uint32_t ifIndex; /**< Interface index that this route applies to. */
};

/**
 * \brief Class representation of the Routing table of the local machine.
 * 
 * This class allows for the manipulation of the Routing table on 
 * the local machine. Sufficient priviledges are needed for addEntry()
 * and deleteEntry().
 * 
 * This class cannon and should not be initialized in your code. There
 * should only ever be one instance.
 * 
 * To access the Routing table:
 *   - RoutingTable table = RoutingTable::instance();
 */
class RoutingTable
{
public:
  /**
   * \brief Retrieves the Routing table from the local machine
   * and stores it in a vector.
   */
  void refreshTable();

  /**
   * \brief Adds a route to the local routing table.
   * 
   * \warning This function requires sufficient priviledges when executed.
   * @param networkDest network destination of the route
   * @param netmask network mask of the route
   * @param nextHop the forwarding IP address
   * @param metric metric of the route
   * @param ifIndex interface that this route applies to
   */
  void addEntry(const IPv4Address &networkDest,
    const IPv4Address &netmask,
    const IPv4Address &nextHop,
    const uint32_t metric,
    const uint32_t ifIndex);

  /**
   * \brief Delets a route from the local routing table.
   * 
   * Only the network destination needs to be supplied to delete a route.
   * \warning This function requires sufficient priviledges when executed.
   * @param networkDest network destination of the route to delete
   */
  void deleteEntry(const IPv4Address &networkDest);

  /**
   * \brief Retrieves route given a network destination.
   * @param networkDest destination network of route
   * @return pointer to the route entry
   */
  RouteEntry *getEntryFromNetDest(const IPv4Address &networkDest);

  /**
   * \brief Retrieves route given an IP destination.
   * 
   * Different from getEntryFromNetDest as an IP address is
   * supplied insted of a network destination.
   * @param ipDest destination address to find route for
   * @return pointer to the route entry
   */
  RouteEntry *getEntryFromIpDest(const IPv4Address &ipDest);

  /**
   * \brief Returns the entries that have been retrieved from the local machine.
   * @return std::vector of Route entries
   */
  std::vector<RouteEntry> &entries() { return m_entries; }

  /**
   * \brief Returns the static instance of this class.
   * @return RoutingTable instance
   */
  static RoutingTable &instance()
  {
    static RoutingTable instance;
    return instance;
  }

private:
  RoutingTable();
  std::vector<RouteEntry> m_entries;
};

}// namespace PacketHacker