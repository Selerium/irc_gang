#include "../../include/IRC.hpp"
#include "../../include/Channel.hpp"

IRC::Join::Join(){}

IRC::Join::~Join(){}

void IRC::Join::excuteJoin(Parse *parse, Client* client, Server* server)
{
	std::vector<std::string> parameter = parse->getParameters();

	if	(client->getAuthantication() == false)
		client->SendServerToClient("failed\r\n");

	//server exsist and pass is correct
	// if (server->channel_map.size() != 0)
	// {
		if (parameter.size() == 0)
			client->SendServerToClient("Parse error\r\n");
		//server doesnt exsist
		if (channelExist(parameter[0], server) == false)
		{
			if (parameter.size() > 1)
				createChannel(parameter[0], parameter[1], server, client);
			else
				createChannel(parameter[0], std::string(""), server, client);
		}
		//server correct but pass wrong
		else if (channelExist(parameter[0], server) == true && channelPass(parameter[0], parameter[1], server) == false)
			client->SendServerToClient(ERROR_475);
		//server and pass correct
		else if (parameter.size() > 1 && (channelExist(parameter[0], server) == true && channelPass(parameter[0], parameter[1], server) == true))
			joinChannel(parameter[0], parameter[1], server, client);
		else
			joinChannel(parameter[0], std::string(""), server, client); 
	// }
}

void IRC::Join::joinChannel(std::string channelname, std::string pass, Server* server, Client* client)
{
	std::map<int, Channel *>::iterator it;
	(void)pass;

	for(it = server->channel_map.begin(); it != server->channel_map.end(); it++)
	{
		if (it->second->getChannelName() == channelname)
		{
			it->second->addChanneluser(client);
		}
	}
}

void IRC::Join::createChannel(std::string channelname, std::string pass, Server* server, Client* client)
{
	if (server->channel_map.size() == 0)
		server->channel_map.insert(std::make_pair(0, new Channel(channelname, pass)));
	else
	{
		std::map<int, Channel *>::iterator it;
		int	c = 0;
		for(it = server->channel_map.begin(); it != server->channel_map.end(); it++)
			c++;
		server->channel_map.insert(std::make_pair(c, new Channel(channelname, pass)));
	}
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
