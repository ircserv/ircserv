/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yechakim <yechakim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 07:32:10 by yechakim          #+#    #+#             */
/*   Updated: 2025/02/20 01:22:42 by yechakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "commands/IRCCommand.hpp"

int main(){
  IRCServer &server = IRCServer::getInstance();
  server.on(CMD_CAP, IRCCommand::cap);
  server.on(CMD_JOIN, IRCCommand::join);
  server.on(CMD_PASS, IRCCommand::pass);
  server.on(CMD_NICK, IRCCommand::nick);
  server.on(CMD_USER, IRCCommand::user);
  server.on(CMD_PING, IRCCommand::ping);
  server.start(6667);
  server.destroy();
  return 0;
}