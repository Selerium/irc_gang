#include "../../include/IRC.hpp"

IRC::Privmsg::Privmsg() : _receiver("") , _Msg(""){} 

IRC::Privmsg::~Privmsg(){}

void IRC::Privmsg::excutePrivmsg(Parse *parse, Client* client, Server* server)
{

	setSender(client->getNickname());

	if (ParseLine(parse, client) == false)
	{
		if (getMsg() == "")
			client->SendServerToClient(": "  ERR_NOTEXTTOSEND  " :" + client->getNickname() + " :No text to send\r\n");
		else 
			checkReceive(server, client);
	}
}
bool IRC::Privmsg::ParseLine(Parse *parse, Client* client)
{
    std::vector<std::string> parameters = parse->getParameters();

    if (parameters.empty())
    {
        client->SendServerToClient(":" ERR_NORECIPIENT ":" + client->getNickname() + " :No recipient given PRIVMSG\r\n");
        client->SendServerToClient(":" ERR_NOTEXTTOSEND ":" + client->getNickname() + " :No text to send");
        return true;
    }

    std::string receiver = parameters[0];
    if (receiver[0] == ':')
    {
        client->SendServerToClient(":" ERR_NORECIPIENT ":" + client->getNickname() + " :No recipient given PRIVMSG\r\n");
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
                client->SendServerToClient(":" ERR_NOTEXTTOSEND ":" + client->getNickname() + " :No text to send\r\n");
                return true;
            }
            setMsg(*it + " ");
        }
    }
	return false;
}


void IRC::Privmsg::checkReceive(Server* server, Client* client)
{
	if (getReceiver()[1] == '@' && getReceiver()[0] == '#')
	{
		std::map<int, Channel *>::iterator it;
		for (it = server->channel_map.begin(); it !=server->channel_map.end(); ++it)
		{
			if (it->second->getChannelName() == getReceiver())
			{
				std::map<Client *, int>::iterator it2;
				for(it2 = it->second->_clients.begin(); it2 == it->second->_clients.end(); it2++)
				{
					if (it->second->_clients.find(client)->second == 1)
						it2->first->SendServerToClient("[ " + getSender() + " ] " +  this->_Msg);
				}
				return;
			}
		}
	}
	else if (getReceiver()[0] == '#')
	{
		std::map<int, Channel *>::iterator it;
		for (it = server->channel_map.begin(); it !=server->channel_map.end(); ++it)
		{
			if (it->second->getChannelName() == getReceiver())
			{
				std::map<Client *, int>::iterator it2;
				for(it2 = it->second->_clients.begin(); it2 == it->second->_clients.end(); it2++)
					it2->first->SendServerToClient("[ " + getSender() + " ] " +  this->_Msg);
			return;
			}
		}
		// maybe i should choeck for mode
	}
	else if (getReceiver()[0] == '%' && getReceiver()[1] == '*' && getReceiver().length() == 2)
	{
		sendToAll(server);
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
					server->clients_map[it->first]->SendServerToClient("[ " + getSender() + " ] " +  this->_Msg); // return (_receiver_fd = it->first); //or return (it->second->getClientFd())
					return;
				}
			}
		}
	}
	client->SendServerToClient(": "  ERR_NOSUCHNICK  " :" + client->getNickname() + "  :No such nick/channel\r\n");
}


void IRC::Privmsg::sendToAll(Server* server)
{
	std::map<int , Client *>::iterator it;
	for (it = server->clients_map.begin(); it != server->clients_map.end() ;++it)
			server->clients_map[it->second->getClientFd()]->SendServerToClient("[ " + getSender() + " ] " +  this->_Msg);
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