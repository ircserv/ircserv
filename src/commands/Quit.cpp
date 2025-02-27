/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewlee <jewlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 02:10:09 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/27 15:07:19 by jewlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Command: QUIT
// Parameters: [<reason>]

// 해당 유저를 채널에서 내보내고 서버 연결을 종료한다.
// 이때, 해당 유저와 유저가 공유하는 채널에 퇴장 메세지를 전송해야 한다. -> PART

// 성공시 응답 -> "Quit: " or "Quit: <reson>"
// 이때, 사용자가 QUIT을 통해 연결을 종료한 게 아니라, PING-PONG 혹은 강제
// 종료로 종료된 경우 그에 맞는 reason을 작성해서 보낸다 -> 맘대로 해라.

#include "IRCCommand.hpp"

namespace IRCCommand {
void quit(int clientSocket, void *message) {
  Message *msg = static_cast<Message *>(message);
  UserRepository &userRepo = UserRepository::getInstance();
  ChannelRepository &channelRepo = ChannelRepository::getInstance();
  User *user = userRepo.getUser(clientSocket);
  std::string responseMessage = "ERROR :Closing Connection";
  std::string broadcastMessage = "QUIT :Quit:";
  if (msg->getParams().size() == 0) {
    user->send(responseMessage);
  } else {
    user->send(responseMessage + ": " + msg->getParams().front());
    broadcastMessage += " " + msg->getParams().front();
  }

  std::vector<Channel *> channels = user->getChannels();
  for (std::vector<Channel *>::iterator it = channels.begin();
       it != channels.end(); ++it) {
    (*it)->send(*user, ":" + user->getNickname() + " " + broadcastMessage);
    user->part(**it);
    if ((*it)->isEmpty()) {
      channelRepo.removeChannel(**it);
    }
  }
  std::set<std::string> invitedChannels = user->getInvitedChannels();
  for (std::set<std::string>::iterator it = invitedChannels.begin();
       it != invitedChannels.end(); ++it) {
    Channel *channel = channelRepo.getChannel(*it);
    if (channel) {
      channel->uninvite(*user);
    }
  }
  user->quit();
}
}  // namespace IRCCommand