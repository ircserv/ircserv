/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yechakim <yechakim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 07:27:35 by yechakim          #+#    #+#             */
/*   Updated: 2025/02/20 09:02:39 by yechakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCCommand.hpp"
#include <iostream>
/**
 * CAP Comamand
 * 
 * 아래와 같은 요청이 왔을때 요청을 처리합니다.
 * - CAP LS
 * - CAP LS 302
 */
namespace IRCCommand {
  void cap(fd clientSocket, void* message){
    Message *msg = (Message*)message;
    std::vector<std::string> params = msg->getParams();
    IRCServer &ircServer = IRCServer::getInstance();
    UserRepository &users = UserRepository::getInstance();
    User *user = users.getUser(clientSocket);

    std::cout << "[EVENT] CAP" << params.size() << std::endl;
    if (params.size() >=2 && params[0] == "LS"){
      user->send("CAP * LS :");
      ircServer.enableWriteEvent(clientSocket);
    }
  }
}