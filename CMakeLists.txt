cmake_minimum_required(VERSION 3.16)
project(PacketHacker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

find_package(PCAP REQUIRED)
include_directories(${PROJECT_NAME} ${PCAP_INCLUDE_DIR})

find_package(wxWidgets COMPONENTS core base propgrid REQUIRED)
include(${wxWidgets_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/src)

set(PACKET_SRCS
    src/packet/adapter.cpp
    src/packet/packet.cpp
    src/packet/eth_packet.cpp
    src/packet/arp_packet.cpp
    src/packet/icmp_packet.cpp
    src/packet/ip_packet.cpp
    src/packet/utils/utils.cpp
    src/packet/utils/packet_utils.cpp
)

set(TEST_SRCS
    test/packet_test.cpp
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
    add_executable(${PROJECT_NAME} WIN32 ${APP_SRCS} ${PACKET_SRCS})
    target_link_libraries(${PROJECT_NAME} ${PCAP_LIBRARY} ${wxWidgets_LIBRARIES} IPHLPAPI Ws2_32)
endif()

# add_executable(TEST ${TEST_SRCS} ${PACKET_SRCS})
# target_link_libraries(TEST ${PCAP_LIBRARY} ${wxWidgets_LIBRARIES} IPHLPAPI Ws2_32)
