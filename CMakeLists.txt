cmake_minimum_required(VERSION 3.21)
project(network_protocols_cp C)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

find_library(libconfig libconfig)

set(CMAKE_C_STANDARD 99)

add_executable(network_protocols_cp main.c config.c server-fsm.c error.c config.h server-fsm.h error.h defines.h)

target_link_libraries(network_protocols_cp config)


