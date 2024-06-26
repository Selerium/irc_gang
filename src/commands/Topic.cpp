#include "../../include/IRC.hpp"

IRC::Topic::Topic() : setChannel(""), topic("")
{}

IRC::Topic::~Topic(){}

void IRC::Topic::excuteTopic(Parse *parse, Client* client, Server* server)
{
	if	(client->getAuthantication() == false) {
		client->SendServerToClient(" : " ERROR_451 " " + client->getNickname() + " :You have not registered\r\n");
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
		client->SendServerToClient(": " ERR_NEEDMOREPARAMS " " + client->getNickname() 
		+ " TOPIC " ":Not enough parameters\r\n");
		return 1;
	}

	setChannel = parameters[0];
	bool textFound = false;
    std::vector<std::string>::iterator it;
    for (it = parameters.begin() + 1; it != parameters.end(); ++it)
    {
        if (!it->empty())
        {
            if ((*it)[0] == ':' && !textFound)
            {
                *it = it->substr(1);
                textFound = true;
            }
            else if ((*it)[0] != ':' && !textFound)
			{
				client->SendServerToClient(": " ERR_NEEDMOREPARAMS " " + client->getNickname() 
				+ " TOPIC " ":Not enough parameters\r\n");
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
			if (it->second->getChannelName() == setChannel )
			{
				if (it->second->FindClient(client->getNickname()) != NULL)
				{
					// add condition depending on Topic permissions:
					if (it->second->_clients.find(client)->second == 0)
					{
						client->SendServerToClient(": " ERR_CHANOPRIVSNEEDED " " + client->getNickname() + " " + setChannel
							+ " :You're not channel operator\r\n");
						return;
					}
					if (topic == "")
					{
						if (it->second->getTopic() == "")
							client->SendServerToClient(": " RPL_NOTOPIC " " + client->getNickname() + " " + setChannel
							+ " :No topic is set\r\n");
						else 
							it->second->setwhosetTopic(client->getNickname());
						client->SendServerToClient(": " RPL_TOPIC " " + client->getNickname() 
							+ " " + setChannel + ":" + it->second->getTopic() + "\r\n");
						client->SendServerToClient(": " RPL_TOPICWHOTIME " " + client->getNickname() 
						+ " " + setChannel + " " + it->second->getwhosetTopic() +  "\r\n");
						return;
					}
					else if (topic ==  " ")
						it->second->setTopic("");
					else
						it->second->setTopic(topic);
					it->second->sendToall( client->getNickname() + " changed the topic of " + setChannel + " to :" +  it->second->getTopic() + "\r\n");
				}
				else 
					client->SendServerToClient(": " ERR_NOTONCHANNEL " " + client->getNickname() + " "
					+ setChannel + " :You're not on that channel\r\n");
			}
		}
	}
	client->SendServerToClient(": " ERR_NOSUCHCHANNEL " " + client->getNickname() + " " 
	+ setChannel + " :No such channel\r\n");
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