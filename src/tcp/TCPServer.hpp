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
typedef void (*EventCallback)(fd fd, void* userData);

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
      void* acceptUserData;
      void* disconnectUserData;
      void* readUserData;
      void* writeUserData;
      void* errorUserData;

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
      void setAcceptCallback(EventCallback callback, void* userData);
      void setDisconnectCallback(EventCallback callback, void* userData);
      void setReadCallback(EventCallback callback, void* userData);
      void setWriteCallback(EventCallback callback, void* userData);
      void setErrorCallback(EventCallback callback, void* userData);
      void eventLoop();
      void disconnectClient(fd clientSocket);
      void enableWriteEvent(fd clientSocket);
};  

#endif