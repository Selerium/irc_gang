#include "../../include/IRC.hpp"
#include <map>

IRC::Quit::Quit(){}

IRC::Quit::~Quit(){}

void IRC::Quit::excuteQuit(Parse *parse, Client* client, Server* server)
{
	std::string quit_msg;
	std::vector<std::string> sample = parse->getParameters();

	for (std::vector<std::string>::iterator it = sample.begin(); it != sample.end(); it++) {
		quit_msg.append(it->c_str());
		quit_msg.append(" ");
	}
	parse->Debug_msg(quit_msg);

	std::cout << "Clients BEFORE QUIT:" << std::endl;
	server->printClients();
	
	//server acknowledges and replies with ERROR :Disconnected from server
	client->SendServerToClient("ERROR :");

	//remove client from pfds array and closed pfd fd
	struct pollfd *tmp = new struct pollfd[server->getNumFds() - 1];
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

	//server sends QUIT :<reason> to clients that share a channel with the user
	//<reason> can be empty
	if (!server->channel_map.empty()) {
		for (std::map<int, Channel *>::iterator it = server->channel_map.begin(); it != server->channel_map.end(); it++) {
			if (it->second->FindClient(client->getNickname()))
				it->second->sendToall(client->getNickname() + "!" + client->getUsername() + "@localhost QUIT :" + quit_msg);
		}
	}

	//remove client from clients_map
	server->clients_map.erase(client->getClientFd());

	std::cout << "Clients AFTER QUIT:" << std::endl;
	server->printClients();
}