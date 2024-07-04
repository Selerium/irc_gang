#include "../../include/IRC.hpp"

IRC::User::User(){}

IRC::User::~User(){}

void IRC::User::excuteUser(Parse *parse, Client* client, Server* server)
{
	(void)server;

	if (!client->getAuthantication())
		client->SendServerToClient(" : " ERROR_451 " " + client->getNickname() + " :You have not registered\r\n");
	else
	{
		if (client->UserSet == false)
		parseMsg(parse, client);
		else
			client->SendServerToClient(": " ERROR_462 " " + client->getNickname() + " :You may not reregister\r\n");
	}
}

int IRC::User::parseMsg(Parse *parse, Client* client)
{
	std::vector<std::string> parameters = parse->getParameters();
	if (parameters.empty() || parameters[0][0] == ':'|| parameters[1][0] == ':' || parameters[2][0] == ':' || parameters.size() < 4)
	{
		client->SendServerToClient(": " ERR_NEEDMOREPARAMS " " + client->getNickname() 
		+ " USER " ":Not enough parameters\r\n");
		return 1;
	}

	if (parameters[0].length() > USERLEN)
		client->setUsername(parameters[0].substr(0, 12));
	else 
		client->setUsername(parameters[0]);

    std::vector<std::string>::iterator it;
	bool textFound;
    for (it = parameters.begin() + 3; it != parameters.end(); ++it)
    {
        if (!it->empty())
        {
            if ((*it)[0] == ':' && !textFound)
            {
                *it = it->substr(1);
                textFound = true;
            }
            else if ((*it)[0] != ':' && !textFound)
			{
				client->SendServerToClient(": " ERR_NEEDMOREPARAMS " " + client->getNickname() 
				+ " USER " ":Not enough parameters\r\n");
                return 1;
			}
            realname += (*it + " ");
        }
    }
	client->setRealname(realname);
	client->UserSet = true;
	return 0;
}
