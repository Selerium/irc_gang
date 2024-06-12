#include "../include/Channel.hpp"

Channel::Channel(std::string name, std::string pass) : _Name(name), _Password(pass)
{
	this->inviteOnly = false;
}

std::string Channel::getChannelName() {return this->_Name;}

std::string Channel::getChannelPassword() {return this->_Password;}

bool Channel::getChannelMode() {return this->inviteOnly;}

void Channel::setChannelMode(bool mode)
{
	this->inviteOnly = mode;
}

void Channel::addChanneluser(Client* client)
{
	if (this->_clients.size() == 0)
		this->_clients.insert(std::make_pair(client, 1));
	else
	{
		std::map<Client *, int>::iterator it;
		int	c = 0;
		for(it = this->_clients.begin(); it == this->_clients.end(); it++)
			c++;
		this->_clients.insert(std::make_pair(client, 0));
	}
	this->_clientAmount++;
}