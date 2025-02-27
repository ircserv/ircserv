#include "middleware.hpp"

#include "../commands/IRCCommand.hpp"

#define GREEN "\033[32m"
#define RESET "\033[0m"
#define RED "\033[31m"
#define GRAY "\033[90m"

bool Middleware::authentifications(int client, void *msg) {
  std::set<std::string> excludedAuthentications =
      Middleware::getExcludedAuthentications();
  Message *message = static_cast<Message *>(msg);
  std::string command = message->getCommand();
  UserRepository &userRepo = UserRepository::getInstance();
  User *user = userRepo.getUser(client);

  if (excludedAuthentications.find(command) != excludedAuthentications.end()) {
    return false;
  }

  if (!user->isauthentified()) {
    std::string username =
        user->getNickname().empty() ? "unknown" : user->getNickname();
    user->send(ERR_PASSWDMISMATCH(username));
    return true;
  }
  return false;
}

bool Middleware::registrations(int client, void *msg) {
  std::set<std::string> excludedAuthentications =
      Middleware::getExcludedRegistrations();
  Message *message = static_cast<Message *>(msg);
  std::string command = message->getCommand();
  UserRepository &userRepo = UserRepository::getInstance();
  User *user = userRepo.getUser(client);

  if (excludedAuthentications.find(command) != excludedAuthentications.end()) {
    return false;
  }

  if (!user->isauthentified()) {
    std::string username =
        user->getNickname().empty() ? "unknown" : user->getNickname();
    user->send(ERR_PASSWDMISMATCH(username));
    return true;
  }

  if (!user->isRegistered()) {
    std::string username =
        user->getNickname().empty() ? "unknown" : user->getNickname();
    user->send(ERR_NOTREGISTERED(username));
    return true;
  }

  return false;
}

bool Middleware::doWelcome(int client, void *msg) {
  UserRepository &userRepo = UserRepository::getInstance();
  User *user = userRepo.getUser(client);
  Message *message = static_cast<Message *>(msg);
  std::string command = message->getCommand();
  IRCServer &server = IRCServer::getInstance();
  if (!(command == CMD_NICK || command == CMD_USER)) return false;

  if (user->isRegistered()) return false;
  if (!user->isauthentified()) return false;

  if (user->getRealname().empty() || user->getNickname().empty()) return false;

  std::string username = user->getNickname();
  user->setRegistered(true);
  user->send(RPL_WELCOME(username));
  user->send(RPL_YOURHOST(username));
  user->send(RPL_CREATED(username, server.getCreatedTime()));
  user->send(RPL_MYINFO(username));
  user->send(RPL_ISUPPORT(username));

  return true;
}

std::set<std::string> Middleware::getExcludedAuthentications() {
  std::set<std::string> excludedAuthentications;
  excludedAuthentications.insert(CMD_PASS);
  excludedAuthentications.insert(CMD_CAP);

  return excludedAuthentications;
}

std::set<std::string> Middleware::getExcludedRegistrations() {
  std::set<std::string> excludedRegistrations;
  excludedRegistrations.insert(CMD_NICK);
  excludedRegistrations.insert(CMD_USER);
  excludedRegistrations.insert(CMD_PASS);
  excludedRegistrations.insert(CMD_CAP);

  return excludedRegistrations;
}
