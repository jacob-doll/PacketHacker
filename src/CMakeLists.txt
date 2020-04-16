set(PACKET_SRCS
    packet/packet_arp.cpp
    packet/packet_eth.cpp
    packet/packet_ip.cpp
    packet/packet.cpp
    packet/utils/utils.cpp
    packet/utils/buffer_utils.cpp
)

set(SRCS 
    app/main.cpp
    app/main_window.cpp
    app/context.cpp
    app/ui/byte_viewer.cpp
)

add_executable(${PROJECT_NAME} WIN32 ${SRCS} ${PACKET_SRCS})
target_link_libraries(${PROJECT_NAME} ${PCAP_LIBRARY} ${wxWidgets_LIBRARIES})
if (WIN32)
    target_link_libraries(${PROJECT_NAME} IPHLPAPI)
endif()