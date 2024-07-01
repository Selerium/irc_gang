#include "../../include/IRC.hpp"

IRC::Cap::Cap(){}

IRC::Cap::~Cap(){}

void IRC::Cap::excuteCap(Parse *parse, Client* client, Server* server)
{
	(void)server;
	std::vector<std::string> parameters = parse->getParameters();
	if (parameters.empty())
		client->SendServerToClient("{EMPTY FIX LATER}\r\n");
	else if (parse->getParameters()[0] == "LS")
		client->SendServerToClient("CAP * ACK : LS 302\r\n");
	else if (parse->getParameters()[0] == "END")
		client->SendServerToClient("CAP * ACK :CAP END\r\n");
}