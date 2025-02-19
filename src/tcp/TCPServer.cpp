#include "TCPServer.hpp"

#include <iostream>

TCPServer::TCPServer() : serverSocket(-1), kq(-1), running(false) {
  clients = std::map<fd, TCPClient *>();
  eventlists = new struct kevent[MAX_EVENTS];
}

TCPServer::~TCPServer() {
  delete[] eventlists;

  if (serverSocket != -1) {
    close(serverSocket);
  }
  if (kq != -1) {
    close(kq);
  }
}

void TCPServer::start(int port) {
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    throw std::runtime_error("Failed to create socket");
  }

  int opt = 1;
  setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(port);

  if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) ==
      -1) {
    throw std::runtime_error("Failed to bind socket");
  }
  initializeKqueue();
  if (listen(serverSocket, SOMAXCONN) == -1) {
    throw std::runtime_error("Failed to listen on socket");
  }

  running = true;
  std::cout << "Server started on port " << port << std::endl;
  while (running) {
    eventLoop();
  }
}

void TCPServer::eventLoop() {
  int newEvents = kevent(kq, NULL, 0, eventlists, MAX_EVENTS, NULL);

  for (int i = 0; i < newEvents; i++) {
    int fd = eventlists[i].ident;
    int filter = eventlists[i].filter;
    int flags = eventlists[i].flags;
    if (fd == serverSocket && filter == EVFILT_READ) {
      int newClient = connectClient();
      acceptCallback(newClient);
      return;
    }
    if (flags & EV_EOF) {  // ^D
      disconnectClient(fd);
      disconnectCallback(fd);
      return;
    }
    handleEventFilter(filter, fd);
  }
}

void TCPServer::initializeKqueue() {
  kq = kqueue();
  if (kq == -1) {
    throw std::runtime_error("Failed to create kqueue");
  }
  setNonBlocking(serverSocket);
  registerEvent(serverSocket, EVFILT_READ, EV_ADD);
}

void TCPServer::setNonBlocking(fd socket) {
  int flags = fcntl(socket, F_GETFL, 0);
  fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

void TCPServer::registerEvent(fd fd, int filter, int flags) {
  struct kevent event;
  EV_SET(&event, fd, filter, flags, 0, 0, NULL);
  flags |= EV_ADD;
  if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
    throw std::runtime_error("Failed to register event");
  }
}

void TCPServer::unregisterEvent(fd fd, int filter) {
  struct kevent event;
  std::cout << "Unregistering event" << fd << " " << filter << std::endl;
  EV_SET(&event, fd, filter, EV_DELETE, 0, 0, NULL);
  if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
    throw std::runtime_error("Failed to unregister event");
  }
}

void TCPServer::handleEventFilter(int filter, int fd) {
  switch (filter) {
    case EVFILT_READ:
      if (readCallback) readCallback(fd);
      break;
    case EVFILT_WRITE:
      if (writeCallback) writeCallback(fd);
      break;
    default:
      break;
  }
}

fd TCPServer::connectClient() {
  struct sockaddr_in clientAddr;
  socklen_t clientAddrSize = sizeof(clientAddr);

  int clientSocket =
      accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
  if (clientSocket == -1) {
    throw std::runtime_error("Failed to accept connection");
  }
  setNonBlocking(clientSocket);
  registerEvent(clientSocket, EVFILT_READ, EV_ADD);

  return clientSocket;
}

void TCPServer::disconnectClient(fd clientSocket) {
  unregisterEvent(clientSocket, EVFILT_READ);
  close(clientSocket);
}

void TCPServer::enableWriteEvent(fd clientSocket) {
  registerEvent(clientSocket, EVFILT_WRITE, EV_ADD | EV_ONESHOT);
}

void TCPServer::setAcceptCallback(EventCallback callback) {
  acceptCallback = callback;
}

void TCPServer::setDisconnectCallback(EventCallback callback) {
  disconnectCallback = callback;
}

void TCPServer::setReadCallback(EventCallback callback) {
  readCallback = callback;
}

void TCPServer::setWriteCallback(EventCallback callback) {
  writeCallback = callback;
}

void TCPServer::setErrorCallback(EventCallback callback) {
  errorCallback = callback;
}
