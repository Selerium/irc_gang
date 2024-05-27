#include "../../include/IRC.hpp"


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
}

IRC::Commands::~Commands(){}

void IRC::Commands::executeCommand(Parse *parse ,Client* client, Server* server, int client_fd) 
{
    std::map<std::string, CommandFunction>::iterator it = commandMap.find( parse->getCommand());
    if (it != commandMap.end())
        (this->*(it->second))(parse,client, server, client_fd);
    else 
    {
        //unknown command
        Parse().sendToClient(ERROR_421, client_fd, Parse().getCommand());
    }
}

void IRC::Commands::pass(Parse *parse,Client* client, Server* server, int client_fd)
{
    IRC::Pass().excutePass(parse,client,server,client_fd);
}

void IRC::Commands::join(Parse *parse, Client* client, Server* server, int client_fd)
{
    IRC::Join().excuteJoin(parse,client,server,client_fd);
}

void IRC::Commands::nick(Parse *parse, Client* client, Server* server, int client_fd)
{
    IRC::Nick().excuteNick(parse,client,server,client_fd);
}

void IRC::Commands::user(Parse *parse, Client* client, Server* server, int client_fd)
{
    IRC::User().excuteUser(parse,client,server,client_fd);
}

void IRC::Commands::whois(Parse *parse, Client* client, Server* server, int client_fd)
{
    IRC::Whois().excuteWhois(parse,client,server,client_fd);
}

void IRC::Commands::mode(Parse *parse, Client* client, Server* server, int client_fd)
{
    IRC::Mode().excuteMode(parse,client,server,client_fd);
}

void IRC::Commands::ping(Parse *parse, Client* client, Server* server, int client_fd)
{
    IRC::Ping().excutePing(parse,client,server,client_fd);
}

void IRC::Commands::cap(Parse *parse, Client* client, Server* server, int client_fd)
{
	IRC::Cap().excuteCap(parse,client,server,client_fd);
}

void IRC::Commands::quit(Parse *parse, Client* client, Server* server, int client_fd)
{
    IRC::Quit().excuteQuit(parse,client,server,client_fd);
}

