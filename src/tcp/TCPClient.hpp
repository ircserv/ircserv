#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <string>

class TCPClient {
 public:
  TCPClient();
  TCPClient(int socket);
  ~TCPClient();
  TCPClient(const TCPClient& other);
  TCPClient& operator=(const TCPClient& other);

  void send(const char* data);
  bool sendBufferFlush();
  std::string receive();
  int getSocket() const;

 private:
  static const int BUFFER_SIZE = 1024;
  int socket;
  std::string readBuffer;
  std::string writeBuffer;
  std::string delimiter;

  std::string::size_type findDelimiter() const;
  void appendToBuffer(const char* data, size_t length);
  void clearReadBuffer();
};

#endif