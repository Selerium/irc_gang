#include "../../include/IRC.hpp"

IRC::Ping::Ping(){}

IRC::Ping::~Ping(){}

void IRC::Ping::excutePing(Parse *parse, Client* client, Server* server, int client_fd)
{
	(void)client;
	(void)server;
	(void)client_fd;
    (void)parse;

}