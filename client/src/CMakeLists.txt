cmake_minimum_required(VERSION 3.10)

set(project client)
project(${project})
set(CMAKE_CXX_STANDARD 20)

#add_subdirectory()

add_executable(client main.cpp)

#target_link_libraries(${project} SocketClient)