/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minhulee <minhulee@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 01:00:59 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/20 02:00:39 by minhulee         ###   ########seoul.kr  */
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