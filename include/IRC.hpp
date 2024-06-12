#ifndef IRC_HPP
#define IRC_HPP

#include <iostream>
#include <cstring>  // for memset
#include <unistd.h> // for close
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> // for struct addrinfo
#include <poll.h>
#include <map>
#include <sstream>
#include <vector>
#include <arpa/inet.h>
#include <cstdlib>

#include "IRC.hpp"
#include "Parse.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "ErrorMsg.hpp"


class Server;
class Client;

class IRC
{
public:
	// claas 1
	class Commands
	{
	public:
		typedef void (Commands::*CommandFunction)(Parse *,Client *, Server *, int);
		std::map<std::string, CommandFunction> commandMap;

		Commands();
		~Commands();

		void pass(Parse *parse,Client *client, Server *server, int client_fd);
		void join(Parse *parse,Client *client, Server *server, int client_fd);
		void nick(Parse *parse,Client *client, Server *server, int client_fd);
		void user(Parse *parse,Client *client, Server *server, int client_fd);
		void whois(Parse *parse,Client *client, Server *server, int client_fd);
		void mode(Parse *parse,Client *client, Server *server, int client_fd);
		void ping(Parse *parse,Client *client, Server *server, int client_fd);
		void cap(Parse *parse,Client *client, Server *server, int client_fd);
		void quit(Parse *parse,Client *client, Server *server, int client_fd);

		void executeCommand(Parse *parse, Client *client, Server *server, int client_fd);
		
	};

	// class 2
	// commands classes
	class Pass
	{
		private:
			std::string _clientPass;

		public:
			Pass();
			~Pass();
			void excutePass(Parse *parse,Client* client, Server* server, int client_fd);
			void setClientPass(std::string pas);
			std::string getClientPass();
			bool checkClientPass(Server* server);
			bool ClientisAuthanticated(Client* client, Server* server, int client_fd);


	};

	class Join 
	{
		private:

		public:
			Join();
			~Join();
			void excuteJoin(Parse *parse, Client* client, Server* server, int client_fd);
			void createChannel(std::string channelname, std::string pass, Server* server, Client* client);
			bool channelPass(std::string channelname, std::string password, Server* server);
			bool channelExist(std::string channelname, Server* server);
			void joinChannel(std::string channelname, std::string pass, Server* server, Client* client);
	};

	class Nick 
	{
		private:

		public:
			Nick();
			~Nick();
			void excuteNick(Parse *parse, Client* client, Server* server, int client_fd);

	};


	class User 
	{
		private:

		public:
			User();
			~User();
			void excuteUser(Parse *parse, Client* client, Server* server, int client_fd);
			bool isRegisterd(Server* server, int client_fd);
	};
	class Whois 
	{
		private:

		public:
			Whois();
			~Whois();
		   void excuteWhois(Parse *parse, Client* client, Server* server, int client_fd);

	};
	class Mode 
	{
		private:

		public:
			Mode();
			~Mode();
			void excuteMode(Parse *parse, Client* client, Server* server, int client_fd);

	};
	class Ping 
	{
		private:

		public:
			Ping();
			~Ping();
		   void excutePing(Parse *parse, Client* client, Server* server, int client_fd);

	};
	class Cap 
	{
		private:

		public:
			Cap();
			~Cap();
		   void excuteCap(Parse *parse, Client* client, Server* server, int client_fd);


	};
	class Quit 
	{
		private:

		public:
			Quit();
			~Quit();
			void excuteQuit(Parse *parse, Client* client, Server* server, int client_fd);

	};
};


#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

#endif
