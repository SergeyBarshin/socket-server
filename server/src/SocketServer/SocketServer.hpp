#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string>

class SocketServer {
    const char* port = "8080";
    int sock;

   public:
    SocketServer();

   private:
    void createListener();
    void startListener();
    void closeSock(int& sock);
};