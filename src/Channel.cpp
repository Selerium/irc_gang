#include "../include/Channel.hpp"

Channel::Channel(std::string name) : name(name), password("")
{}

std::string Channel::getChannelName() {return this->name;}

std::string Channel::getChannelPassword() {return this->password;}

bool Channel::getChannelMode() {return this->inviteOnly;}

void Channel::setChannelMode(bool mode)
{
	this->inviteOnly = mode;
}