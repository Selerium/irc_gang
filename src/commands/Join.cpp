#include "../../include/IRC.hpp"
#include "../../include/Channel.hpp"

IRC::Join::Join(){}

IRC::Join::~Join(){}

void IRC::Join::excuteJoin(Parse *parse, Client* client, Server* server, int client_fd)
{
	std::vector<std::string> parameter = parse->getParameters();

	//server exsist and pass is correct
	if (server->channel_map.size() != 0)
	{
		//server doesnt exsist
		if (channelExist(parameter[0], server) == false)
		{
			if (parameter.size() > 1)
				createChannel(parameter[0], server, client);
			else
				createChannel(parameter[0], server, client);
			Parse().sendToClient("Server exist\r\n", client_fd, "");
		}
		//server correct but pass wrong
		else if (channelExist(parameter[0], server) == true && channelPass(parameter[0], parameter[1], server) == false)
			Parse().sendToClient(ERROR_475, client_fd, "");
		//server and pass correct
		else
			Parse().sendToClient("Server exist\r\n", client_fd, "");
	}

	(void)client;
	(void)server;
	(void)client_fd;
	(void)parse;
}

void IRC::Join::createChannel(std::string channelname, Server* server, Client* client)
{
	if (server->channel_map.size() == 0)
		server->channel_map.insert(std::make_pair(0, new Channel(channelname)));
	else
	{
		std::map<int, Channel *>::iterator it;
		int	c = 0;
		for(it = server->channel_map.begin(); it == server->channel_map.end(); it++)
			c++;
		server->channel_map.insert(std::make_pair(c, new Channel(channelname)));
	}
	(void)client;
}

bool IRC::Join::channelPass(std::string channelname, std::string password, Server* server)
{
	std::map<int, Channel *>::iterator it;

	for(it = server->channel_map.begin(); it == server->channel_map.end(); it++)
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

	for(it = server->channel_map.begin(); it == server->channel_map.end(); it++)
	{
		if (it->second->getChannelName() == channelname)
			return(true);
	}
	return(false);
}
