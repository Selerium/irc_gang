#include "../../include/IRC.hpp"

IRC::Mode::Mode(){}

IRC::Mode::~Mode(){}

void IRC::Mode::excuteMode(Parse *parse, Client* client, Server* server, int client_fd)
{
	(void)client;
	(void)server;
	(void)client_fd;
    (void)parse;

}