#include "../../include/IRC.hpp"

IRC::Join::Join(){}

IRC::Join::~Join(){}

void IRC::Join::excuteJoin(Parse *parse, Client* client, Server* server)
{
	std::vector<std::string> parameter = parse->getParameters();

	// authentication check
	if	(client->isregisterd() == false) {
		client->SendServerToClient(": " ERROR_451 " localhost JOIN :You have not registered");
		return ;
	}
	if (parse->getParameters().empty() || parameter.size() == 0) {
		client->SendServerToClient(": " ERROR_461 " localhost JOIN :Please enter a valid channel name");
		return ;
	}

	if (parameter.size() > 1)
	{
		if (channelExist(parameter[0], server) == false) {
			createChannel(parameter[0], parameter[1], server, client);
			joinChannel(parameter[0], parameter[1], server, client);
		}
		else if (channelExist(parameter[0], server) == true && channelPass(parameter[0], parameter[1], server) == false)
			client->SendServerToClient(ERROR_475);
		else if (channelExist(parameter[0], server) == true && channelPass(parameter[0], parameter[1], server) == true)
			joinChannel(parameter[0], parameter[1], server, client);

	}
	else
	{
		if (channelExist(parameter[0], server) == false)
			createChannel(parameter[0], std::string(""), server, client);
		joinChannel(parameter[0], std::string(""), server, client); 
	}
}

void IRC::Join::joinChannel(std::string channelname, std::string pass, Server* server, Client* client)
{
	Parse parse;
	parse.Debug_msg(":" + client->getNickname() + "!" + client->getUsername() + " JOIN " + channelname.substr(1));

	std::map<int, Channel *>::iterator it;

	for(it = server->channel_map.begin(); it != server->channel_map.end(); it++)
	{
		if (it->second->getChannelName() == channelname)
		{
			if (it->second->getsetLimit() && it->second->_clientAmount == it->second->getLimit()) {
				client->SendServerToClient(":471 " + client->getNickname() + "!" + client->getUsername() + " JOIN " + " " + channelname + " :Cannot join channel (+l)");
				return ;
			}
			if (it->second->getInviteMode() && (!it->second->_clients.find(client)->first || it->second->checkPermission(client) != 2)) {
				client->SendServerToClient(":473 " + client->getNickname() + "!" + client->getUsername() + " JOIN " + " " + channelname + " :Cannot join channel (+i)");
				return ;
			}
			if (it->second->getChannelPassword().length() && it->second->getChannelPassword() != pass) {
				client->SendServerToClient(":475 " + client->getNickname() + "!" + client->getUsername() + " JOIN " + " " + channelname + " :Cannot join channel (+k)");
				return ;
			}
			it->second->addChanneluser(client);
			if (it->second->_clientAmount == 1)
				it->second->welcomeMsgChan1(client);
			else if (it->first != 2)
				it->second->welcomeMsgChan2(client);
			// it->second->sendToall(":" + client->getNickname() +"!" + client->getUsername() + " JOIN " + channelname);
		}
	}
}

void IRC::Join::createChannel(std::string channelname, std::string pass, Server* server, Client* client)
{
	Parse parse;
	parse.Debug_msg("Channel " + channelname + " created!");
	// if (server->channel_map.size() == 0)
	// 	server->channel_map.insert(std::make_pair(0, new Channel(channelname, pass)));
	// else
	// {
		// std::map<int, Channel *>::iterator it;
		// int	c = 0;
		// for(it = server->channel_map.begin(); it != server->channel_map.end(); it++)
		// 	c++;
	// }
	server->channel_map.insert(std::make_pair(server->channel_map.size(), new Channel(channelname, pass)));

	(void)client;
}

bool IRC::Join::channelPass(std::string channelname, std::string password, Server* server)
{
	std::map<int, Channel *>::iterator it;

	for(it = server->channel_map.begin(); it != server->channel_map.end(); it++)
	{
		if (it->second->getChannelName() == channelname)
		{
			if (it->second->getChannelPassword() == password)
				return(true);
			return(false);
		}
	}
	return(false);
}

bool IRC::Join::channelExist(std::string channelname, Server* server)
{
	std::map<int, Channel *>::iterator it;

	for(it = server->channel_map.begin(); it != server->channel_map.end(); it++)
	{
		if (it->second->getChannelName() == channelname)
			return(true);
	}
	return(false);
}





/*
RPL_TOPIC (332) 
  "<client> <channel> :<topic>"
Sent to a client when joining the <channel> to inform them of the current topic of the channel.

RPL_NAMREPLY (353) 
  "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
Sent as a reply to the NAMES command, this numeric lists the clients that are joined to <channel> and their status in that channel.
*/