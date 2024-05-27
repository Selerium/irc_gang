#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "IRC.hpp"

class Channel
{
	private:
		std::string name;
		std::string password;
		bool inviteOnly;
		// int *clients;

	public:
		Channel(std::string name);

		std::string getChannelName();
		std::string getChannelPassword();
		bool getChannelMode();
		void setChannelMode(bool mode);
};

#endif