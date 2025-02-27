#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#define MAX_EVENTS 32

#include <fcntl.h>
#include <netinet/in.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

#include <map>
#include <set>
#include <string>

#include "../utils/utils.hpp"
#include "TCPClient.hpp"

typedef int fd;
typedef void (*EventCallback)(fd fd);

class TCPServer {
 private:
  int serverSocket;
  int kq;
  bool running;
  struct kevent eventlists[MAX_EVENTS];
  int port;
  std::string ip;
  std::set<struct kevent, utils::KeventCompare> writeEvents;

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
  fd connectClient();
  void handleEventFilter(int filter, int fd);

 public:
  TCPServer();
  ~TCPServer();

  void start();
  void stop();
  void setPort(int port);
  void setIp(std::string ip);
  void setAcceptCallback(EventCallback callback);
  void setDisconnectCallback(EventCallback callback);
  void setReadCallback(EventCallback callback);
  void setWriteCallback(EventCallback callback);
  void eventLoop();
  void disconnectClient(fd clientSocket);
  void enableWriteEvent(fd clientSocket);
};

#endif