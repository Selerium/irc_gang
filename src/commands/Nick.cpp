#include "../../include/IRC.hpp"

IRC::Nick::Nick(){}

IRC::Nick::~Nick(){}

bool	dup_names(std::string nickname, Server* server)
{
	std::map<int , Client *>::iterator it;
	for(it = server->clients_map.begin(); it == server->clients_map.end(); it++)
	{
		if (it->second->getNickname() == nickname)
			return(true);
	}
	return(false);
}

bool	valid_nick(std::string nickname)
{
	// allow alphanum []{}\|
	std::string allowed_char = "[]{}\\|";
	const char	*str = nickname.c_str();

	// no digits for the first character
	if (isdigit(str[0]))
		return(false);
	for (size_t c = 0; c != nickname.size(); c++)
	{
		if (isalnum(str[c]) || allowed_char.find_first_of(str[c]))
			continue;
		else
			return(false);
	}
	return(true);
}

void	IRC::Nick::excuteNick(Parse *parse, Client* client, Server* server, int client_fd)
{
	// fix when i join as (nc localhost)
	std::vector<std::string> parameter = parse->getParameters();

	if (parameter.empty()) 
		Parse().sendToClient(parameter[0] + ERROR_431, client_fd, "");
	else if (dup_names(parameter[0], server) == true)
		Parse().sendToClient(parameter[0] + ERROR_433, client_fd, "");
	else if (valid_nick(parameter[0]) == false)
		Parse().sendToClient(parameter[0] + ERROR_432, client_fd, "");
	else
	{
		std::string oldnick = client->getNickname();
		client->setNickname(parameter[0]);
		oldnick = oldnick + " NICK " + parameter[0];
		Parse().sendToClient("Nickname successfully changed", client_fd, "");
	}
}
