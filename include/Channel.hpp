#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "IRC.hpp"
#include "Client.hpp"

class Client; 

class Channel
{
	private:
		std::string	_Name;
		std::string	_Password;
		bool		_inviteOnly;
		std::string	_Topic;
		std::string	_whosetTopic;
		int			_limit;
		// int *_Admins;



	public:
	//0 = Normal joined
	//1 = Admin
	//2 = invited
		std::map<Client *, int> _clients;
		int _clientAmount;
		
		Channel(std::string name, std::string pass);

		std::string getChannelName();
		std::string getChannelPassword();
		std::string getTopic();
		std::string getwhosetTopic();
		int			getLimit();

		bool getChannelMode();

		void setChannelMode(Client* client, bool mode);
		void setTopic(std::string str);
		void setwhosetTopic(std::string str);

		void setPass(Client* client, std::string str);
		void removePass(Client* client);

		void addChanneluser(Client* client);
		
		void inviteChanneluser(Client* admin, Client* client);
		
		Client* FindClient(std::string clientNick);
		void sendToall(std::string msg);
		void removeNick(Client* client, std::string nick);

		void Permissions(Client* admin, Client* client, bool perm);

		void setLimiter(Client* client, int amount);

		int checkPermission(Client* client);


};

#endif