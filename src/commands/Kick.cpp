/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewlee <jewlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 00:27:02 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/27 15:07:09 by jewlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Command: KICK
// Parameters: <channel> <user> *( "," <user> ) [<comment>]

// 해당 채널에서 user를 강제로 제거합니다.

// 파라미터 부족한 경우, ERR_NEEDMOREPARAMS (461)
// 해당 채널이 없는 경우, ERR_NOSUCHCHANNEL (403)
// 해당 채널의 권한이 없는 경우, ERR_CHANOPRIVSNEEDED (482)
// 해당 채널에 대상이 없는 경우, ERR_USERNOTINCHANNEL (441) -> 킥의 대상을
// 의미합니다. KICK 실행자가 해당 채널에 없는 경우, ERR_NOTONCHANNEL (442)

// 여러명의 유저를 동시에 KICK한 경우, 모든 사용자를 포함한 일련의 메세지를
// 전송해야 합니다.
// -> nummeric으로 정의되진 않았습니다.

// ex) KICK <channel> <users> <comment>

#include "IRCCommand.hpp"

namespace IRCCommand {
void kick(int clientSocket, void *message) {
  Message *msg = static_cast<Message *>(message);
  std::vector<std::string> params = msg->getParams();
  UserRepository &userRepo = UserRepository::getInstance();
  ChannelRepository &channelRepo = ChannelRepository::getInstance();
  User *user = userRepo.getUser(clientSocket);

  if (params.size() < 2) {
    return user->send(ERR_NEEDMOREPARAMS(user->getNickname(), "KICK"));
  }
  std::string channelName = params[0];
  std::vector<std::string> kickedUsers = utils::split(params[1], ',');
  std::string comment = params.size() > 2 ? params[2] : "";

  if (!channelRepo.hasChannel(channelName)) {
    return user->send(ERR_NOSUCHCHANNEL(user->getNickname(), channelName));
  }

  Channel *channel = channelRepo.getChannel(channelName);
  if (!channel->isOperator(*user)) {
    return user->send(ERR_CHANOPRIVSNEEDED(user->getNickname(), channelName));
  }

  if (!channel->hasUser(*user)) {
    return user->send(ERR_NOTONCHANNEL(user->getNickname(), channelName));
  }

  for (std::vector<std::string>::iterator it = kickedUsers.begin();
       it != kickedUsers.end(); ++it) {
    User *target = userRepo.getUser(*it);
    if (!target || !channel->hasUser(*target)) {
      user->send(ERR_USERNOTINCHANNEL(user->getNickname(), *it, channelName));
      continue;
    }
    std::string broadcastMessage = ":" + user->getNickname() + " KICK " +
                                   channelName + " " + target->getNickname();
    if (!comment.empty()) broadcastMessage += " :" + comment;
    channel->broadcast(broadcastMessage);
    channel->kick(*target);
  }
}
}  // namespace IRCCommand
