#include "IRCServer.hpp"

#include <iostream>

#define RED "\033[31m"
#define RESET "\033[0m"

IRCServer *IRCServer::instance = NULL;

IRCServer::IRCServer() : server(), events(), password() {
  time_t rawtime;
  time(&rawtime);

  char timeBuffer[80];
  struct tm *timeinfo = localtime(&rawtime);
  strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeinfo);

  createdTime = ":This server was created ";
  createdTime += timeBuffer;
  createdTime += "KST";
}

IRCServer::~IRCServer() {}

void IRCServer::stop() { server.stop(); }

IRCServer &IRCServer::getInstance() {
  if (instance == NULL) {
    instance = new IRCServer();
  }
  return *instance;
}

void IRCServer::destroy() {
  if (instance != NULL) {
    delete instance;
  }
}

std::string const &IRCServer::getCreatedTime() const { return createdTime; }

void IRCServer::setPort(int port) { server.setPort(port); }

void IRCServer::setIp(std::string ip) { server.setIp(ip); }

void IRCServer::setPassword(std::string password) { this->password = password; }

void IRCServer::start() {
  server.setAcceptCallback(acceptCallback);
  server.setDisconnectCallback(disconnectCallback);
  server.setReadCallback(readCallback);
  server.setWriteCallback(writeCallback);
  server.start();
}

void IRCServer::on(const std::string event, IRCEventCallback callback) {
  this->events[event] = callback;
}

void IRCServer::broadcast(const std::string &data) {
  UserRepository &userRepo = UserRepository::getInstance();
  std::map<int, User> &users = userRepo.getUsers();
  for (std::map<fd, User>::iterator it = users.begin(); it != users.end();
       ++it) {
    this->send(it->first, data);
  }
}

void IRCServer::send(fd clientSocket, const std::string &data) {
  UserRepository &users = UserRepository::getInstance();
  users.getUser(clientSocket)->send(data);
  server.enableWriteEvent(clientSocket);
}

void IRCServer::disconnect(fd clientSocket) {
  UserRepository &users = UserRepository::getInstance();
  ChannelRepository &channelRepo = ChannelRepository::getInstance();
  User *user = users.getUser(clientSocket);
  std::set<std::string> invitedChannels = user->getInvitedChannels();
  for (std::set<std::string>::iterator it = invitedChannels.begin();
       it != invitedChannels.end(); ++it) {
    Channel *channel = channelRepo.getChannel(*it);
    if (channel) {
      channel->uninvite(*user);
    }
  }
  users.removeUser(clientSocket);
  this->server.disconnectClient(clientSocket);
}

void IRCServer::enableWriteEvent(fd clientSocket) {
  server.enableWriteEvent(clientSocket);
}

bool IRCServer::authenticate(std::string challenge) {
  return password == challenge;
}

void IRCServer::acceptCallback(fd eventSocket) {
  UserRepository &users = UserRepository::getInstance();
  users.addUser(eventSocket);
}

void IRCServer::disconnectCallback(fd eventSocket) {
  UserRepository &users = UserRepository::getInstance();
  IRCServer &irc = IRCServer::getInstance();
  User *user = users.getUser(eventSocket);
  if (!user) return;

  std::vector<Channel *> channels = user->getChannels();
  for (std::vector<Channel *>::iterator it = channels.begin();
       it != channels.end(); ++it) {
    (*it)->send(*user, ":" + user->getFullName() + " QUIT :QUIT: disconnected");
    user->part(**it);
    if ((*it)->isEmpty()) {
      ChannelRepository::getInstance().removeChannel(**it);
    }
  }
  irc.disconnect(eventSocket);
}

void IRCServer::readCallback(fd eventSocket) {
  IRCServer &irc = IRCServer::getInstance();
  UserRepository &userRepo = UserRepository::getInstance();
  User *user = userRepo.getUser(eventSocket);
  if (!user) return;
  std::vector<std::string> messages;
  try {
    messages = user->receive();
  } catch (std::exception &e) {
    std::vector<Channel *> channels = user->getChannels();
    for (std::vector<Channel *>::iterator it = channels.begin();
         it != channels.end(); ++it) {
      (*it)->send(*user,
                  ":" + user->getFullName() + " QUIT :Quit: disconnected");
      user->part(**it);
      if ((*it)->isEmpty()) {
        ChannelRepository::getInstance().removeChannel(**it);
      }
    }
    irc.disconnect(eventSocket);
    return;
  }
  Parser parser;
  for (std::vector<std::string>::iterator it = messages.begin();
       it != messages.end(); ++it) {
    std::string msg = *it;
    Message currMsg = parser.parseMessage(*it);
    if (irc.events.find(currMsg.getCommand()) != irc.events.end()) {
      if (Middleware::authentifications(eventSocket, &currMsg)) {
        continue;
      }
      if (Middleware::registrations(eventSocket, &currMsg)) {
        continue;
      }

      irc.events[currMsg.getCommand()](eventSocket, &currMsg);

      Middleware::doWelcome(eventSocket, &currMsg);
    } else {
      irc.send(eventSocket,
               ERR_UNKOWNCOMMAND(user->getNickname(), currMsg.getCommand()));
    }
  }
}

void IRCServer::writeCallback(fd eventSocket) {
  IRCServer &irc = IRCServer::getInstance();
  UserRepository &userRepo = UserRepository::getInstance();
  User *user = userRepo.getUser(eventSocket);
  ChannelRepository &channelRepo = ChannelRepository::getInstance();
  if (!user) return;

  try {
    if (!user->sendBufferFlush()) {
      irc.enableWriteEvent(eventSocket);
      return;
    }
  } catch (std::exception &e) {
    std::vector<Channel *> channels = user->getChannels();
    for (std::vector<Channel *>::iterator it = channels.begin();
         it != channels.end(); ++it) {
      (*it)->send(*user,
                  ":" + user->getFullName() + " QUIT :Quit: disconnected");
      user->part(**it);
      if ((*it)->isEmpty()) {
        channelRepo.removeChannel(**it);
      }
    }
    irc.disconnect(eventSocket);
    return;
  }

  if (user->isQuit()) {
    irc.disconnect(eventSocket);
  }
}
