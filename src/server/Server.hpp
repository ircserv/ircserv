#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
// for epoll

#include <sys/epoll.h>
#include <vector>


# define BACKLOG_QUEUE_SIZE 10
# define BUFFER_SIZE 1024

class Server {
  public:
    static Server* getInstance();
    void listen();
    void setPort(int port);
    // void use(std::string key, std::function<void(std::string)> controller);
    // void send(const std::string& message);
    private:
      static Server* instance;
      Server();
      ~Server();
      Server(const Server&);
      Server& operator=(const Server&);

      void setSocket();
      int port;
      int socketFD;
      struct sockaddr_in serverAddr;
      char buffer[BUFFER_SIZE];
      bool isRunning;
};


#endif