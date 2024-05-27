#ifndef SERVER_HPP
#define	SERVER_HPP

#include "IRC.hpp"
#include "Parse.hpp"

class IRC;
class Client;
class Parse;

class Server
{
	private:
		struct addrinfo *result;
		int socket_fd;
		int num_poll;
		int num_fd;
		int fd_size;
		std::string port;
		int client_sockfd;
		std::string client_msg;
		std::string _ServerPass;

	public:
		std::map<int , Client *> clients_map;
		struct pollfd *pfds;// an array of pollfd structures


		Server(char **argv);
		~Server();

		void GetaddrInfo();
		void socket_int();
		void Bind_listen();
		void poll_fd();

		void start_IRC();
		void add_new_client();
		void read_message(int fd);

		void setPort(std::string port);
		void setPass(std::string pass);

		std::string getServerPass();

		/// functions for testing - check clients and fds  //
		void printClients();

};

class ErrorException : public std::exception 
{
private:
    const char *message;

public:
    ErrorException(const char* msg);
    const char* what() const throw();
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