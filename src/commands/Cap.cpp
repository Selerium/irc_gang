#include "../../include/IRC.hpp"

IRC::Cap::Cap(){}

IRC::Cap::~Cap(){}

void IRC::Cap::excuteCap(Parse *parse, Client* client, Server* server, int client_fd)
{
	(void)client;
	(void)server;
	(void)client_fd;
    (void)parse;

	//test case
	if (parse->getParameters()[0] == "LS")
    	Parse().sendToClient("CAP * ACK :302 CAP LS\r\n", client_fd, "");
	else if (parse->getParameters()[0] == "END")
    	Parse().sendToClient("CAP * ACK :CAP END\r\n", client_fd, "");



}