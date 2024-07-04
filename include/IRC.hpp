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
#include <fcntl.h>
#include <csignal> 
#include <cstdlib>
#include <cctype>

#include "IRC.hpp"
#include "Parse.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "ErrorMsg.hpp"

#define USERLEN 12


class Server;
class Client;


class IRC
{
public:
    // claas 1
    class Commands
    {
    public:
        typedef void (Commands::*CommandFunction)(Parse *,Client *, Server *);
        std::map<std::string, CommandFunction> commandMap;

        Commands();
        ~Commands();

        void pass(Parse *parse,Client *client, Server *server);
        void join(Parse *parse,Client *client, Server *server);
        void nick(Parse *parse,Client *client, Server *server);
        void user(Parse *parse,Client *client, Server *server);
        void whois(Parse *parse,Client *client, Server *server);
        void mode(Parse *parse,Client *client, Server *server);
        void ping(Parse *parse,Client *client, Server *server);
        void cap(Parse *parse,Client *client, Server *server);
		void privmsg(Parse *parse,Client* client, Server* server);
        void quit(Parse *parse,Client *client, Server *server);
        void topic(Parse *parse,Client *client, Server *server);
        void kick(Parse *parse,Client *client, Server *server);
        void invite(Parse *parse,Client *client, Server *server);


		
        void executeCommand(Parse *parse, Client *client, Server *server);
		void WelcomeMsg(Client* client);
        
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
            void excutePass(Parse *parse,Client* client, Server* server);
            void setClientPass(std::string pas);
            std::string getClientPass();
            bool checkClientPass(Server* server);
            bool ClientisAuthanticated(Client* client);


    };

    	class Join 
	{
		private:

		public:
			Join();
			~Join();
			void excuteJoin(Parse *parse, Client* client, Server* server);
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
            void excuteNick(Parse *parse, Client* client, Server* server);

    };


    class User 
    {
        private:

        public:
            User();
            ~User();
            std::string realname;  
           void excuteUser(Parse *parse, Client* client, Server* server);
           int parseMsg(Parse *parse, Client* client);
    };
    class Whois 
    {
        private:

        public:
            Whois();
            ~Whois();
           void excuteWhois(Parse *parse, Client* client, Server* server);


    };
    class Mode 
    {
        private:

        public:
            Mode();
            ~Mode();
           void excuteMode(Parse *parse, Client* client, Server* server);

    };
    class Ping 
    {
        private:

        public:
            Ping();
            ~Ping();
           void excutePing(Parse *parse, Client* client, Server* server);

    };
    class Cap 
    {
        private:

        public:
            Cap();
            ~Cap();
           void excuteCap(Parse *parse, Client* client, Server* server);


    };
    class Quit 
    {
        private:

        public:
            Quit();
            ~Quit();
           void excuteQuit(Parse *parse, Client* client, Server* server);

    };

	class Privmsg
	{
		private:
			std::string _receiver;
			std::string _Msg;
			std::string _Sender;




        public:
            Privmsg();
            ~Privmsg();
           void excutePrivmsg(Parse *parse, Client* client, Server* server);
		   bool ParseLine(Parse *parse, Client* client);
		   void checkReceive(Server* server, Client* client);
		   void sendToAll(Server* server);

		   void setReceiver(std::string str);
		   void setMsg(std::string str);
		   void setSender(std::string str);

		//    void sendClientToClient(Server* server);

		   std::string getSender();
		   std::string getReceiver();
		   std::string getMsg();

	};

	class Topic 
    {
        private:

        public:
            Topic();
            ~Topic();
           void excuteTopic(Parse *parse, Client* client, Server* server);
		   int parseMsg(Parse *parse, Client* client);
		   void checkChannel(Client* client, Server* server);
		   void CheckTopic(Client* client, Server* server);
		   
		   std::string setChannel;
		   std::string topic;
    };

	class Kick 
    {
        private:

        public:
            Kick();
            ~Kick();
           void excuteKick(Parse *parse, Client* client, Server* server);
		   int parseMsg(Parse *parse, Client* client);
		   void checkChannel(Client* client, Server* server);

		   std::string setChannel;
		   std::string setkickNick;
		   std::string comment;
    };

	class Invite 
    {
        private:

        public:
            Invite();
            ~Invite();
           void excuteInvite(Parse *parse, Client* client, Server* server);
		   int parseMsg(Parse *parse, Client* client);
		   void checkChannel(Client* client, Server* server);
		   int checkUser(Client* client, Server* server);

		   	std::string setChannel;
			std::string setInviteNick;
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
