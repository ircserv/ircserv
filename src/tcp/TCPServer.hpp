#ifndef TCP_SERVER_HPP
# define TCP_SERVER_HPP

#include <sys/event.h>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include "TCPClient.hpp"

typedef int fd;
typedef void (*EventCallback)(fd fd);

class TCPServer {
    private:
      int serverSocket;
      int kq;
      int maxEvents;
      bool running;
      struct kevent* eventlists;
      std::map<fd, TCPClient *> clients;
      EventCallback acceptCallback;
      EventCallback disconnectCallback;
      EventCallback writeCallback;
      EventCallback readCallback;
      EventCallback errorCallback;

      TCPServer(const TCPServer&);
      TCPServer& operator=(const TCPServer&);
      
      void initializeKqueue();
      void registerEvent(fd fd, int filter, int flags);
      void setNonBlocking(fd socket);
      void unregisterEvent(fd fd, int filter);
      fd connectClient();
      void handleEventFilter(int filter, int fd);

    public:
      TCPServer();
      ~TCPServer(); 

      void start(int port);
      void setAcceptCallback(EventCallback callback);
      void setDisconnectCallback(EventCallback callback);
      void setReadCallback(EventCallback callback);
      void setWriteCallback(EventCallback callback);
      void setErrorCallback(EventCallback callback);
      void eventLoop();
      void disconnectClient(fd clientSocket);
      void enableWriteEvent(fd clientSocket);
};  

#endif