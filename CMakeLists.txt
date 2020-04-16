cmake_minimum_required(VERSION 3.16)
project(PacketHacker)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

find_package(PCAP REQUIRED)
include_directories(${PROJECT_NAME} ${PCAP_INCLUDE_DIR})

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/src)

# add_subdirectory(pcap-tests)
add_subdirectory(src)
