#include "../../include/IRC.hpp"

IRC::Topic::Topic() : setChannel(""), topic("")
{}

IRC::Topic::~Topic(){}

void IRC::Topic::excuteTopic(Parse *parse, Client* client, Server* server)
{
	if	(client->isregisterd() == false) {
		client->SendServerToClient(" : " ERROR_451 " " + client->getNickname() + " :You have not registered");
		return ;
	}

	if (parseMsg(parse, client) != 1)
		checkChannel(client, server);
}
int IRC::Topic::parseMsg(Parse *parse, Client* client)
{
	std::vector<std::string> parameters = parse->getParameters();
	if (parameters.empty() || parameters[0][0] == ':')
	{
		client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_NEEDMOREPARAMS " TOPIC :Not enough parameters");
		return 1;
	}

	setChannel = parameters[0];
	bool textFound = false;
	std::vector<std::string>::iterator it;
	for (it = parameters.begin() + 1; it != parameters.end(); ++it)
	{
		parse->Debug_msg(*it);
		if (!it->empty())
		{
			if (!textFound && (*it)[0] == ':')
			{
				*it = it->substr(1);
				textFound = true;
			}
			else if (!textFound && (*it)[0] != ':')
			{
				client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_NEEDMOREPARAMS " TOPIC :Not enough parameters");
				return 1;
			}
			topic += (*it + " ");
		}
	}
	return 0;
}

void IRC::Topic::checkChannel(Client* client, Server* server)
{
	if (!server->channel_map.empty())
	{
		std::map<int, Channel *>::iterator it;
		for (it = server->channel_map.begin(); it !=server->channel_map.end(); ++it)
		{
			if (it->second->getChannelName().substr(1) == setChannel)
			{
				if (it->second->FindClient(client->getNickname()) != NULL)
				{
					// add condition depending on Topic permissions:
					if (it->second->getTopicMode() && it->second->_clients.find(client)->second == 0 && topic.size())
					{
						client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " + ERR_CHANOPRIVSNEEDED " TOPIC " + setChannel + " :You're not channel operator");
						return;
					}
					if (topic == "")
					{
						if (it->second->getTopic() == "") {
							client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " RPL_NOTOPIC " TOPIC " + setChannel + " :No topic is set");
							return ;
						}
						else 
							it->second->setwhosetTopic(client->getNickname());
						client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " RPL_TOPIC " TOPIC " + setChannel + " :" + it->second->getTopic());
						// client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " RPL_TOPICWHOTIME " TOPIC " + setChannel + " :" + it->second->getwhosetTopic());
						return;
					}
					else if (topic[0] ==  ':' || topic[0] == ' ') {
						it->second->setTopic(client, "");
						it->second->sendToall(":" + client->getNickname() + "!" + client->getUsername() + " 332 TOPIC #" + setChannel);
						return ;
					}
					else
						it->second->setTopic(client, topic);
					it->second->sendToall(":" + client->getNickname() + "!" + client->getUsername() + " 332 TOPIC #" + setChannel + " :" +  it->second->getTopic());
					return ;
				}
				else {
					client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " 442 TOPIC #" + setChannel + " :You're not on that channel");
					return ;
				}
			}
		}
	}
	client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_NOSUCHCHANNEL " TOPIC " + setChannel + " :No such channel");
	return;
}
/*
ERR_NEEDMOREPARAMS (461) 
  "<client> <command> :Not enough parameters" 


ERR_NOSUCHCHANNEL (403) 
  "<client> <channel> :No such channel"


  ERR_NOTONCHANNEL (442) 
  "<client> <channel> :You're not on that channel"

  ERR_CHANOPRIVSNEEDED (482) 
  "<client> <channel> :You're not channel operator"


RPL_NOTOPIC (331) 
  "<client> <channel> :No topic is set"


  RPL_TOPIC (332) 
  "<client> <channel> :<topic>"

  PL_TOPICWHOTIME (333) 
  "<client> <channel> <nick> <setat>"
Sent to a client to let them know who set the topic (<nick>) and when they set it (<setat> is a unix timestamp). Sent after RPL_TOPIC (332).
*/