#include "../../include/IRC.hpp"

IRC::Quit::Quit(){}

IRC::Quit::~Quit(){}

void IRC::Quit::excuteQuit(Parse *parse, Client* client, Server* server, int client_fd)
{
	(void)client;
	(void)server;
	(void)client_fd;
    (void)parse;

}