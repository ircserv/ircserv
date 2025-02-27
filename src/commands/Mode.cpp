#include "IRCCommand.hpp"

bool validkey(char mode, std::string key);

namespace IRCCommand {
void mode(int clientSocket, void *message) {
  Message *msg = static_cast<Message *>(message);
  std::vector<std::string> params = msg->getParams();
  UserRepository &userRepo = UserRepository::getInstance();
  ChannelRepository &channelRepo = ChannelRepository::getInstance();
  User *user = userRepo.getUser(clientSocket);
  std::string response = ":" + user->getNickname() + " MODE ";

  if (params.size() < 2) {
    return user->send(ERR_NEEDMOREPARAMS(user->getNickname(), "MODE"));
  }

  std::string target = params[0];
  std::string modes = params.size() > 1 ? params[1] : "";
  std::vector<std::string> keys = params.size() > 2
                                      ? utils::split(params[2], ',')
                                      : std::vector<std::string>();

  if (target[0] != '#' && target[0] != '&') {  // target = user;
    if (!userRepo.getUser(target)) {
      return user->send(ERR_NOSUCHNICK(user->getNickname(), target));
    }
    if (target != user->getNickname()) {
      return user->send(ERR_USERSDONTMATCH(user->getNickname()));
    }
    if (modes.empty()) {
      return user->send(RPL_UMODEIS(user->getNickname(), "MODE"));
    }
    return user->send(ERR_UMODEUNKNOWNFLAG(user->getNickname()));
  }

  // TARGET = CHANNEL
  if (!channelRepo.hasChannel(target)) {
    return user->send(ERR_NOSUCHCHANNEL(user->getNickname(), target));
  }

  if (!user->isJoined(target)) {
    return user->send(ERR_NOTONCHANNEL(user->getNickname(), target));
  }

  Channel *channel = channelRepo.getChannel(target);

  if (modes.empty()) {
    return user->send(RPL_CHANNELMODEIS(user->getNickname(), target,
                                        channel->getModeString(), ""));
  }

  if (!channel->isOperator(*user)) {
    return user->send(ERR_CHANOPRIVSNEEDED(user->getNickname(), target));
  }

  if (modes.find_first_of("lkoit") == std::string::npos ||
      modes.find_first_of("+-") == std::string::npos) {
    return user->send(ERR_UMODEUNKNOWNFLAG(user->getNickname()));
  }
  bool initial = false;
  bool flag = false;
  size_t keylen = keys.size();
  size_t keyIdx = 0;
  for (std::string::iterator it = modes.begin(); it != modes.end(); ++it) {
    if (*it == '+' || *it == '-') {
      initial = true;
      flag = *it == '+' ? true : false;
      continue;
    }
    if (!initial) continue;
    if (*it == 't' || *it == 'i') {
      channel->setMode(*it, flag, NULL);
      channel->broadcast(response + target + " " + (flag ? "+" : "-") + *it);
      continue;
    }

    if (*it == 'l' || *it == 'k') {
      if (!flag) {
        channel->setMode(*it, flag, NULL);
        channel->broadcast(response + target + " " + (flag ? "+" : "-") + *it);
        continue;
      }
      // set mode +
      if (keyIdx >= keylen) {
        continue;
      }
      if (!validkey(*it, keys[keyIdx])) {
        user->send(ERR_INVALIDMODEPARAM(user->getNickname(), target, *it,
                                        keys[keyIdx]));
      } else {
        channel->setMode(*it, flag, &keys[keyIdx]);
        channel->broadcast(response + target + " " + (flag ? "+" : "-") + *it +
                           " " + keys[keyIdx]);
      }
      keyIdx++;
      continue;
    }
    if (*it == 'o') {
      if (keyIdx >= keylen) {
        continue;
      }
      if (!channel->hasUser(keys[keyIdx])) {
        user->send(
            ERR_USERNOTINCHANNEL(user->getNickname(), keys[keyIdx], target));
        continue;
      }
      channel->setMode(*it, flag, &keys[keyIdx]);
      channel->broadcast(response + target + " " + (flag ? "+" : "-") + *it +
                         " " + keys[keyIdx]);
      keyIdx++;
    }
  }

}  // mode function
}  // namespace IRCCommand

bool validkey(char mode, std::string key) {
  if (mode == 'l') {
    return utils::isNumber(key) && std::strtod(key.c_str(), NULL) > 0;
  }
  return (key.find_first_of("\0\r\n\a:,#&") == std::string::npos);
}