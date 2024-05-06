#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "IRC.hpp"
#include "Channel.hpp"

class Client
{
	private:
	//client info
	std::string _Username;
	std::string _Nickname;
	std::string _hostName;
	int _ClientFd;
	//bool _registerd;
	bool	_isAuthanticated;

	std::map<int, Channel *> Client_channels; // a map for the channes the client is registerd for 

	public:
		Client(int fd, int isAuthanticated);
		int getClientFd();
		bool getAuthantication();
		std::string getUsername();
		std::string getNickname();
		std::string getHostname();

};

#endif