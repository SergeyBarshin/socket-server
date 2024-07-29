#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>

void closeSock(int& sock) {  // закрыть сокет, если сервер не 'поднялся'
    close(sock);
    sock = -1;
}

int create_connection(char* addr, char* port) {
    addrinfo* res = nullptr;
    int gaiError;

    addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };

    if ((gaiError = getaddrinfo(addr, port, &hints, &res))) {
        throw "gai error: " + std::string(gai_strerror(gaiError));
    }

    int sock = -1;

    for (addrinfo* ai = res; ai; ai = ai->ai_next) {
        sock = socket(ai->ai_family, ai->ai_socktype, 0);

        if (sock < 0) {
            std::cerr << "socket error" << '\n';
            continue;
        }

        if (connect(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            std::cerr << "connection error" << '\n';
            closeSock(sock);
            continue;
        }

        break;
    }

    freeaddrinfo(res);
    return sock;
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cout << "Unavailable argumentsts\n";
        return -1;
    }

    int sock = create_connection(argv[1], argv[2]);

    if (sock < 0) return 1;

    char respBuff[1024];

    if (read(sock, &respBuff, sizeof(respBuff) - 1) > 0)
        std::cout << "resp: " << respBuff << '\n';

    close(sock);
    return 0;
};

/*
принцип работы
getaddrinfo
socket()
connect()

read()
close()

cmake -S src -B build && cmake --build build && ./build/client localhost 8080
*/