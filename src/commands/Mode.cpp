#include "../../include/IRC.hpp"
#include <asm-generic/errno.h>

IRC::Mode::Mode(){}

IRC::Mode::~Mode(){}

std::string print_num(int num) {
	std::stringstream result;
	result << num;
	std::string str = result.str();
	return (str);
}

void IRC::Mode::excuteMode(Parse *parse, Client* client, Server* server)
{
	if	(client->isregisterd() == false) {
		client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " + ERROR_451 + " MODE :You have not registered");
		return ;
	}

	if (parse->getParameters().size() == 0) {
		client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " + ERROR_461 + " MODE :Not enough parameters");
		return ;
	}

	std::vector<std::string> parameter = parse->getParameters();
	std::string channelname;
	if (parameter.size() > 1) {
		channelname = '#' + parameter[0];
	}
	else {
		channelname = parameter[0];
	}

	if (server->channel_map.empty())
	{
		if (checkUser(server, channelname, client, parse) == 0)
			client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " 403 MODE :channel doesnt exist");
		return;
	}

	parse->Debug_msg("hellppppp");

	std::map<int, Channel *>::iterator it;
	for(it = server->channel_map.begin(); it != server->channel_map.end(); it++)
	{
		if (it->second->getChannelName() == channelname)
		{
			if (!it->second->FindClient(client->getNickname())) {
				client->SendServerToClient(client->getNickname() + " " + channelname + " :442 You're not on that channel");
				return ;
			}
			if (parameter.size() == 1)
			{
				client->SendServerToClient("Channel name |" + it->second->getChannelName());
				
				if (it->second->getsetLimit() == true)
					client->SendServerToClient("User limit | TRUE | " + print_num(it->second->getLimit()));
				else
					client->SendServerToClient("User limit | FALSE");

				if (it->second->getChannelMode() == true)
					client->SendServerToClient("Requires invite | TRUE");
				else
					client->SendServerToClient("Requires invite | FALSE");

				if (it->second->getChannelPassword().length())
					client->SendServerToClient("Requires pass | TRUE");
				else
					client->SendServerToClient("Requires pass | FALSE");

				if (it->second->checkPermission(client) == 0)
					client->SendServerToClient("Channel privilage | User");
				else if (it->second->checkPermission(client) == 1)
					client->SendServerToClient("Channel privilage | Operator");
				else if (it->second->checkPermission(client) == 2)
					client->SendServerToClient("Channel privilage | Invited");
				else
					client->SendServerToClient("Channel privilage | NON");

				client->SendServerToClient("Channel Topic | " + print_num(it->second->getTopicMode()) + " | " + it->second->getTopic() + "");
				return ;
			}
			
			if (parameter[1] == "+L" || parameter[1] == "+l")
			{
				if (parameter.size() == 3)
				{
					int num = std::atoi(parameter[2].c_str());
					it->second->setLimiter(client, num, parameter[2]);
				}
				else {
					client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " :Wrong number of arguments");
				}
			}
			else if (parameter[1] == "-L" || parameter[1] == "-l") {
				it->second->removeLimit(client);
			}
			else if (parameter[1] == "+K" || parameter[1] == "+k")
			{
				if (parameter.size() == 3) {
					it->second->setPass(client, parameter[2]);
				}
				else {
					client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " :Wrong number of arguments");
				}
			}
			else if (parameter[1] == "-K" || parameter[1] == "-k") {
				it->second->removePass(client);
			}
			else if (parameter[1] == "+I" || parameter[1] == "+i")
			{
				it->second->setChannelMode(client, true);
			}
			else if (parameter[1] == "-I" || parameter[1] == "-i")
			{
				it->second->setChannelMode(client, false);
			}
			else if (parameter[1] == "+O" || parameter[1] == "+o")
			{
				if (!it->second->FindClient(parameter[2])) {
					client->SendServerToClient(client->getNickname() + " " + parameter[2] + " " + it->second->getChannelName() + " :" ERR_USERNOTINCHANNEL + " They aren't on that channel");
					return ;
				}
				if (parameter.size() == 3)
				{
					it->second->Permissions(client, it->second->FindClient(parameter[2]), true, parameter[2]);
				}
				else {
					client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " :Wrong number of arguments");
				}
			}
			else if (parameter[1] == "-O" || parameter[1] == "-o")
			{
				if (parameter.size() == 3) {
					if (!it->second->FindClient(parameter[2])) {
						client->SendServerToClient(client->getNickname() + " " + parameter[2] + " " + it->second->getChannelName() + " :" ERR_USERNOTINCHANNEL + " They aren't on that channel");
						return ;
					}
					it->second->Permissions(client, it->second->FindClient(parameter[2]), false, parameter[2]);
				}
				else
					client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " :Wrong number of arguments");
			}
			else if (parameter[1] == "+T" || parameter[1] == "+t")
			{
				it->second->SetTopicMode(client, true);
			}
			else if (parameter[1] == "-T" || parameter[1] == "-t")
			{
				it->second->SetTopicMode(client, false);
			}
			else {
				client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " MODE " + parameter[1] + " :is unknown mode char to me");
			}
			return ;
		}
	}
	if (it == server->channel_map.end())
	{
		if (checkUser(server, channelname, client, parse) == 0)
			client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " 403 MODE " + channelname + " :channel doesnt exist");
	}
	(void)parse;
}

int IRC::Mode::checkUser(Server* server, std::string name, Client* client, Parse* parse)
{
	(void)name;
	(void)client;
	std::map<int , Client *>::iterator it;
	if (!server->clients_map.empty())
	{
		for (it = server->clients_map.begin(); it != server->clients_map.end(); it++)
		{
			parse->Debug_msg(it->second->getNickname() + " _ " + parse->getParameters()[0]);
			if (it->second->getNickname() == parse->getParameters()[0] && parse->getParameters()[1].empty() == true)
			{
				it->second->SendServerToClient(":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + parse->getParameters()[0] + " :+i");
				return 1;
			}
			else if (it->second->getNickname() == parse->getParameters()[0] && parse->getParameters()[1].empty() == false)
			{
				it->second->SendServerToClient(":" + it->second->getNickname() + "!" + it->second->getUsername() + " MODE " + parse->getParameters()[0] + " :" + parse->getParameters()[1]);
				return 1;
			}
		}
	}
	return 0;
}

// · l: Set/remove the user limit to channel
void Channel::removeLimit(Client* client)
{
	if (checkPermission(client) == 1)
	{
		this->_setLimit = false;
		sendToall(":" + client->getNickname() + "!" + client->getUsername() + " MODE " + getChannelName() + " -l ");
		return;
	}
	client->SendServerToClient(": No permissions");
}

void Channel::setLimiter(Client* client, int amount, std::string str)
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
		sendToall(":" + client->getNickname() + "!" + client->getUsername() + " MODE " + getChannelName() + " +l " + str);
		return;
	}
	client->SendServerToClient(": No permissions");
}

// · k: Set/remove the channel key (password)
void Channel::removePass(Client* client)
{
	if (checkPermission(client) == 1)
	{
		this->_Password = "";
		sendToall(":" + client->getNickname() + "!" + client->getUsername() + " MODE " + getChannelName() + " -k ");
	}
	else
		client->SendServerToClient(": No permissions to change pass");
}

void Channel::setPass(Client* client, std::string str)
{
	if (checkPermission(client) == 1)
	{
		this->_Password = str;
		sendToall(":" + client->getNickname() + "!" + client->getUsername() + " MODE " + getChannelName() + " +k " + str);
	}
	else
		client->SendServerToClient(": No permissions to change pass");
}

//  i: Set/remove Invite-only channel
void Channel::setChannelMode(Client* client, bool mode)
{
	if (checkPermission(client) == 1)
	{
		client->SendServerToClient(": Mode changed!");
		this->_inviteOnly = mode;
		if (mode == true)
			sendToall(":" + client->getNickname() + "!" + client->getUsername() + " MODE " + getChannelName() + " +i ");
		else 
			sendToall(":" + client->getNickname() + "!" + client->getUsername() + " MODE " + getChannelName() + " -i ");
		// if (this->_inviteOnly == true)
		// 	this->_limit = _clientAmount;
	}
	else
		client->SendServerToClient(": No permissions to change mode");
}


// · o: Give/take channel operator privilege
void Channel::Permissions(Client* admin, Client* client, bool perm, std::string str)
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
		{
			it->second = 1;
			sendToall(":" + client->getNickname() + "!" + client->getUsername() + " MODE " + getChannelName() + " +o " + str);
		}
		else if (perm == true && it->second != 2)
		{
			it->second = 0;
			sendToall(":" + client->getNickname() + "!" + client->getUsername() + " MODE " + getChannelName() + " -o " + str);
			this->welcomeMsgChan2(it->first);
		}
		else
		{
			it->second = 0;
			this->welcomeMsgChan2(it->first);
		}
	}

}

// · t: Set/remove the restrictions of the TOPIC command to channel
void Channel::SetTopicMode(Client* client, bool mode)
{
	if (checkPermission(client) == 1)
	{
		this->_topicMode = mode;
		if (mode == true )
			sendToall(":" + client->getNickname() + "!" + client->getUsername() + " MODE " + getChannelName() + " +t ");
		else 
			sendToall(":" + client->getNickname() + "!" + client->getUsername() + " MODE " + getChannelName() + " -t ");
	}
	else
		client->SendServerToClient(": No permissions to change mode");
}
