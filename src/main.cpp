/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yechakim <yechakim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 07:32:10 by yechakim          #+#    #+#             */
/*   Updated: 2025/02/17 11:55:08 by yechakim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCServer.hpp"
#include "commands/IRCCommand.hpp"

int main(){
  IRCServer &server = IRCServer::getInstance();
  server.on("CAP", IRCCommand::cap);
  server.on("JOIN", IRCCommand::join);
  server.on("PASS", IRCCommand::pass);
  server.on("NICK", IRCCommand::nick);
  server.on("USER", IRCCommand::user);
  server.start(6667);
  server.destroy();
  return 0;
}