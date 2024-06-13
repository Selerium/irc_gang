#include "../../include/IRC.hpp"

IRC::Quit::Quit(){}

IRC::Quit::~Quit(){}

void IRC::Quit::excuteQuit(Parse *parse, Client* client, Server* server)
{
	(void)client;
	(void)server;
    (void)parse;

}