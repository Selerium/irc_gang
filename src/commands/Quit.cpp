#include "../../include/IRC.hpp"

IRC::Quit::Quit(){}

IRC::Quit::~Quit(){}

void IRC::Quit::excuteQuit(Parse *parse, Client* client, Server* server)
{
	(void)client;
	(void)server;
    (void)parse;

	std::cout << "Clients BEFORE QUIT:" << std::endl;
	server->printClients();

	// in case invalid client_fd (should never happen)
	if (client->getClientFd() >= server->getFdSize() || client->getClientFd() <= 0) {
		std::cout << "coding error: this fd doesn't exist in array" << std::endl;
		return ;
	}

	//disconnect connection
	close(server->pfds[client->getClientFd()].fd);

	//remove client from pfds array
	struct pollfd *tmp = new struct pollfd[server->getFdSize() - 1];
	//adds pfds to new array before client_fd
	for (int i = 0; i < client->getClientFd(); i++) {
		tmp[i] = server->pfds[i];
	}
	//adds pfds to new array after client_fd (ignores clientfd)
	for (int i = client->getClientFd(); i + 1 < server->getFdSize(); i++)
		tmp[i] = server->pfds[i + 1];
	delete[] server->pfds;
	server->pfds = tmp;
	server->setFdSize(server->getFdSize() - 1);

	//remove client from clients_map
	server->clients_map.erase(client->getClientFd());

	//server acknowledges and replies with ERROR :Disconnected from server

	//server sends QUIT :<reason> to clients that share a channel with the user
	//<reason> can be empty

	//if client closes connection without QUIT, server must still send QUIT :<reason> to others
	//<reason> = {"Ping timeout", "Disconnected", "Excess Flood"} etc.

	std::cout << "Clients AFTER QUIT:" << std::endl;
	server->printClients();
}