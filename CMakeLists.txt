cmake_minimum_required(VERSION 3.16)

project(PacketHacker CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

find_package(PCAP REQUIRED)
include_directories(${PROJECT_NAME} ${PCAP_INCLUDE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/include/packethacker)

add_subdirectory(src)

option(BUILD_SANDBOX "Build sandbox test of packet generation." ON)

if (BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif()

# find_package(wxWidgets COMPONENTS core base propgrid aui REQUIRED)
# include(${wxWidgets_USE_FILE})

# set(APP_SRCS 
#     src/main.cpp
#     src/app/main_window.cpp
#     src/app/context.cpp
#     src/app/ui/byte_viewer.cpp
#     src/app/ui/packet_tree.cpp
#     src/app/ui/details_pane.cpp
#     src/app/ui/received_pane.cpp
#     src/app/ui/stream_pane.cpp
# )

# add_executable(${PROJECT_NAME} WIN32 ${APP_SRCS})
# target_link_libraries(${PROJECT_NAME} PacketHackerLib ${wxWidgets_LIBRARIES})

