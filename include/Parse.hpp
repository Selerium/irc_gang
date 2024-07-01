#ifndef MSG_PARSING_HPP 
#define MSG_PARSING_HPP	

#include "IRC.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Client;

class Parse
{
	public:
	std::vector<std::pair<std::string, std::vector<std::string> > > _messages;

	Parse();
	~Parse();

	void getCommandInfo(const std::string& clientMsg, Client *client);
	void Debug_msg(std::string msg);
	void printcommandinfo();
	void sendToClient(Client *client);

	std::vector<std::string> getParameters();
	std::string getCommand();

};

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#endif