#pragma once

#include <pcap.h>
#include <exception>

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
   * \warning Stream must be closed in order to open.
   * \exception Throws a stream exception if the stream is already opened
   * or stream cannot be opened. 
   */
  void openPacketStream();

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
   * \exception Throws an exception if stream is not open or if
   * sending the packet fails.
   * @param packet packet to be sent
   */
  void sendPacket(Packet *packet);

  /**
   * \brief Reads the next packet from the stream and returns the byte buffer.
   * 
   * \warning Packet stream must be opened before receiving packets.
   * \exception Throws an exception if stream is not open or if there
   * was an error reading the next packet.
   * @param size pointer to size integer, this is set to the size of the packet
   * @return byte array of data that contains packet info, nullptr is 
   * returned if no packet received.
   */
  const uint8_t *getNextPacket(uint32_t *size);

  /**
   * \brief Returns whether stream is open or not.
   * @return true if open, false if not 
   */
  bool streamOpen() const { return m_streamOpen; }

  /**
   * \brief Excption class for throwing stream exceptions.
   */
  class StreamException : public std::exception
  {
  public:
    /**
     * \brief Default constructor.
     * @param message message of exception 
     */
    StreamException(const std::string &message)
      : m_message(std::move(message))
    {}

    /**
     * \brief Returns the exception message.
     * @return message
     */
    virtual const char *what() const throw()
    {
      return m_message.c_str();
    }

  private:
    std::string m_message;
  };

private:
  Interface *m_streamInterface;
  pcap *m_handle;
  bool m_streamOpen;
};

}// namespace PacketHacker