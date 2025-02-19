/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minhulee <minhulee@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 00:19:56 by minhulee          #+#    #+#             */
/*   Updated: 2025/02/20 00:25:31 by minhulee         ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

// Command: INVITE
// Parameters: <nickname> <channel>

// 해당 대상을 channel에 invite 합니다.

// 대상 채널이 존재하지 않는 경우, ERR_NOSUCHCHANNEL
// 커맨드를 실행한 유저가 해당 채널에 없는 경우, ERR_NOTONCHANNEL
// 해당 채널에 있으나 권한이 없는 경우, ERR_CHANOPRIVSNEEDED
// 사용자가 이미 대상 채널에 있는 경우, ERR_USERONCHANNEL

// 초대가 성공 시 RPL_INVITING를 커맨드 실행 유저에게
// 초대된 사용자에게 INVITE <target> <channel> :<source> 형식의 메세지를 전송
// -> 이때, source는 초대를 실행한 유저를 의미
// -> A가 B를 초대하고 성공한 경우
// A는 RPL_INVITING 응답을 전달
// B는 "INVITE B <channel> :A" 의 일련의 문자열을 전달
