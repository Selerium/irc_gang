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

	//server acknowledges and replies with ERROR :Disconnected from server
	client->SendServerToClient("ERROR :");

	//remove client from pfds array and closed pfd fd
	struct pollfd *tmp = new struct pollfd[server->getFdSize()];
	bool found = false;
	for (int i = 0; i < server->getNumFds() - 1; i++) {
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
	if (!found)
		close(server->pfds[server->getNumFds() - 1].fd);
	delete[] server->pfds;
	server->pfds = tmp;
	server->setNumFds(server->getNumFds() - 1);

	//server sends QUIT :<reason> to clients that share a channel with the user
	//<reason> can be empty
	if (!server->channel_map.empty()) {
		for (std::map<int, Channel *>::iterator it = server->channel_map.begin(); it != server->channel_map.end(); it++) {
			if (it->second->FindClient(client->getNickname())) {
				it->second->sendToall(":" + client->getNickname() + "!" + client->getUsername() + " QUIT :" + quit_msg);
				it->second->_clientAmount--;
				it->second->_clients.erase(client);
				if (it->second->_clientAmount == 0) {
					delete it->second;
					server->channel_map.erase(it->first);
					return ;
				}
			}
		}
	}

	//remove client from clients_map
	Client *todel = server->clients_map[client->getClientFd()];
	server->clients_map.erase(client->getClientFd());
	delete todel;
}