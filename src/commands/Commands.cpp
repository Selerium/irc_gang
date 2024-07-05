#include "../../include/IRC.hpp"
#include<algorithm>


IRC::Commands::Commands(){
    			commandMap["PASS"] = &Commands::pass;
    			commandMap["JOIN"] = &Commands::join;
				commandMap["NICK"] = &Commands::nick;
				commandMap["USER"] = &Commands::user;
				commandMap["WHOIS"] = &Commands::whois;
				commandMap["MODE"] = &Commands::mode;
				commandMap["PING"] = &Commands::ping;
				commandMap["CAP"] = &Commands::cap;
				commandMap["QUIT"] = &Commands::quit;
				commandMap["PRIVMSG"] = &Commands::privmsg;
				commandMap["TOPIC"] = &Commands::topic;
				commandMap["KICK"] = &Commands::kick;
				commandMap["INVITE"] = &Commands::invite;
}


IRC::Commands::~Commands(){}

std::string toUpperCase(const std::string& str) 
{
	std::string upperCaseStr = str;
	std::transform(upperCaseStr.begin(), upperCaseStr.end(), upperCaseStr.begin(), ::toupper);
	return upperCaseStr;
}

void IRC::Commands::executeCommand(Parse *parse ,Client* client, Server* server) 
{
	while (!parse->_messages.empty())
	{

		std::map<std::string, CommandFunction>::iterator it = commandMap.find(toUpperCase(parse->getCommand()));
		if (it != commandMap.end())
		{
			(this->*(it->second))(parse,client, server);
			if (client->getWelcomeMsg() == false && client->getAuthantication() == true && client->isregisterd() == true)
			{
				WelcomeMsg(client);
				client->setWelcomeMsg(true);
			}

		}
		else 
		{
			//unknown command
			client->SendServerToClient(": 421 "  ERROR_421  " " + parse->getCommand());
		}
		parse->_messages.erase(parse->_messages.begin());
	}
}

void IRC::Commands::pass(Parse *parse,Client* client, Server* server)
{
    IRC::Pass().excutePass(parse,client,server);
}

void IRC::Commands::join(Parse *parse, Client* client, Server* server)
{
    IRC::Join().excuteJoin(parse,client,server);
}

void IRC::Commands::nick(Parse *parse, Client* client, Server* server)
{
    IRC::Nick().excuteNick(parse,client,server);
}

void IRC::Commands::user(Parse *parse, Client* client, Server* server)
{
    IRC::User().excuteUser(parse,client,server);
}

void IRC::Commands::whois(Parse *parse, Client* client, Server* server)
{
    IRC::Whois().excuteWhois(parse,client,server);
}

void IRC::Commands::mode(Parse *parse, Client* client, Server* server)
{
    IRC::Mode().excuteMode(parse,client,server);
}

void IRC::Commands::ping(Parse *parse, Client* client, Server* server)
{
    IRC::Ping().excutePing(parse,client,server);
}

void IRC::Commands::cap(Parse *parse, Client* client, Server* server)
{
    IRC::Cap().excuteCap(parse,client,server);
}

void IRC::Commands::quit(Parse *parse, Client* client, Server* server)
{
    IRC::Quit().excuteQuit(parse,client,server);
}

void IRC::Commands::privmsg(Parse *parse,Client* client, Server* server)
{
    IRC::Privmsg().excutePrivmsg(parse,client,server);
}


void IRC::Commands::topic(Parse *parse, Client* client, Server* server)
{
    IRC::Topic().excuteTopic(parse,client,server);
}

void IRC::Commands::kick(Parse *parse, Client* client, Server* server)
{
    IRC::Kick().excuteKick(parse,client,server);
}

void IRC::Commands::invite(Parse *parse, Client* client, Server* server)
{
    IRC::Invite().excuteInvite(parse,client,server);
}

void IRC::Commands::WelcomeMsg(Client* client)
{
	std::string welcomeMsg = ":" 
								"" RPL_WELCOME " "
								+ client->getNickname()
								+ " :Welcome to the Internet Relay Network, "
								+ client->getNickname()
								+ "!"
								+ client->getUsername()
								+ "\r\n";
	client->SendServerToClient(welcomeMsg);
	client->SendServerToClient(":irc 002 " + client->getNickname() + " :Your host is irc, running version 1.3");
	client->SendServerToClient(":irc 003 " + client->getNickname() + " :This server was created july->2024");
	client->SendServerToClient(":irc 004 " + client->getNickname() + " irc 1.3  itklo");
}