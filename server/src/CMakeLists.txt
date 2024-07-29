cmake_minimum_required(VERSION 3.10)

set(project server)
project(${project})          
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(SocketServer)

add_executable(server main.cpp)

target_link_libraries(${project} SocketServer)