#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "IRC.hpp"
#include "Channel.hpp"

class Channel;

class Client
{
	private:
	//client info
	std::string _Username;
	std::string _Nickname;
	std::string _hostName;
	std::string _RealName;
	int _ClientFd;
	bool _welcomMsgsent;
	//bool _registerd;
	bool	_isAuthanticated;

	std::map<int, Channel *> Client_channels; // a map for the channes the client is registerd for 

	public:
		std::string _ClientToServer;
		std::string _ServerToClient;
		bool UserSet;

		Client(int fd, std::string hostname);
		int getClientFd();
		bool getAuthantication();
		std::string getUsername();
		std::string getNickname();
		std::string getHostname();
		std::string getRealname();

		bool getWelcomeMsg();
		void setAuthantication(bool flag);
		void setNickname(std::string name);
		void setUsername(std::string name);
		void setRealname(std::string name);
		void SendServerToClient(std::string msg);

		void setWelcomeMsg(bool flag);

		bool isregisterd();
};

#endif