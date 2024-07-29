#include "SocketServer.hpp"

void SocketServer::closeSock(
    int& sock) {  // закрыть сокет, если сервер не 'поднялся'
    close(sock);
    sock = -1;
}

void SocketServer::createListener() {
    addrinfo* res = nullptr;
    int gaiError;  // get addr info

    addrinfo hints = {
        // параметры по которым мы ищем
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_PASSIVE,
    };

    if ((gaiError = getaddrinfo(nullptr, port, &hints, &res))) {
        throw "gai error: " + std::string(gai_strerror(gaiError));
    }

    addrinfo* ai = res;

    bool wasErrors = false;  // если какой-то из

    // итерируем по всем адресам и на 1 'рабочем' запускаем сервер
    for (addrinfo* ai = res; ai; ai = ai->ai_next) {
        sock = socket(ai->ai_family, ai->ai_socktype, 0);

        if (sock < 0) {
            std::cerr << "socket error" << '\n';
            continue;
        }

        // делаем порт пригодным для повторного использования
        int one = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one))) {
            std::cerr << "setsockopt error" << '\n';
            closeSock(sock);
            continue;
        }

        // try to bind and listen
        if (bind(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            std::cerr << "bind error" << '\n';
            closeSock(sock);
            continue;
        }

        if (listen(sock, SOMAXCONN) < 0) {
            std::cerr << "bind error" << '\n';
            closeSock(sock);
            continue;
        }

        break;  // завершаем цикл на первом 'рабочем' сокете
    }

    freeaddrinfo(res);
}

void SocketServer::startListener() {

    while (true) {
        sockaddr_in address;
        socklen_t addrlen = sizeof(address);

        int connection = accept(sock, (struct sockaddr*)&address, &addrlen);

        char ip_str[INET_ADDRSTRLEN];
        if (!inet_ntop(AF_INET, &address.sin_addr, ip_str,
                       sizeof(ip_str)))  // получаем 'читаемый ip'
            std::cerr << "parce IP error";

        std::cout << ip_str << ':' << address.sin_port << '\n';

        std::string resp = "OK\n";
        write(connection, &resp, resp.size());

        close(connection);
    }
    close(sock);
}

SocketServer::SocketServer() : sock(-1) {

    try {
        createListener();
    } catch (const char* errMsg) {
        std::cerr << "ERR: " << errMsg << '\n';
        exit(-1);
    }

    std::cout << "--- Server started on port 8080 ---\n";

    startListener();
}
