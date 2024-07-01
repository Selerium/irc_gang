#include "../../include/IRC.hpp"

IRC::Quit::Quit(){}

IRC::Quit::~Quit(){}

void IRC::Quit::excuteQuit(Parse *parse, Client* client, Server* server)
{
	(void)parse;

	std::cout << "Clients BEFORE QUIT:" << std::endl;
	server->printClients();
	
	//server acknowledges and replies with ERROR :Disconnected from server
	client->SendServerToClient("ERROR :");

	// //disconnect connection
	// close(server->pfds[client->getClientFd()].fd);

	// //remove client from pfds array
	struct pollfd *tmp = new struct pollfd[server->getNumFds() - 1];
	// //adds pfds to new array before client_fd
	// for (int i = 0; i < client->getClientFd(); i++) {
	// 	tmp[i] = server->pfds[i];
	// }
	// //adds pfds to new array after client_fd (ignores clientfd)
	// for (int i = client->getClientFd(); i + 1 < server->getNumFds(); i++)
	// 	tmp[i] = server->pfds[i + 1];
	// delete[] server->pfds;
	// server->pfds = tmp;
	// server->setNumFds(server->getNumFds() - 1);
	bool found = false;
	for (int i = 0; i < server->getNumFds(); i++) {
		if (server->pfds[i].fd == client->getClientFd()) {
			close(server->pfds[i].fd);
			found = true;
		}
		else {
			if (found)
				tmp[i] = server->pfds[i + 1];
			else
				tmp[i] = server->pfds[i];
		}
	}

	//remove client from clients_map
	server->clients_map.erase(client->getClientFd());


	// //server sends QUIT :<reason> to clients that share a channel with the user
	// //<reason> can be empty

	// //if client closes connection without QUIT, server must still send QUIT :<reason> to others
	// //<reason> = {"Ping timeout", "Disconnected", "Excess Flood"} etc.

	std::cout << "Clients AFTER QUIT:" << std::endl;
	server->printClients();
}