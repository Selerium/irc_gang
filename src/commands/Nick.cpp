#include "../../include/IRC.hpp"
#include <cctype>

std::string toUpperCase(const std::string& str);

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
	std::string allowed_char = "[]{}\\|abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	const char	*str = nickname.c_str();

	if (toUpperCase(nickname).substr(0, 3) == "BOT") {
		return (false);
	}

	// no digits for the first character
	if (isdigit(str[0]))
		return(false);
	for (size_t c = 0; c != nickname.size(); c++)
	{
		if (isalnum(str[c]) || allowed_char.find_first_of(str[c]) != std::string::npos)
			continue;
		else
			return(false);
	}
	return(true);
}

void	IRC::Nick::excuteNick(Parse *parse, Client* client, Server* server)
{
	std::vector<std::string> parameter = parse->getParameters();

	if (!client->getAuthantication())
		client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " + ERROR_451 + " NICK :You have not registered");
	else if (parameter.empty()) 
		client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " 431 NICK " ERROR_431);
	else if (dup_names(parameter[0], server) == true)
		client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " 433 NICK " + parameter[0] + " " ERROR_433);
	else if (parameter[0][0] == ':' || valid_nick(parameter[0]) == false)
		client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " 432 NICK " + parameter[0] + " " ERROR_432);
	else
	{
		std::string oldnick = client->getNickname();
		client->setNickname(parameter[0]);
		client->SendServerToClient(":" + oldnick + "!" + client->getUsername() + " NICK " + parameter[0]);
	}
}
