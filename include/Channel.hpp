#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "IRC.hpp"
#include "Client.hpp"

class Client; 

class Channel
{
	private:
		std::string _Name;
		std::string _Password;
		bool inviteOnly;
		std::string _Topic;
		std::string _whosetTopic;
		// int *_Admins;



	public:
		std::map<Client *, int> _clients;
		int _clientAmount;
		
		Channel(std::string name, std::string pass);

		std::string getChannelName();
		std::string getChannelPassword();
		std::string getTopic();
		std::string getwhosetTopic();

		bool getChannelMode();

		void setChannelMode(bool mode);
		void setTopic(std::string str);
		void setwhosetTopic(std::string str);


		void addChanneluser(Client* client);
		Client* FindClient(std::string clientNick);
		void sendToall(std::string msg);
		void removeNick(std::string nick);


};

#endif