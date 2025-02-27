/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jewlee <jewlee@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 02:00:19 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/27 15:07:15 by jewlee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Command: PING
// Parameters: <token>

// 서버는 클라이언트의 연결 여부를 확인하기 위해 PING token의 요청을 보낸다.
// 혹은 클라이언트의 PING 신청에 PONG을 보낸다.

// 이때 token은 server에서 일방적으로 지정하는 문자열이다. or 반대의 경우
// 또한, client 마다 해당 명령어의 구현이 일부 상이함으로 token에는 그 어떤
// 암호화 방식을 도입해선 안된다.
// -> PING abcd 일 경우, PONG abcd로 응답

// PING-PONG은 Server-Client 양방향으로 사용가능하다.

// ERR_NEEDMOREPARAMS (461) - 파람 부족
// "<client> <command> :Not enough parameters"
// ERR_NOORIGIN (409) - 클라가 PING을 신청했는데 token이 없을 때
// "<client> :No origin specified"

#include "../parser/Message.hpp"
#include "IRCCommand.hpp"

namespace IRCCommand {
void ping(int clientSocket, void *message) {
  Message *msg = static_cast<Message *>(message);
  IRCServer &server = IRCServer::getInstance();
  UserRepository &users = UserRepository::getInstance();
  User *user = users.getUser(clientSocket);

  if (msg->getParams().size() == 0) {
    user->send(ERR_NEEDMOREPARAMS(user->getNickname(), msg->getCommand()));
  } else {
    std::string token = msg->getParams()[0];
    // TODO: when servername is defined, use that instead of inline string
    user->send("PONG NIRC :" + token);
  }
  server.enableWriteEvent(clientSocket);
}

}  // namespace IRCCommand