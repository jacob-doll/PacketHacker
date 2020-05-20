cmake_minimum_required(VERSION 3.16)
project(PacketHacker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

find_package(PCAP REQUIRED)
include_directories(${PROJECT_NAME} ${PCAP_INCLUDE_DIR})

find_package(wxWidgets COMPONENTS core base propgrid REQUIRED)
include(${wxWidgets_USE_FILE})

find_package(tinyxml2 CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src)

set(PACKET_SRCS
    src/packet/packet.cpp
    src/packet/utils/utils.cpp
)

set(SRCS 
    src/main.cpp
    src/app/main_window.cpp
    src/app/context.cpp
    src/app/ui/byte_viewer.cpp
    src/app/ui/packet_tree.cpp
)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SRCS} ${PACKET_SRCS})
    target_link_libraries(${PROJECT_NAME} ${PCAP_LIBRARY} ${wxWidgets_LIBRARIES} IPHLPAPI Ws2_32 tinyxml2::tinyxml2)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "data/ARPPacket.xml" "${CMAKE_BINARY_DIR}/$<CONFIG>/data"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "data/EthernetPacket.xml" "${CMAKE_BINARY_DIR}/$<CONFIG>/data"
)
