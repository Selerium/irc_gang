#include "../../include/IRC.hpp"

IRC::Privmsg::Privmsg() : _receiver("") , _Msg(""){} 

IRC::Privmsg::~Privmsg(){}

void IRC::Privmsg::excutePrivmsg(Parse *parse, Client* client, Server* server)
{
	if	(client->isregisterd() == false) {
		client->SendServerToClient(":localhost " ERROR_451 " " + client->getNickname() + " :You have not registered");
		return ;
	}

	setSender(client->getNickname());

	if (ParseLine(parse, client) == false)
	{
		if (getMsg() == "")
			client->SendServerToClient(":localhost "  ERR_NOTEXTTOSEND  " " + client->getNickname() + " :No text to send");
		else 
			checkReceive(server, client);
	}
}
bool IRC::Privmsg::ParseLine(Parse *parse, Client* client)
{
    std::vector<std::string> parameters = parse->getParameters();

    if (parameters.empty())
    {
        client->SendServerToClient(":localhost " ERR_NORECIPIENT " " + client->getNickname() + " :No recipient given PRIVMSG");
        client->SendServerToClient(":localhost " ERR_NOTEXTTOSEND " " + client->getNickname() + " :No text to send");
        return true;
    }

    std::string receiver = parameters[0];
    if (receiver[0] == ':')
    {
        client->SendServerToClient(":localhost " ERR_NORECIPIENT " " + client->getNickname() + " :No recipient given PRIVMSG");
        return true;
    }

    setReceiver(receiver);

    bool textFound = false;
    std::vector<std::string>::iterator it;
    for (it = parameters.begin() + 1; it != parameters.end(); ++it)
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
                client->SendServerToClient(":localhost " ERR_NOTEXTTOSEND " " + client->getNickname() + " :No text to send");
                return true;
            }
            setMsg(*it + " ");
        }
    }
	return false;
}


void IRC::Privmsg::checkReceive(Server* server, Client* client)
{
	Parse parse;
	if (getReceiver().length() && getReceiver()[0] == '@' && getReceiver()[1] == '#')
	{
		parse.Debug_msg("send to channel mods");
		std::map<int, Channel *>::iterator it;
		for (it = server->channel_map.begin(); it != server->channel_map.end(); ++it)
		{
			parse.Debug_msg(it->second->getChannelName() + " - " + getReceiver());
			if (it->second->getChannelName() == getReceiver().substr(1))
			{
				std::map<Client *, int>::iterator it2;
				for(it2 = it->second->_clients.begin(); it2 != it->second->_clients.end(); it2++)
				{
					parse.Debug_msg("sending message to an operator");
					if (it2->second == 1 && it2->first->getNickname() != client->getNickname())
						it2->first->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + "@localhost" + " PRIVMSG " + getReceiver() + " :" + this->_Msg);
				}
				return;
			}
		}
		return ;
	}
	else if (getReceiver().length() && getReceiver()[0] == '#')
	{
		std::map<int, Channel *>::iterator it;
		for (it = server->channel_map.begin(); it != server->channel_map.end(); ++it)
		{
			parse.Debug_msg(it->second->getChannelName() + " - " + getReceiver());
			parse.Debug_msg(it->second->getChannelName() == getReceiver() ? "yes" : "no");
			if (it->second->getChannelName() == getReceiver())
			{
				if (it->second->FindClient(client->getNickname()) == NULL) {
					client->SendServerToClient(": " ERR_NOTONCHANNEL " " + client->getNickname() + "!" + client->getUsername() + "@localhost" + " PRIVMSG " + getReceiver() + " :" + "You are not in channel");
					return ;
				}
				parse.Debug_msg("found the channel! " + getReceiver());
				std::map<Client *, int>::iterator it2;
				for(it2 = it->second->_clients.begin(); it2 != it->second->_clients.end(); it2++) {
					parse.Debug_msg(it2->first->getNickname());
					if (it2->second != 2 && it2->first->getNickname() != client->getNickname())
						it2->first->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + "@localhost" + " PRIVMSG " + getReceiver() + " :" + this->_Msg);
				}
				return;
			}
		}
		return ;
		// maybe i should choeck for mode
	}
	// else if (getReceiver().length() >= 2 && getReceiver()[0] == '%' && getReceiver()[1] == '*')
	else if (getReceiver().length() && getReceiver()[0] == '$')
	{
		parse.Debug_msg("send to everyone");
		sendToAll(server, client);
		return;
	}
	else 
	{
		std::map<int , Client *>::iterator it;
		if (!server->clients_map.empty())
		{
			for (it = server->clients_map.begin(); it != server->clients_map.end() ;++it)
			{
				if (it->second->getNickname() == getReceiver())
				{
					server->clients_map[it->first]->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + "@localhost" + " PRIVMSG " + getReceiver() + " :" + this->_Msg); // return (_receiver_fd = it->first); //or return (it->second->getClientFd())
					return;
				}
			}
		}
		return ;
	}
	client->SendServerToClient(":localhost "  ERR_NOSUCHNICK  " " + client->getNickname() + " :No such nick/channel");
}


void IRC::Privmsg::sendToAll(Server* server, Client *client)
{
	std::map<int , Client *>::iterator it;
	for (it = server->clients_map.begin(); it != server->clients_map.end() ;++it)
		if (it->second->getNickname() != client->getNickname())
			server->clients_map[it->second->getClientFd()]->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + "@" + "localhost PRIVMSG $ :" + this->_Msg);
}

void IRC::Privmsg::setReceiver(std::string str){ this->_receiver = str;}
void IRC::Privmsg::setMsg(std::string str){this->_Msg += str;}
void IRC::Privmsg::setSender(std::string str){this->_Sender += str;}


std::string IRC::Privmsg::getReceiver(){return _receiver;}
std::string IRC::Privmsg::getMsg(){return _Msg;}
std::string IRC::Privmsg::getSender(){return _Sender;}



/*     Command: PRIVMSG
  Parameters: <target>{,<target>} <text to be sent>
  <target> is the nickname of a client or the name of a channel.
  
  ERR_NOSUCHNICK (401)   "<client> <nickname> :No such nick/channel"
ERR_NOSUCHSERVER (402)   "<client> <server name> :No such server"
ERR_CANNOTSENDTOCHAN (404) "<client> <channel> :Cannot send to channel"
ERR_TOOMANYTARGETS (407) 
ERR_NORECIPIENT (411)  "<client> :No recipient given (<command>)"
ERR_NOTEXTTOSEND (412)   "<client> :No text to send"
ERR_NOTOPLEVEL (413)
ERR_WILDTOPLEVEL (414)
RPL_AWAY (301)
*/