#include "../../include/IRC.hpp"

IRC::Mode::Mode(){}

IRC::Mode::~Mode(){}

void IRC::Mode::excuteMode(Parse *parse, Client* client, Server* server)
{
	std::vector<std::string> parameter = parse->getParameters();

	if	(client->getAuthantication() == false)
		client->SendServerToClient("failed\r\n");
	if (parameter.size() == 0)
		client->SendServerToClient("Parse error\r\n");
	
	std::string channelname = parameter[0];
	std::map<int, Channel *>::iterator it;
	for(it = server->channel_map.begin(); it != server->channel_map.end(); it++)
	{
		if (it->second->getChannelName() == channelname)
		{
			if (parameter.size() == 1)
			{
				client->SendServerToClient(it->second->getChannelName() + "\r\n");
				
				if (it->second->getsetLimit() == true)
					client->SendServerToClient("User limit : TRUE : " + it->second->getLimit() + "\r\n");
				else
					client->SendServerToClient("User limit : FALSE\r\n");
				
				if (it->second->getChannelMode() == true)
					client->SendServerToClient("Channel pass : TRUE : " + it->second->getChannelPassword() + "\r\n");
				else
					client->SendServerToClient("Channel pass : FALSE\r\n");

				client->SendServerToClient("Channel privilage : " + it->second->getChannelMode() + "\r\n");

				client->SendServerToClient("Channel Topic : " + it->second->getTopicMode() + " : " + it->second->getTopic() + "\r\n");
			}
			if (parameter[1] == "+L" || parameter[1] == "+l")
			{
				if (parameter.size() == 3)
				{
					int num = std::atoi(parameter[2].c_str());
					it->second->setLimiter(client, num);
				}
			}
			if (parameter[1] == "-L" || parameter[1] == "-l")
				it->second->removeLimit(client);
			if (parameter[1] == "+K" || parameter[1] == "+k")
			{
				it->second->setPass(client, parameter[2]);
			}
			if (parameter[1] == "-K" || parameter[1] == "-k")
				it->second->removePass(client);
			if (parameter[1] == "+I" || parameter[1] == "+i")
			{
				if (parameter.size() == 2)
					it->second->setChannelMode(client, true);
			}
			if (parameter[1] == "-I" || parameter[1] == "-i")
			{
				if (parameter.size() == 2)
					it->second->setChannelMode(client, false);
			}
			if (parameter[1] == "+O" || parameter[1] == "+o")
			{
				if (parameter.size() == 3 && it->second->FindClient(parameter[2]))
					it->second->Permissions(client, it->second->FindClient(parameter[2]), true);
			}
			if (parameter[1] == "-O" || parameter[1] == "-o")
			{
				if (parameter.size() == 3 && it->second->FindClient(parameter[2]))
					it->second->Permissions(client, it->second->FindClient(parameter[2]), false);
			}
			if (parameter[1] == "+T" || parameter[1] == "+t")
			{
				if (parameter.size() == 3 && it->second->FindClient(parameter[2]))
					it->second->Permissions(client, it->second->FindClient(parameter[2]), true);
			}
			if (parameter[1] == "-T" || parameter[1] == "-t")
			{
				if (parameter.size() == 3 && it->second->FindClient(parameter[2]))
					it->second->Permissions(client, it->second->FindClient(parameter[2]), false);
			}
		}
	}
	(void)client;
	(void)server;
    (void)parse;

}

// · l: Set/remove the user limit to channel
void Channel::removeLimit(Client* client)
{
	if (checkPermission(client) == 1)
	{
		this->_setLimit = false;
		return;
	}
	client->SendServerToClient(": No permissions");
}

void Channel::setLimiter(Client* client, int amount)
{
	if (checkPermission(client) == 1)
	{
		if (this->_clientAmount > amount)
		{
			client->SendServerToClient(": Set amount is lower than already joined");
			this->_limit = this->_clientAmount;
		}
		else
			this->_limit = amount;
		this->_setLimit = true;
		return;
	}
	client->SendServerToClient(": No permissions");
}

// · k: Set/remove the channel key (password)
void Channel::removePass(Client* client)
{
	if (checkPermission(client) == 1)
		this->_Password = "";
}

void Channel::setPass(Client* client, std::string str)
{
	if (checkPermission(client) == 1)
		this->_Password = str;
	else
		client->SendServerToClient(": No permissions to change pass");
}

//  i: Set/remove Invite-only channel
void Channel::setChannelMode(Client* client, bool mode)
{
	if (checkPermission(client) == 1)
	{
		this->_inviteOnly = mode;
		if (this->_inviteOnly == true)
			this->_limit = _clientAmount;
	}
	else
		client->SendServerToClient(": No permissions to change mode");
}


// · o: Give/take channel operator privilege
void Channel::Permissions(Client* admin, Client* client, bool perm)
{
	if (checkPermission(admin) != 1)
	{
		admin->SendServerToClient(": No permissions");
		return;
	}
	else if (checkPermission(client) == 2)
	{
		client->SendServerToClient(": User did not join yet");
		return;
	}
	std::map<Client *, int>::iterator it;
	it = this->_clients.find(client);
	if (it->first->getNickname() == client->getNickname())
	{
		if (perm == true)
			it->second = 1;
		else
			it->second = 0;
	}

}

// · t: Set/remove the restrictions of the TOPIC command to channel
void Channel::SetTopicMode(Client* client, bool mode)
{
	if (checkPermission(client) == 1)
		this->_topicMode = mode;
	else
		client->SendServerToClient(": No permissions to change mode");
}
