// Command: TOPIC
// Parameters: <channel> [<topic>]

#include "IRCCommand.hpp"
#include "Response.hpp"

namespace IRCCommand {
	void	topic(int clientSocket, void *message)
	{
		Message *msg = static_cast<Message *>(message);
		UserRepository &userRepo = UserRepository::getInstance();
		ChannelRepository	&channelRepo = ChannelRepository::getInstance();
		User	*user = userRepo.getUser(clientSocket);
		std::vector<std::string>	param = msg->getParams();
		std::string	nick = user->getNickname(), cmd = msg->getCommand();

		// pram이 없는 경우
		if (param.size() == 0)
			user->send(ERR_NEEDMOREPARAMS(nick, cmd));

		Channel	*channel = channelRepo.getChannel(param[0]);
		// 토픽의 대상이 되는 채널이 없는 경우
		if (!channel)
			user->send(ERR_NOSUCHCHANNEL(nick, msg->getParams()[0]));

		std::string	channelName = channel->getName();
		// 실행자가 해당 채널에 등록되지 않은 경우
		if (!user->isJoined(channelName))
			user->send(ERR_NOTONCHANNEL(nick, channelName));

		// 토픽 조회 및 설정 분리
		std::string	topic = channel->getTopic();
		if (param.size() == 1)
		{
			// 토픽 조회
			if (topic.empty())
				user->send(RPL_NOTOPIC(nick, channelName));
			else
			{
				user->send(RPL_TOPIC(nick, channelName, topic));
				user->send(RPL_TOPICWHOTIME(user->getUsername(), channelName, nick, "0:0"));
			}
		}
		else
		{
			// 토픽 설정 -> 유저 권한 확인 필요 -> 모드 미설정 시 모든 사용자가 가능
			if (0 && channel->isOperator(*user))
				user->send(ERR_CHANOPRIVSNEEDED(nick, channelName));

			// TOPIC #chan : -> 토픽을 제거하라는 의미
			if (param[1] == ":")
				channel->setTopic("");
			else
			{
				topic = param[1];
				for (size_t i = 2; i < param.size(); i++)
					topic += param[i];
				channel->setTopic(topic);

				topic = channel->getTopic();
				//:minhulee!minhulee@127.0.0.1 TOPIC #c :sex
				channel->send(*user, ":" + nick + " TOPIC " + channelName + " :" + topic);
				user->send(RPL_TOPIC(nick, channelName, topic));
				user->send(RPL_TOPICWHOTIME(user->getUsername(), channelName, nick, "0:0"));
			}
		}
	}
}