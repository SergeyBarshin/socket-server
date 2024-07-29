#include "SocketServer/SocketServer.hpp"

int main(int argc, char* argv[]) {  // localhost::8080
    SocketServer server;            // создаем объект

    return 0;
}

/*
принцип работы:
getaddrinfo()
socket()
bind()
listen()

accept()
read()->write()
close()

cmake -S src -B build && cmake --build build && ./build/server
*/