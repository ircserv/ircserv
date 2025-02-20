#include "IRCServer.hpp"
#include <iostream>

IRCServer *IRCServer::instance = NULL;

IRCServer::IRCServer()
: server(), events(), password("gotohome")
{
  time_t rawtime;
  time(&rawtime);
        
  char timeBuffer[80];
  struct tm* timeinfo = localtime(&rawtime);
  strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  
  createdTime = ":This server was created ";
  createdTime += timeBuffer;
  createdTime += "KST";
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

std::string const &IRCServer::getCreatedTime() const
{
  return createdTime;
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
  std::map<int, User> &users = userRepo.getUsers();
  for(std::map<fd, User>::iterator it = users.begin(); it != users.end(); ++it)
  {
    this->send(it->first, data);
  }
}

void IRCServer::send(fd clientSocket, const char * data)
{
  UserRepository &users = UserRepository::getInstance();
  users.getUser(clientSocket)->send(data);
  server.enableWriteEvent(clientSocket);
}

void IRCServer::disconnect(fd clientSocket)
{
  UserRepository &users = UserRepository::getInstance();
  this->server.disconnectClient(clientSocket);
  users.removeUser(clientSocket);
}

void IRCServer::disconnectAll()
{
  UserRepository &userRepo = UserRepository::getInstance();
  std::map<int, User> &users = userRepo.getUsers();
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
      std::string msg = *it;

      Message currMsg = parser.parseMessage(*it);  
      if (irc.events.find(currMsg.getCommand()) != irc.events.end()) {
        if (Middleware::authentifications(eventSocket, &currMsg)) {
          continue ;
        }
        if (Middleware::registrations(eventSocket, &currMsg)) {
          continue ;
        }

        irc.events[currMsg.getCommand()](eventSocket, &currMsg);

        Middleware::doWelcome(eventSocket, &currMsg);
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


