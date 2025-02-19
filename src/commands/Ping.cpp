/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minhulee <minhulee@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 02:00:19 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/17 03:33:06 by minhulee         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

// Command: PING
// Parameters: <token>

// 서버는 클라이언트의 연결 여부를 확인하기 위해 PING token의 요청을 보낸다.
// 혹은 클라이언트의 PING 신청에 PONG을 보낸다.

// 이때 token은 server에서 일방적으로 지정하는 문자열이다. or 반대의 경우
// 또한, client 마다 해당 명령어의 구현이 일부 상이함으로 token에는 그 어떤 암호화 방식을 도입해선 안된다.
// -> PING abcd 일 경우, PONG abcd로 응답

// PING-PONG은 Server-Client 양방향으로 사용가능하다.

// ERR_NEEDMOREPARAMS (461) - 파람 부족
// "<client> <command> :Not enough parameters"
// ERR_NOORIGIN (409) - 클라가 PING을 신청했는데 token이 없을 때
// "<client> :No origin specified"