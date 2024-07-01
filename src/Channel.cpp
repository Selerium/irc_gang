#include "../include/Channel.hpp"

Channel::Channel(std::string name, std::string pass) : _Name(name), _Password(pass) , _Topic("")
{
	this->_clientAmount = 0;
	this->_inviteOnly = false;
}

std::string Channel::getChannelName() {return this->_Name;}

std::string Channel::getChannelPassword() {return this->_Password;}

bool Channel::getChannelMode() {return this->_inviteOnly;}

std::string Channel::getTopic(){return this->_Topic;}

std::string Channel::getwhosetTopic(){return this->_whosetTopic;}

int Channel::getLimit(){return this->_limit;}

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

void Channel::setTopic(std::string str)
{
	this->_Topic = str;
}

void Channel::setwhosetTopic(std::string str)
{
	this->_whosetTopic = str;
}

void Channel::setPass(Client* client, std::string str)
{
	if (checkPermission(client) == 1)
		this->_Password = str;
	else
		client->SendServerToClient(": No permissions to change pass");
}

void Channel::removePass(Client* client)
{
	if (checkPermission(client) == 1)
		this->_Password = "";
}

void Channel::addChanneluser(Client* client)
{
	if (this->_clientAmount == 0)
	{
		this->_clients.insert(std::make_pair(client, 1));
		this->_clientAmount++;
	}
	else if (getChannelMode() == true)
	{
		if (_clientAmount >= getLimit())
			//print that its reached its limit
			return;
		std::map<Client *, int>::iterator it;
		for(it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			if (it->first->getNickname() == client->getNickname() && 
				it->second == 2)
				it->second = 0;
		}
		this->_clientAmount++;
	}
	else
	{
		std::map<Client *, int>::iterator it;
		for(it = this->_clients.begin(); it != this->_clients.end(); it++)
		{
			if (it->first->getNickname() == client->getNickname())
				//already joined
				break;
		}
		this->_clients.insert(std::make_pair(client, 0));
		this->_clientAmount++;
	}
}

void Channel::inviteChanneluser(Client* admin, Client* client)
{
	if (checkPermission(admin) != 1)
	{
		client->SendServerToClient(": No permissions to invite to channel");
		return;
	}
	else if (FindClient(client->getNickname()) != NULL)
	{
		client->SendServerToClient(": User is already in channel");
		return;
	}
	this->_clients.insert(std::make_pair(client, 2));
	this->_clientAmount++;
}

Client* Channel::FindClient(std::string clientNick)
{
	std::map<Client *, int>::iterator it;
	for(it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->first->getNickname() == clientNick)
			return (it->first);
	}
	return NULL;
}


void Channel::sendToall(std::string msg)
{
	std::map<Client *, int>::iterator it;
	for(it = this->_clients.begin(); it != this->_clients.end(); it++)
		if (it->second == 0 || it->second == 1)
			it->first->SendServerToClient(msg);
}

void Channel::removeNick(Client* client, std::string nick)
{
	if (checkPermission(client) != 1)
		client->SendServerToClient("NO PERMISSIONS");
	std::map<Client *, int>::iterator it;
	for(it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->first->getNickname() == nick)
			this->_clients.erase(it->first);
	}
}

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
	for(it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->first->getNickname() == client->getNickname())
		{
			if (perm == true)
				it->second = 1;
			else
				it->second = 0;
		}
	}
}

void Channel::setLimiter(Client* client, int amount)
{
	if (checkPermission(client) == 1)
	{
		if (this->_clientAmount > amount)
			this->_limit = this->_clientAmount;
		else
			this->_limit = amount;
		return;
	}
	client->SendServerToClient(": No permissions");
}

int Channel::checkPermission(Client* client)
{
	std::map<Client *, int>::iterator it;

	it = this->_clients.find(client);
	return (it->second);
}
