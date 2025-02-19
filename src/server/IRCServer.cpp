#include "IRCServer.hpp"
#include <iostream>

IRCServer *IRCServer::instance = NULL;

IRCServer::IRCServer()
: server(), events(), users(UserRepository::getInstance()), channels(ChannelRepository::getInstance()), password("gotohome")
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

void IRCServer::enableWriteEvent(fd clientSocket) {
  server.enableWriteEvent(clientSocket);
}

bool IRCServer::authenticate(std::string challenge){
  return password == challenge;
}

void IRCServer::acceptCallback(fd eventSocket)
{
  UserRepository &users = UserRepository::getInstance();
  users.addUser(User(eventSocket));
}

void IRCServer::disconnectCallback(fd eventSocket)
{
  /// TODO : disconnect 사이클 정하기
  (void)eventSocket;
}


void IRCServer::readCallback(fd eventSocket)
{
  IRCServer &irc = IRCServer::getInstance();
  UserRepository &userRepo = UserRepository::getInstance();
  User *user = userRepo.getUser(eventSocket);
  std::vector<std::string> messages = user->receive();
  Parser parser;
  for(std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it){
      // Message msg = Parser::parse(message);
      std::string msg = *it;
      Message currMsg = parser.parseMessage(*it);  
      if(irc.events.find(currMsg.getCommand()) != irc.events.end()) {
        irc.events[currMsg.getCommand()](eventSocket, &currMsg);
      } else {
        std::cout << "[IRC] NO EVENT HANDLER FOR " << currMsg.getCommand() << std::endl;
      }
      
  }
}

void IRCServer::writeCallback(fd eventSocket)
{
  // IRCServer *irc = static_cast<IRCServer*>(server);
  UserRepository &userRepo = UserRepository::getInstance();
  User *user = userRepo.getUser(eventSocket);
  user->sendBufferFlush();
}

void IRCServer::errorCallback(fd eventSocket)
{
  IRCServer &irc = IRCServer::getInstance();
  std::cout << "Error on socket " << eventSocket << std::endl;

  irc.disconnect(eventSocket);
  //TODO : Implement error handling
}


