
set(PACKET_SRCS
  arp_table.cpp
  hardware_address.cpp
  interface_table.cpp
  ip_address.cpp
  layer.cpp
  packet.cpp
  packet_queue.cpp
  packet_stream.cpp
  routing_table.cpp
  layers/arp_layer.cpp
  layers/data_layer.cpp
  layers/eth_layer.cpp
  layers/icmp_layer.cpp
  layers/ip_layer.cpp
  layers/udp_layer.cpp
  utils/adapter_utils.cpp
  utils/layer_utils.cpp
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