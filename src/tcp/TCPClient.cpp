#include "TCPClient.hpp"

TCPClient::TCPClient()
    : socket(-1), readBuffer(""), writeBuffer(""), delimiter("\r\n") {}

TCPClient::TCPClient(int socket)
    : socket(socket), readBuffer(""), writeBuffer(""), delimiter("\r\n") {}

TCPClient::~TCPClient() {}

TCPClient::TCPClient(const TCPClient& other)
    : socket(other.socket),
      readBuffer(other.readBuffer),
      writeBuffer(other.writeBuffer),
      delimiter(other.delimiter) {}

TCPClient& TCPClient::operator=(const TCPClient& other) {
  if (this != &other) {
    socket = other.socket;
    readBuffer = other.readBuffer;
    writeBuffer = other.writeBuffer;
    delimiter = other.delimiter;
  }
  return *this;
}

std::string::size_type TCPClient::findDelimiter() const {
  return readBuffer.find(delimiter);
}

void TCPClient::appendToBuffer(const char* data, size_t length) {
  readBuffer.append(data, length);
}

void TCPClient::clearReadBuffer() { readBuffer.clear(); }

void TCPClient::send(const char* data) {
  writeBuffer.append(data);
  writeBuffer.append(delimiter);
}

bool TCPClient::sendBufferFlush() {
  ssize_t bytesSent =
      ::send(socket, writeBuffer.c_str(), writeBuffer.length(), 0);
  if (bytesSent < 0) {
    throw std::runtime_error("broken pipe: ");
  }
  writeBuffer = writeBuffer.substr(bytesSent);
  if (writeBuffer.empty()) {
    return true;
  }
  return false;
}

std::string TCPClient::receive() {
  char data[BUFFER_SIZE];
  memset(data, 0, BUFFER_SIZE);
  if (!readBuffer.empty() && findDelimiter() != std::string::npos) {
    std::string message = readBuffer.substr(0, findDelimiter());
    readBuffer.erase(0, findDelimiter() + delimiter.length());
    return message;
  }
  ssize_t bytesReceived = recv(socket, data, BUFFER_SIZE, 0);
  if (bytesReceived == -1) {
    return "";
  }
  appendToBuffer(data, bytesReceived);
  memset(data, 0, BUFFER_SIZE);
  std::string::size_type delimiterPos = findDelimiter();
  if (delimiterPos == std::string::npos) {
    return "";
  }
  std::string message = readBuffer.substr(0, delimiterPos);
  readBuffer.erase(0, delimiterPos + delimiter.length());
  return message;
}

int TCPClient::getSocket() const { return socket; }
