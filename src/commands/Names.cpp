#include "IRCCommand.hpp"

namespace IRCCommand {

void names(int clientSocket, void *message) {
  Message *msg = static_cast<Message *>(message);
  std::vector<std::string> params = msg->getParams();
  UserRepository &userRepo = UserRepository::getInstance();
  User *user = userRepo.getUser(clientSocket);
  ChannelRepository &chRepo = ChannelRepository::getInstance();

  if (params.size() == 0) {
    std::map<int, User> users = userRepo.getUsers();
    std::string userlist;
    for (std::map<int, User>::iterator it = users.begin(); it != users.end();
         ++it) {
      userlist += it->second.getNickname();
      if (it != --users.end()) {
        userlist += " ";
      }
    }
    user->send(RPL_NAMREPLY(user->getNickname(), "*", "", userlist));
    return user->send(RPL_ENDOFNAMES(user->getNickname(), "*"));
  }

  std::vector<std::string> channelNames = utils::split(params[0], ',');
  for (std::vector<std::string>::iterator it = channelNames.begin();
       it != channelNames.end(); ++it) {
    Channel *ch = chRepo.getChannel((*it));
    if (!ch) {
      user->send(ERR_NOSUCHCHANNEL(user->getNickname(), *it));
      continue;
    }
    std::string userlist;
    std::vector<User *> users = ch->getUsers();
    for (std::vector<User *>::iterator it = users.begin(); it != users.end();
         ++it) {
      userlist += (*it)->getNickname();
      if (it != --users.end()) {
        userlist += " ";
      }
    }
    user->send(RPL_NAMREPLY(user->getNickname(), ch->getSymbol(), ch->getName(),
                            userlist));
    user->send(RPL_ENDOFNAMES(user->getNickname(), ch->getName()));
  }
}
}  // namespace IRCCommand