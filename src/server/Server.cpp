#include "Server.hpp"

Server* Server::instance = NULL;

Server::Server() : port(0), socketFD(0), isRunning(false) {
  instance = this;
}

Server::~Server()
{
}

Server *Server::getInstance() {
  if(!instance) {
    instance = new Server();
  }
  return instance;
}

void Server::setPort(int port) {
  this->port = port;
}

void Server::listen(){
  if (!port) throw std::runtime_error("Port is not set");

  socketFD = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFD < 0) throw std::runtime_error("Socket creation failed");

  setSocket();

  if (bind(socketFD, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
    throw std::runtime_error("Bind failed");
  }

  if (::listen(socketFD, BACKLOG_QUEUE_SIZE) < 0) {
    throw std::runtime_error("Listen failed");
  }

  isRunning = true;
  std::cout << "Server is running on port " << port << std::endl;
  std::vector<int> clients;
  
  int epfd = epoll_create1(0);
  if (epfd == -1) {
    throw std::runtime_error("Epoll creation failed");
  }

  struct epoll_event ev, events[MAX_EVENTS];
  ev.events = EPOLLIN;
  ev.data.fd = socketFD;
  
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, socketFD, &ev) == -1) {
    throw std::runtime_error("Epoll control failed");
  }

  while (isRunning) {
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      throw std::runtime_error("Epoll wait failed");
    }

    for (int i = 0; i < nfds; i++) {
      if (events[i].data.fd == socketFD) {
        int newSocketFD = accept(socketFD, NULL, NULL);
        if (newSocketFD < 0) {
          throw std::runtime_error("Accept failed");
        }

        ev.events = EPOLLIN;
        ev.data.fd = newSocketFD;
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, newSocketFD, &ev) == -1) {
          throw std::runtime_error("Epoll control failed for new client");
        }

        clients.push_back(newSocketFD);
        std::cout << "Client connected" << std::endl;
      } else {
        char buffer[1024];
        int len = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
        
        if (len <= 0) {
          clients.erase(std::remove(clients.begin(), clients.end(), events[i].data.fd), clients.end());
          epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
          close(events[i].data.fd);
          std::cout << "Client disconnected" << std::endl;
        } else {
          for (size_t j = 0; j < clients.size(); j++) {
            if (clients[j] != events[i].data.fd) {
              send(clients[j], buffer, len, 0);
            }
          }
        }
      }
    }
  }
}

void Server::setSocket(){
  // 주소 구조체 설정
  
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(port);
  
  // SO_REUSEADDR 옵션 설정
  int opt = 1;
  if (setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    throw std::runtime_error("Setsockopt failed");
  }
}

