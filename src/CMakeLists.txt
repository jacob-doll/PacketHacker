
set(PACKET_SRCS
  arp_table.cpp
  hardware_address.cpp
  interface_table.cpp
  ip_address.cpp
  packet.cpp
  packet_queue.cpp
  packet_stream.cpp
  routing_table.cpp
  packets/arp_packet.cpp
  packets/data_packet.cpp
  packets/eth_packet.cpp
  packets/icmp_packet.cpp
  packets/ip_packet.cpp
  packets/udp_packet.cpp
  utils/adapter_utils.cpp
  utils/packet_utils.cpp
)

if (WIN32)
    set(PLATFORM_SRCS
        platform/win32interface.cpp
        platform/win32arp_table.cpp
        platform/win32routing_table.cpp
    )

    add_library(PacketHackerLib STATIC ${PACKET_SRCS} ${PLATFORM_SRCS})
    target_link_libraries(PacketHackerLib ${PCAP_LIBRARY} IPHLPAPI Ws2_32)
endif()