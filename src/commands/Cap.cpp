/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cap.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yechakim <yechakim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 07:27:35 by yechakim          #+#    #+#             */
/*   Updated: 2025/02/17 11:54:44 by yechakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCCommand.hpp"
#include <iostream>

namespace IRCCommand{
  void cap(fd clientSocket, void* message){
    (void)message;
    IRCServer &ircServer = IRCServer::getInstance();
    UserRepository &users = UserRepository::getInstance();
    users.getUser(clientSocket)->send("CAP * LS:");
    ircServer.enableWriteEvent(clientSocket);
  }
}