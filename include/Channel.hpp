#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "IRC.hpp"

class Channel
{
	private:
		std::string _Name;
		std::string _Password;
		bool inviteOnly;
		int *_Admins;

		// · l: Set/remove the user limit to channel

	public:
		std::map<Client *, int> _clients;
		int _clientAmount;
		
		Channel(std::string name, std::string pass);

		std::string getChannelName();
		std::string getChannelPassword();
		bool getChannelMode();
		void setChannelMode(bool mode);
		void addChanneluser(Client* client);
};

#endif