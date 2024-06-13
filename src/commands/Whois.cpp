#include "../../include/IRC.hpp"

IRC::Whois::Whois(){}

IRC::Whois::~Whois(){}

void IRC::Whois::excuteWhois(Parse *parse, Client* client, Server* server)
{
	if (!parse->getParameters().empty())
	{
		std::map<int , Client *>::iterator it;
		if (!server->clients_map.empty())
		{
			for (it = server->clients_map.begin(); it != server->clients_map.end() ; ++it)
			{
				parse->Debug_msg(it->second->getNickname());
				if (it->second->getNickname() == parse->getParameters()[0] && it->second->isregisterd() == true)
				{
						client->SendServerToClient(": " ERR_NOSUCHNICK " "  
						+ client->getNickname() +  " " + parse->getParameters()[0] 
						+ " :has identified for this nick\r\n");

						client->SendServerToClient(": " RPL_WHOISUSER " "  
						+ client->getNickname() +  " " + it->second->getNickname() + " " + it->second->getUsername()
						+ " " + it->second->getHostname()
						+ " * :" + it->second->getUsername() + "\r\n");

						client->SendServerToClient(": " RPL_ENDOFWHOIS " "  
						+ client->getNickname() +  " " + it->second->getNickname()
						+ " :End of /WHOIS list\r\n");
						return;
				}
			}
		}
		client->SendServerToClient( ": " ERR_NOSUCHNICK " " +  client->getNickname() +  " " + parse->getParameters()[0] + " :No such nick/channel\r\n"); 
	}
	else
		client->SendServerToClient( ": " ERR_NONICKNAMEGIVEN " " +  client->getNickname() + " :No nickname given\r\n");

}


/*

RPL_ENDOFWHOIS (318) 
  "<client> <nick> :End of /WHOIS list"

ERR_NOSUCHNICK (401) 
  "<client> <nickname> :No such nick/channel"

ERR_NOSUCHSERVER (402) 
  "<client> <server name> :No such server"

ERR_NONICKNAMEGIVEN (431) 
  "<client> :No nickname given"


REPLAYS 

RPL_WHOISREGNICK (307) 
  "<client> <nick> :has identified for this nick"

RPL_WHOISUSER (311) 
  "<client> <nick> <username> <host> * :<realname>"


RPL_WHOISOPERATOR (313) 
  "<client> <nick> :is an IRC operator"


RPL_WHOISCHANNELS (319) 
  "<client> <nick> :[prefix]<channel>{ [prefix]<channel>}

  RPL_WHOISHOST (378) 
  "<client> <nick> :is connecting from *@localhost 127.0.0.1"

  RPL_WHOISMODES (379) 
  "<client> <nick> :is using modes +ailosw"

RPL_WHOISSECURE (671) 
  "<client> <nick> :is using a secure connection"


  */