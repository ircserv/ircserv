/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minhulee <minhulee@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 00:27:02 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/20 01:00:28 by minhulee         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

// Command: KICK
// Parameters: <channel> <user> *( "," <user> ) [<comment>]

// 해당 채널에서 user를 강제로 제거합니다.

// 파라미터 부족한 경우, ERR_NEEDMOREPARAMS (461)
// 해당 채널이 없는 경우, ERR_NOSUCHCHANNEL (403)
// 해당 채널의 권한이 없는 경우, ERR_CHANOPRIVSNEEDED (482)
// 해당 채널에 대상이 없는 경우, ERR_USERNOTINCHANNEL (441) -> 킥의 대상을 의미합니다.
// KICK 실행자가 해당 채널에 없는 경우, ERR_NOTONCHANNEL (442)

// 여러명의 유저를 동시에 KICK한 경우, 모든 사용자를 포함한 일련의 메세지를 전송해야 합니다.
// -> nummeric으로 정의되진 않았습니다.

// ex) KICK <channel> <users> <comment>