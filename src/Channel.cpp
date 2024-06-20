#include "../include/Channel.hpp"

Channel::Channel(std::string name, std::string pass) : _Name(name), _Password(pass) , _Topic("")
{
	this->inviteOnly = false;
}

std::string Channel::getChannelName() {return this->_Name;}

std::string Channel::getChannelPassword() {return this->_Password;}

bool Channel::getChannelMode() {return this->inviteOnly;}

std::string Channel::getTopic(){return this->_Topic;}

std::string Channel::getwhosetTopic(){return this->_whosetTopic;}



void Channel::setChannelMode(bool mode)
{
	this->inviteOnly = mode;
}

void Channel::setTopic(std::string str)
{
	this->_Topic = str;
	
}

void Channel::setwhosetTopic(std::string str)
{
	this->_whosetTopic = str;
}

void Channel::addChanneluser(Client* client)
{
	if (this->_clients.size() == 0)
		this->_clients.insert(std::make_pair(client, 1));
	else
	{
		std::map<Client *, int>::iterator it;
		int	c = 0;
		for(it = this->_clients.begin(); it != this->_clients.end(); it++)
			c++;
		this->_clients.insert(std::make_pair(client, 0));
	}
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
		it->first->SendServerToClient(msg);
}

void Channel::removeNick(std::string nick)
{
	std::map<Client *, int>::iterator it;
	for(it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->first->getNickname() == nick)
			this->_clients.erase(it->first);
	}
}


