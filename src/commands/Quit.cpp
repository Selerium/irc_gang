#include "../../include/IRC.hpp"

IRC::Quit::Quit(){}

IRC::Quit::~Quit(){}

void IRC::Quit::excuteQuit(Parse *parse, Client* client, Server* server, int client_fd)
{
	std::vector<std::string> parameter = parse->getParameters();

	// if (parameter.empty())
		//just quit
	// else
		Parse().sendToClient("EMPTY NICKNAME\r\n", client_fd, "");
		// Quit: Bye for now!
		// ; dan- is exiting the network with
		// the message: "Quit: Bye for now!"
	(void)client;
	(void)server;
	(void)client_fd;
	(void)parse;

}