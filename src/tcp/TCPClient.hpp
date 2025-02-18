#ifndef TCP_CLIENT_HPP
# define TCP_CLIENT_HPP

#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class TCPClient {
    public:
      TCPClient();
      TCPClient(int socket);
      ~TCPClient();
      TCPClient(const TCPClient& other);
      TCPClient& operator=(const TCPClient& other);

      void send(const char* data);
      void sendBufferFlush();
      std::string receive();
      int getSocket() const;

    private:
      static const int BUFFER_SIZE = 1024;
      int socket;
      bool connected;
      std::string readBuffer;
      std::string writeBuffer;
      std::string delimiter;
      std::string::size_type findDelimiter() const;
      void appendToBuffer(const char* data, size_t length);
      void clearReadBuffer();

};

#endif