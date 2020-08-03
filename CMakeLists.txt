cmake_minimum_required(VERSION 3.16)

project(PacketHacker CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

find_package(PCAP REQUIRED)
include_directories(${PROJECT_NAME} ${PCAP_INCLUDE_DIR})

find_package(wxWidgets COMPONENTS core base propgrid aui REQUIRED)
include(${wxWidgets_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/src)

option(BUILD_SANDBOX "Build sandbox test of packet generation." OFF)

set(PACKET_SRCS
    src/packet/adapter.cpp
    src/packet/packet.cpp
    # src/packet/packet_queue.cpp
    src/packet/packets/arp_packet.cpp
    src/packet/packets/data_packet.cpp
    src/packet/packets/eth_packet.cpp
    src/packet/packets/icmp_packet.cpp
    src/packet/packets/ip_packet.cpp
    src/packet/packets/udp_packet.cpp
    src/packet/utils/adapter_utils.cpp
    src/packet/utils/packet_utils.cpp
)

set(SANDBOX_SRCS
    sandbox/sandbox.cpp
)

set(APP_SRCS 
    src/main.cpp
    src/app/main_window.cpp
    src/app/context.cpp
    src/app/ui/byte_viewer.cpp
    src/app/ui/packet_tree.cpp
    src/app/ui/details_pane.cpp
    src/app/ui/stream_pane.cpp
)

if (WIN32)
    set(PLATFORM_SRCS
        src/packet/platform/win32adapter.cpp
    )

    add_library(PacketHackerLib STATIC ${PACKET_SRCS} ${PLATFORM_SRCS})
    target_link_libraries(PacketHackerLib ${PCAP_LIBRARY} IPHLPAPI Ws2_32)

    add_executable(${PROJECT_NAME} WIN32 ${APP_SRCS})
    target_link_libraries(${PROJECT_NAME} PacketHackerLib ${wxWidgets_LIBRARIES})

    if (BUILD_SANDBOX)
        add_executable(TEST ${TEST_SRCS} ${PACKET_SRCS})
        target_link_libraries(TEST ${PCAP_LIBRARY} IPHLPAPI Ws2_32)
    endif()
endif()

