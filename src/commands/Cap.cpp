/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewlee <jewlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 07:27:35 by yechakim          #+#    #+#             */
/*   Updated: 2025/02/27 15:07:01 by jewlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCCommand.hpp"

/**
 * CAP Comamand
 *
 * 아래와 같은 요청이 왔을때 요청을 처리합니다.
 * - CAP LS
 * - CAP LS 302
 */
namespace IRCCommand {
void cap(fd clientSocket, void *message) {
  Message *msg = (Message *)message;
  std::vector<std::string> params = msg->getParams();
  IRCServer &ircServer = IRCServer::getInstance();
  UserRepository &users = UserRepository::getInstance();
  User *user = users.getUser(clientSocket);

  if (params.size() >= 2 && params[0] == "LS") {
    user->send("CAP * LS :");
    ircServer.enableWriteEvent(clientSocket);
  }
}
}  // namespace IRCCommand