#include "../../include/IRC.hpp"

IRC::Ping::Ping(){}

IRC::Ping::~Ping(){}

void IRC::Ping::excutePing(Parse *parse, Client* client, Server* server)
{
	(void)server;
	
	std::string uniqueToken = "";

	if	(client->getAuthantication() == false) {
		client->SendServerToClient(" : " ERROR_451 " " + client->getNickname() + " :You have not registered\r\n");
		return ;
	}

	if (!parse->getParameters().empty()) 
		std::string uniqueToken = parse->getParameters()[0];
	client->SendServerToClient("PONG " + uniqueToken + "\r\n");

	// irssi doesnt handel these cases // but keep her for evaluation 
	// if (!parse->getParameters().empty()) 
	// {
	// 	std::string uniqueToken = parse->getParameters()[0];
	// 	if (uniqueToken[0] == ':')
	// 		client->SendServerToClient("PONG " + uniqueToken + "\r\n");
	// 	else 
	// 		client->SendServerToClient(": "  ERR_NOORIGIN " " +  client->getNickname() + " :No origin specified\r\n");
	// }
	// else 
	// 	client->SendServerToClient(": "  ERR_NEEDMOREPARAMS " " +  client->getNickname() + " PING :Not enough parameters\r\n");
}		

/*

ERR_NEEDMOREPARAMS (461) "<client> <command> :Not enough parameters"

ERR_NOORIGIN (409)  "<client> :No origin specified"


*/