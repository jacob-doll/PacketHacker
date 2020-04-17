cmake_minimum_required(VERSION 3.16)
project(PacketHacker)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

find_package(PCAP REQUIRED)
include_directories(${PROJECT_NAME} ${PCAP_INCLUDE_DIR})

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/src)

set(PACKET_SRCS
    src/packet/packet_arp.cpp
    src/packet/packet_data.cpp
    src/packet/packet_eth.cpp
    src/packet/packet_ip.cpp
    src/packet/packet.cpp
    src/packet/utils/utils.cpp
    src/packet/utils/buffer_utils.cpp
)

set(SRCS 
    src/app/main.cpp
    src/app/main_window.cpp
    src/app/context.cpp
    src/app/ui/byte_viewer.cpp
)

add_executable(${PROJECT_NAME} WIN32 ${SRCS} ${PACKET_SRCS})
target_link_libraries(${PROJECT_NAME} ${PCAP_LIBRARY} ${wxWidgets_LIBRARIES})
if (WIN32)
    target_link_libraries(${PROJECT_NAME} IPHLPAPI)
endif()
