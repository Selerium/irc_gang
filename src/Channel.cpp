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

bool Channel::getsetLimit(){return this->_setLimit;}

void Channel::setTopic(Client* client, std::string str)
{
	if (getTopicMode() == true) {
		if (checkPermission(client)) {
			this->_Topic = str;
			setwhosetTopic(client->getNickname());
		}
	}
	else
		this->_Topic = str;
}

void Channel::setwhosetTopic(std::string str)
{
	this->_whosetTopic = str;
}

bool Channel::getTopicMode() {return this->_topicMode;}

void Channel::addChanneluser(Client* client)
{
	if (this->_clientAmount == 0)
	{
		this->_clients.insert(std::make_pair(client, 1));
		this->_clientAmount++;
	}
	else if (getsetLimit() == true)
	{

	}
	else if (getChannelMode() == true)
	{
		if (_clientAmount >= getLimit())
		{
			client->SendServerToClient("Already reached limit of users in channel");
			return;
		}
		std::map<Client *, int>::iterator it;
		it = this->_clients.find(client);
		if (it->first->getNickname() == client->getNickname() && it->second == 2)
			it->second = 0;
		this->_clientAmount++;
	}
	else
	{
		std::map<Client *, int>::iterator it;
		it = this->_clients.find(client);
		if (it->first->getNickname() == client->getNickname())
		{
			client->SendServerToClient("Already in server");
			return;
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
	it = this->_clients.find(client);
	if (it->first->getNickname() == nick)
		this->_clients.erase(it->first);
}


int Channel::checkPermission(Client* client)
{
	std::map<Client *, int>::iterator it;

	it = this->_clients.find(client);
	return (it->second);
}
