#include "IRCServer.hpp"
#include <iostream>

IRCServer *IRCServer::instance = NULL;

IRCServer::IRCServer()
: server(), events(), users(UserRepository::getInstance()), channels(ChannelRepository::getInstance())
{
}

IRCServer::~IRCServer(){}



IRCServer &IRCServer::getInstance()
{
  if(instance == NULL)
  {
    instance = new IRCServer();
  
  }
  return *instance;
}

void IRCServer::destroy()
{
  if(instance != NULL)
  {
    delete instance;
  }
}

void IRCServer::start(int port)
{
  server.setAcceptCallback(acceptCallback);
  server.setDisconnectCallback(disconnectCallback);
  server.setReadCallback(readCallback);
  server.setWriteCallback(writeCallback);
  server.setErrorCallback(errorCallback);
  server.start(port);
}

void IRCServer::on(const std::string event, IRCEventCallback callback)
{
  this->events[event] = callback;
}

void IRCServer::broadcast(const char * data)
{
  UserRepository &userRepo = UserRepository::getInstance();
  std::map<int, User> users = userRepo.getUsers();
  for(std::map<fd, User>::iterator it = users.begin(); it != users.end(); ++it)
  {
    this->send(it->first, data);
  }
}

void IRCServer::send(fd clientSocket, const char * data)
{
  users.getUser(clientSocket)->send(data);
  server.enableWriteEvent(clientSocket);
}

void IRCServer::disconnect(fd clientSocket)
{
  this->server.disconnectClient(clientSocket);
  users.removeUser(clientSocket);
}

void IRCServer::disconnectAll()
{
  std::map<fd, User> users = this->users.getUsers();
  for(std::map<fd, User>::iterator it = users.begin(); it != users.end(); ++it)
  {
    this->disconnect(it->first);
  }
}

void IRCServer::enableWriteEvent(fd clientSocket)
{
  server.enableWriteEvent(clientSocket);
}

void IRCServer::acceptCallback(fd eventSocket)
{
  IRCServer* ircServer = static_cast<IRCServer*>(server);
  ircServer->users.addUser(User(eventSocket));
}

void IRCServer::disconnectCallback(fd eventSocket)
{
  IRCServer* irc = static_cast<IRCServer*>(server);
  /// TODO : disconnect 사이클 정하기
  (void)irc;
  (void)eventSocket;
}


void IRCServer::readCallback(fd eventSocket)
{
    IRCServer &irc = IRCServer::getInstance();
    UserRepository &userRepo = UserRepository::getInstance();
    User *user = userRepo.getUser(eventSocket);
    std::string message = user->receive();
    if(message.empty()){
      std::cout << "message is empty" << std::endl;
      return ;
    }
    std:: cout << "Server Side received message : " << message << std::endl;
    std::string command = message.substr(0, message.find(" "));
    std::string params = message.substr(message.find(" ") + 1);
    if(irc.events.find(command) != irc.events.end())
    {
      irc.events[command](eventSocket, (void*)params.c_str());
    }
    else
    {
      irc.broadcast(message.c_str());
    }
    
    // std::cout << "Received message: " << message << std::endl;
    // std::string command = message.substr(0, message.find(" "));
    // irc->broadcast(message.c_str());
}

void IRCServer::writeCallback(fd eventSocket)
{
  // IRCServer *irc = static_cast<IRCServer*>(server);
  (void)server;
  UserRepository &userRepo = UserRepository::getInstance();
  User *user = userRepo.getUser(eventSocket);
  user->sendBufferFlush();
}

void IRCServer::errorCallback(fd eventSocket)
{
  IRCServer *irc = static_cast<IRCServer*>(server);
  std::cout << "Error on socket " << eventSocket << std::endl;

  irc->disconnect(eventSocket);
  //TODO : Implement error handling
}


