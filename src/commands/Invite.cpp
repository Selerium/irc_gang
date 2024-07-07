#include "../../include/IRC.hpp"

IRC::Invite::Invite(): setChannel(""), setInviteNick(""){}

IRC::Invite::~Invite(){}

void IRC::Invite::excuteInvite(Parse *parse, Client* client, Server* server)
{
	if	(client->isregisterd() == false) {
		client->SendServerToClient(": " ERROR_451 " INVITE :You have not registered");
		return ;
	}

	if (parseMsg(parse, client) != 1)
		if (checkUser(client , server) != 1)
			checkChannel(client, server);
}

int IRC::Invite::parseMsg(Parse *parse, Client* client)
{
	std::vector<std::string> parameters = parse->getParameters();
	if (parameters.empty() || parameters.size() < 2)
	{
		client->SendServerToClient(": " ERR_NEEDMOREPARAMS " " + client->getNickname() + "!" + client->getUsername() + " INVITE :Not enough parameters");
		return 1;
	}
	setInviteNick = parameters[0];
	setChannel = "#" + parameters[1];
	return (0);

}

void IRC::Invite::checkChannel(Client* client, Server* server)
{
	Parse parse;

	parse.Debug_msg("user found! inviting...");
	if (!server->channel_map.empty())
	{
		std::map<int, Channel *>::iterator it;
		for (it = server->channel_map.begin(); it !=server->channel_map.end(); ++it)
		{
			if (it->second->getChannelName() == setChannel)
			{
				if (it->second->FindClient(client->getNickname()) != NULL)
				{
					if (it->second->getInviteMode() == true && it->second->_clients.find(client)->second == 0)
					{
						client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_CHANOPRIVSNEEDED " INVITE :You're not channel operator");
						return;
					}
					else
					{
						if (it->second->FindClient(setInviteNick) != NULL)
							client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_USERONCHANNEL " INVITE :User is already on channel");
						else {
							for (std::map<int, Client *>::iterator it2 = server->clients_map.begin(); it2 != server->clients_map.end(); it2++) {
								if (it2->second->getNickname() == setInviteNick) {
									it2->second->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " 341 INVITE " + setInviteNick + " " + setChannel);
									client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " 341 INVITE " + setInviteNick + " " + setChannel);
									it->second->_clients.insert(std::make_pair(it2->second, 2));
									it->second->_clientAmount++;
									return ;
								}
							}
						}
						return ;
					}
				}
				else 
					client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_NOTONCHANNEL " INVITE " + setInviteNick + " " + setChannel + " :You're not on that channel");
				return;
			}
		}
	}
	client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_NOSUCHCHANNEL " INVITE " + setInviteNick +  " " + setChannel + " :No such channel");
	return;
}

int IRC::Invite::checkUser(Client* client ,Server* server)
{
	Parse parse;

	parse.Debug_msg("Checking for the user");
	std::map<int , Client *>::iterator it;
	if (!server->clients_map.empty())
	{
		for (it = server->clients_map.begin(); it != server->clients_map.end() ;++it)
		{
			parse.Debug_msg(it->second->getNickname() + " - " + setInviteNick);
			if (it->second->getNickname() == setInviteNick)
				return 0;
		}
	}
	client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_NOSUCHNICK " INVITE " + setInviteNick +  " " + setChannel + ":No such nick/channel");
	return 1;
}





/*
RPL_INVITING (341)
"<client> <nick> <channel>"
	Sent as a reply to the INVITE command to indicate that the attempt 
	was successful and the client with the nickname <nick> has been invited to <channel>.
ERR_NEEDMOREPARAMS (461)
ERR_NOSUCHCHANNEL (403)
ERR_NOTONCHANNEL (442)
ERR_CHANOPRIVSNEEDED (482)
ERR_USERONCHANNEL (443) 
  "<client> <nick> <channel> :is already on channel"
Returned when a client tries to invite <nick> to a channel they’re already joined to.
*/