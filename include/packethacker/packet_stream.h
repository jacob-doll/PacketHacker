#pragma once

#include <pcap.h>

#include "packet.h"
#include "interface_table.h"

namespace PacketHacker {

/**
 * \brief This class allows for the opening of packet streams on an interface.
 * 
 * With this class you can open a packet stream and then 
 * use the open stream to read incoming packets like a sniffer
 * or send packets.
 * 
 * In order to use this class you first have to select an interface. With
 * the interface selected you can then open a stream up and subsequently
 * close it when finished.
 * 
 * ex: 
 * Interface* inter = ...;
 * Packet stream(inter);
 * 
 * char errbuf[256];
 * stream.openPacketStream(errbuf);
 * stream.sendPacket(..., errbuf);
 * stream.closePacketStream();
 */
class PacketStream
{
public:
  /**
   * \brief Creates a packet stream with a supplied interface.
   * @param streamInterface interface to open streamon.
   */
  PacketStream(Interface *streamInterface);

  /**
   * \brief Opens the stream to send or receive packets.
   * 
   * \warning Stream must be closed in order to open.
   * @param errbuf buffer where error information is outputted
   * @return true if packet stream could successfully be opened,
   * false if any errors occur
   */
  bool openPacketStream(char *errbuf);

  /**
   * \brief Closes the packet stream.
   * 
   * If packet stream already closed then this does nothing.
   */
  void closePacketStream();

  /**
   * \brief Sends a packet through the interface.
   * 
   * \warning Packet stream must be opened before sending packets
   * can happen.
   * @param packet packet to be sent
   * @param errbuf buffer where error information is outputted
   * @return true if packet successfully sent, false if any errors
   * occur
   */
  bool sendPacket(Packet *packet, char *errbuf);

  /**
   * \brief Reads the next packet from the stream and returns the byte buffer.
   * 
   * \warning Packet stream must be opened before receiving packets.
   * @param size pointer to size integer, this is set to the size of the packet
   * @param errbuf buffer where error information is outputted
   * @return byte array of data that contains packet info, nullptr is 
   * returned if no packet received.
   */
  const uint8_t *getNextPacket(uint32_t *size, char *errbuf);

  /**
   * \brief Returns whether stream is open or not.
   * @return true if open, false if not 
   */
  bool streamOpen() const { return m_streamOpen; }

private:
  Interface *m_streamInterface;
  pcap *m_handle;
  bool m_streamOpen;
};

}// namespace PacketHacker