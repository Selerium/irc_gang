#include "../../include/IRC.hpp"

IRC::Nick::Nick(){}

IRC::Nick::~Nick(){}

bool	dup_names(std::string nickname, Server* server)
{
	std::map<int , Client *>::iterator it;
	for(it = server->clients_map.begin(); it != server->clients_map.end(); it++)
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

void	IRC::Nick::excuteNick(Parse *parse, Client* client, Server* server)
{
	std::vector<std::string> parameter = parse->getParameters();

	if (parameter.empty()) 
		client->SendServerToClient(ERROR_431 "\r\n");
	else if (dup_names(parameter[0], server) == true)
		client->SendServerToClient(parameter[0] + ERROR_433 + "\r\n");
	else if (valid_nick(parameter[0]) == false)
		client->SendServerToClient(parameter[0] + ERROR_432 + "\r\n");
	else
	{
		std::string oldnick = client->getNickname();
		client->setNickname(parameter[0]);
		oldnick = oldnick + " NICK " + parameter[0] + "\r\n";
		client->SendServerToClient(oldnick);
	}
}
	