#include "TCPClient.hpp"
#include <iostream>


TCPClient::TCPClient()
: socket(-1), connected(false), readBuffer(""), writeBuffer(""), delimiter("\r\n")
{
}

TCPClient::TCPClient(int socket) 
: socket(socket),
  connected(true),
  readBuffer(""),
  writeBuffer(""),
  delimiter("\r\n")
{}

TCPClient::~TCPClient(){}

TCPClient::TCPClient(const TCPClient& other) 
: socket(other.socket),
connected(other.connected), 
readBuffer(other.readBuffer),
writeBuffer(other.writeBuffer),
delimiter(other.delimiter) {
}

TCPClient& TCPClient::operator=(const TCPClient& other){
  if(this != &other){
    socket = other.socket;
    connected = other.connected;
    readBuffer = other.readBuffer;
    writeBuffer = other.writeBuffer;
    delimiter = other.delimiter;
  }
  return *this;
}

std::string::size_type TCPClient::findDelimiter() const {
  return readBuffer.find(delimiter);
}

void TCPClient::appendToBuffer(const char* data, size_t length){
  readBuffer.append(data, length);
}

void TCPClient::clearReadBuffer(){
  readBuffer.clear();
}

void TCPClient::send(const char* data){
  writeBuffer.append(data + delimiter);
}

void TCPClient::sendBufferFlush()
{
  ssize_t bytesSent = ::send(socket, writeBuffer.c_str(), writeBuffer.length(), 0);
  if (bytesSent == -1) {
    connected = false;
    // TODO:  NEED To check
  }
  writeBuffer.clear();
}

std::string TCPClient::receive(){
  char data[BUFFER_SIZE];
  ssize_t bytesReceived = recv(socket, data, BUFFER_SIZE, 0);
  std::cout << "Received " << bytesReceived << " bytes" << std::endl;
  std::cout << "Data: " << data << std::endl;
  if(bytesReceived == -1){
    connected = false;
    return "";
  }
  appendToBuffer(data, bytesReceived);
  std::string::size_type delimiterPos = findDelimiter();
  if(delimiterPos == std::string::npos){
    return "";
  }
  std::string message = readBuffer.substr(0, delimiterPos);
  readBuffer.erase(0, delimiterPos + delimiter.length());
  return message;
}

int TCPClient::getSocket() const
{
  return socket;
}
