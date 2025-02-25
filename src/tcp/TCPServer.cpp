#include "TCPServer.hpp"
#include <arpa/inet.h>
#include <iostream>

void handle_kevent_error(int error_code) {
    std::cerr << "kevent 실패: ";
    
    switch (error_code) {
        case EACCES:
            std::cerr << "접근 권한이 없습니다." << std::endl;
            break;
            
        case EFAULT:
            std::cerr << "changelist 또는 eventlist가 접근 가능한 주소 공간 영역을 가리키지 않습니다." << std::endl;
            break;
            
        case EBADF:
            std::cerr << "kq 파일 디스크립터가 유효하지 않습니다." << std::endl;
            break;
            
        case EINTR:
            std::cerr << "kevent() 호출이 신호에 의해 중단되었습니다." << std::endl;
            break;
            
        case EINVAL:
            std::cerr << "지정된 시간 제한 또는 필터가 유효하지 않습니다." << std::endl;
            break;
            
        case ENOENT:
            std::cerr << "식별자가 존재하지 않습니다." << std::endl;
            break;
            
        case ENOMEM:
            std::cerr << "메모리 할당에 실패했습니다." << std::endl;
            break;
            
        case ESRCH:
            std::cerr << "프로세스가 존재하지 않습니다." << std::endl;
            break;
            
        case EAGAIN:
            std::cerr << "리소스 한도에 도달하였습니다." << std::endl;
            break;

        case ENOTSUP:
            std::cerr << "이 시스템에서 지원되지 않는 작업입니다." << std::endl;
            break;

        default:
            std::cerr << "알 수 없는 오류: " << strerror(error_code) << std::endl;
            break;
    }
}


TCPServer::TCPServer() : serverSocket(-1), kq(-1), running(false), port(0), ip("") {
  clients = std::map<fd, TCPClient *>();
  eventlists = new struct kevent[MAX_EVENTS];
  writeEvents = std::set<struct kevent, utils::KeventCompare>();
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

void TCPServer::start() {
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    throw std::runtime_error("Failed to create socket");
  }

  int opt = 1;
  setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = ip.empty() ? INADDR_ANY : inet_addr(ip.c_str());
  serverAddr.sin_port = htons(port);

  if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) ==
      -1) {
    throw std::runtime_error("Failed to bind socket");
  }
  initializeKqueue();
  setNonBlocking(serverSocket);
  struct kevent serverEvent;
  EV_SET(&serverEvent, serverSocket, EVFILT_READ, EV_ADD, 0, SOMAXCONN, NULL);
  if (kevent(kq, &serverEvent, 1, NULL, 0, NULL) == -1){
    throw std::runtime_error("Failed to register server event");
  }
  if (listen(serverSocket, SOMAXCONN) == -1) {
    throw std::runtime_error("Failed to listen on socket");
  }

  running = true;
  // // std::cout << "Server started on port " << port << std::endl;
  while (running) {
    eventLoop();
  }
}

void TCPServer::setPort(int port) {
  this->port = port;
}

void TCPServer::setIp(std::string ip) {
  this->ip = ip;
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
      // // std::cout << "New client connected" << std::endl;
      continue ;
    }

    if (flags & EV_EOF) {  // ^D
      disconnectCallback(fd);
      continue ;
    }
    handleEventFilter(filter, fd);
  }
  std::vector<struct kevent> writeEventsVector(writeEvents.begin(), writeEvents.end());
  if (kevent(kq, writeEventsVector.data(), writeEventsVector.size(), NULL, 0, NULL) == -1) {
    handle_kevent_error(errno);
    throw std::runtime_error("Failed to write events");
  }
  writeEvents.clear();
}

void TCPServer::initializeKqueue() {
  kq = kqueue();
  if (kq == -1) {
    throw std::runtime_error("Failed to create kqueue");
  }
}

void TCPServer::setNonBlocking(fd socket) {
  int flags = fcntl(socket, F_GETFL, 0);
  fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

void TCPServer::registerEvent(fd fd, int filter, int flags) {
  struct kevent event;
  EV_SET(&event, fd, filter, flags, 0, 0, NULL);
  flags |= EV_ADD;
  
  writeEvents.insert(event);
}

void TCPServer::unregisterEvent(fd fd, int filter) {
  struct kevent event;

  EV_SET(&event, fd, filter, EV_DELETE, 0, 0, NULL);
  
  if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
    throw std::runtime_error("Failed to unregister event" + std::to_string(fd));
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
  struct kevent event;
  EV_SET(&event, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
  if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
    throw std::runtime_error("Failed to register client event");
  }
  return clientSocket;
}

void TCPServer::disconnectClient(fd clientSocket) {
  // unregisterEvent(clientSocket, EVFILT_READ | EVFILT_WRITE);
  struct kevent event;
  EV_SET(&event, clientSocket, EVFILT_WRITE, 0, 0, 0, NULL);
  writeEvents.erase(event);
  std::cout << "[DISCONNECT] : " << clientSocket << std::endl;
  close(clientSocket);
}

void TCPServer::enableWriteEvent(fd cientSocket) {
  registerEvent(cientSocket, EVFILT_WRITE, EV_ADD | EV_ONESHOT);
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
