#include "../../include/IRC.hpp"

IRC::Kick::Kick(): setChannel(""), setkickNick(""), comment(""){}

IRC::Kick::~Kick(){}

void IRC::Kick::excuteKick(Parse *parse, Client* client, Server* server)
{
	if	(client->isregisterd() == false) {
		client->SendServerToClient(": " ERROR_451 " :You have not registered");
		return ;
	}

	if (parseMsg(parse, client) != 1)
		checkChannel(client, server);
}

int IRC::Kick::parseMsg(Parse *parse, Client* client)
{
	std::vector<std::string> parameters = parse->getParameters();
	if (parameters.empty() || parameters.size() < 2 || parameters[0][0] == ':' || parameters[1][0] == ':')
	{
		client->SendServerToClient(":" + client->getNickname() + client->getUsername() + " " ERR_NEEDMOREPARAMS " KICK " ":Not enough parameters");
		return 1;
	}

	setChannel = parameters[0];
	setkickNick = parameters[1];

	bool textFound = false;
    std::vector<std::string>::iterator it;
	for (it = parameters.begin() + 2; it != parameters.end(); ++it)
    {
        if (!it->empty())
        {
            if ((*it)[0] == ':' && !textFound)
            {
                *it = it->substr(1);
                textFound = true;
            }
            comment += (*it + " ");
        }
    }
	return 0;
}

void IRC::Kick::checkChannel(Client* client, Server* server)
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
					if (it->second->_clients.find(client)->second == 0)
					{
						client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_CHANOPRIVSNEEDED " KICK " + setChannel + " :You're not channel operator");
						return;
					}
					else
					{
						if (it->second->FindClient(setkickNick) != NULL)
						{
							// it->second->FindClient(setkickNick)->SendServerToClient("You were kicked by " + client->getNickname() + " (Reason:  " + comment + ")");
							it->second->sendToall(":" + client->getHostname() +"!" + client->getUsername() + " KICK " + it->second->getChannelName() + " " + setkickNick + " :" + comment);
							it->second->removeNick(client, setkickNick);
							return ;
						}
						else
							client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_USERNOTINCHANNEL " KICK " + setChannel + " :They aren't on that channel");
					}
				}
				else 
					client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " 442 KICK " + setChannel + " :You're not on that channel");
				return;
			}
		}
	}
	client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_NOSUCHCHANNEL " KICK " + setChannel + " :No such channel");
	return;

}




/*
ERR_NEEDMOREPARAMS 461
ERR_NOSUCHCHANNEL (403)
ERR_CHANOPRIVSNEEDED 482
ERR_USERNOTINCHANNEL (441)
ERR_NOTONCHANNEL (442) 

*/