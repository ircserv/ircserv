/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewlee <jewlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 01:00:59 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/27 15:07:17 by jewlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Command: PRIVMSG
// Parameters: <target>{,<target>} <text to be sent>

// 해당 타켓에게 메세지를 전송합니다.

// 해당 타켓이 없을 때, ERR_NOSUCHNICK (401)
// 해당 채널에 보낼 수 없을때(여러가지 이유로), ERR_CANNOTSENDTOCHAN (404)
// 해당 커맨드의 타켓이 서버의 정의보다 수가 많을 때, ERR_TOOMANYTARGETS (407)
// 타겟이 없는 경우, ERR_NORECIPIENT (411)
// 텍스트가 없는 경우, ERR_NOTEXTTOSEND (412)

#include "../utils/utils.hpp"
#include "IRCCommand.hpp"

namespace IRCCommand {
void privmsg(fd clientSocket, void *message) {
  IRCServer &server = IRCServer::getInstance();
  UserRepository &userRepo = UserRepository::getInstance();
  ChannelRepository &channelRepo = ChannelRepository::getInstance();
  Message *msg = static_cast<Message *>(message);
  std::vector<std::string> params = msg->getParams();
  User *user = userRepo.getUser(clientSocket);

  if (params.size() < 2) {
    user->send(ERR_NEEDMOREPARAMS(user->getNickname(), msg->getCommand()));
    server.enableWriteEvent(clientSocket);
    return;
  }

  std::vector<std::string> targets = utils::split(params[0], ',');
  std::string text = params[1];
  std::vector<std::string>::iterator target;

  for (target = targets.begin(); target != targets.end(); target++) {
    bool toOper = false;
    std::string targetName = *target;
    std::string message =
        ":" + user->getNickname() + " " + msg->getCommand() + " ";

    if (userRepo.hasUser(targetName)) {
      User *targetUser = userRepo.getUser(targetName);
      targetUser->send(message + targetName + " :" + text);
      continue;
    }

    if (targetName[0] == '@') {
      toOper = true;
      targetName.erase(0, 1);
    }

    if (!channelRepo.hasChannel(targetName)) {
      user->send(ERR_NOSUCHNICK(user->getNickname(),
                                (toOper ? "@" : "") + targetName));
      continue;
    }

    Channel *channel = channelRepo.getChannel(targetName);

    if (!user->isJoined(targetName)) {
      user->send(ERR_CANNOTSENDTOCHAN(user->getNickname(), targetName));
      continue;
    }

    if (toOper) {
      channel->toOperators(*user, message + targetName + " :" + text);
    } else {
      channel->send(*user, message + targetName + " :" + text);
    }
  }
}
}  // namespace IRCCommand