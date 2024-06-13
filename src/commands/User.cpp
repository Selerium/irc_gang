#include "../../include/IRC.hpp"

IRC::User::User(){}

IRC::User::~User(){}

void IRC::User::excuteUser(Parse *parse, Client* client, Server* server)
{
	(void)client;
	(void)server;
    (void)parse;
	server->clients_map[client->getClientFd()]->setUsername(parse->getParameters()[0]);
	client->SendServerToClient("Username successfully changed\r\n");

	// if (isRegisterd(server, client_fd))
	// 	client.SendServerToClient(ERROR_451, client_fd, "");


}

// bool IRC::User::isRegisterd(Server* server, int client_fd)
// {
// 	std::map<int , Client *>::iterator it = server->clients_map.find(client_fd);

// 	if(it == server->clients_map.end())
// 		return true;
// 	return false;
// }
