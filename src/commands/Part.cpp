/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minhulee <minhulee@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 03:30:01 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/17 03:40:02 by minhulee         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

// Command: PART
// Parameters: <channel>{,<channel>} [<reason>]

// 유저가 해당 채널을 떠남
// 이때, 유저가 복수 개의 채널을 떠날 수 있음

// PART #twilight_zone ; leave channel "#twilight_zone"
// PART #oz-ops,&group5 ; leave both channels "&group5" and "#oz-ops".

// 그러나, 해당 유저가 속한 채널이 여러개 겹치는 유저가 있을 때
// 해당 유저에 대한 PART 메세지는 각 건이 별도로 전송되어야 함

// A가 #C #D를 떠날 때, 동일한 채널에 있는 B는 아래와 같이 받음
// :A!user@host PART #C :<reason>!
// :B!user@host PART #C :<reason>!

// ERR_NEEDMOREPARAMS (461) - 파람 부족
// 461 "<client> <command> :Not enough parameters"

// ERR_NOSUCHCHANNEL (403) - 채널 못찾음
// 403 "<client> <channel> :No such channel"

// ERR_NOTONCHANNEL (442) - 해당 채널에 안속함
// 442  "<client> <channel> :You're not on that channel"
