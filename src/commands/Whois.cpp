#include "../../include/IRC.hpp"

IRC::Whois::Whois(){}

IRC::Whois::~Whois(){}

void IRC::Whois::excuteWhois(Parse *parse, Client* client, Server* server)
{
	if	(client->isregisterd() == false) {
		client->SendServerToClient(" : " ERROR_451 " " + client->getNickname() + " :You have not registered");
		return ;
	}

	if (!parse->getParameters().empty())
	{
		std::map<int , Client *>::iterator it;
		if (!server->clients_map.empty())
		{
			for (it = server->clients_map.begin(); it != server->clients_map.end() ; ++it)
			{
				parse->Debug_msg(it->second->getNickname());
				parse->Debug_msg(parse->getParameters()[0]);
				if (it->second->getNickname() == parse->getParameters()[0] && it->second->isregisterd() == true)
				{
						client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_NOSUCHNICK " WHOIS " + parse->getParameters()[0] + " :has identified for this nick");

						client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " RPL_WHOISUSER " WHOIS " + it->second->getNickname() + " " + it->second->getUsername() + " " + it->second->getHostname() + " * :" + it->second->getUsername());

						client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " RPL_ENDOFWHOIS " WHOIS " + client->getNickname() +  " " + it->second->getNickname() + " :End of /WHOIS list");
						return;
				}
			}
		}
		client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_NOSUCHNICK " " +  client->getNickname() +  " " + parse->getParameters()[0] + " :No such nick/channel"); 
	}
	else
		client->SendServerToClient(":" + client->getNickname() + "!" + client->getUsername() + " " ERR_NONICKNAMEGIVEN " " +  client->getNickname() + " :No nickname given");

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
  "<client> <nick> :is connecting from * 127.0.0.1"

  RPL_WHOISMODES (379) 
  "<client> <nick> :is using modes +ailosw"

RPL_WHOISSECURE (671) 
  "<client> <nick> :is using a secure connection"


  */