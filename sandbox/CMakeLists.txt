set(SANDBOX_SRCS
    sandbox.cpp
)

add_executable(SANDBOX ${SANDBOX_SRCS})
target_link_libraries(SANDBOX PacketHackerLib)