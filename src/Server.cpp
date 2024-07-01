#include "../include/Server.hpp"

int signal_flag = 0;

////////---------->>>>>>>> Error handling exception <<<<<<<<<<--------------/////////

const char* ErrorException::what() const throw() 
{
	return message;
}

ErrorException::ErrorException(const char *msg) : message(msg) {}


////////---------->>>>>>>> Server set up <<<<<<<<<<--------------/////////

Server::Server(char **argv) : socket_fd(-1) , num_fd(1) , fd_size(10)  , port("6667")
{
	try
	{
		signal(SIGINT,signalHandler);
		signal_flag = 0;
		this->pfds = new struct pollfd[this->fd_size];
		setPort(argv[1]);
		setPass(argv[2]);
		start_IRC();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
}

Server::~Server()
{
	std::cout << "Free all data" << std::endl;
	if (this->result != NULL) {
        freeaddrinfo(this->result);
    }
	delete[] this->pfds;
}

void Server::GetaddrInfo()
{
	// 1) Define hints for getaddrinfo : it allows you to specify 
	//criteria for selecting socket address
	struct addrinfo hints;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP socket
	hints.ai_flags = AI_PASSIVE; // For wildcard IP address : the returned addresses will be used for a server's bind call,
	hints.ai_protocol = IPPROTO_TCP;

	int status = getaddrinfo(NULL, &this->port[0], &hints, &this->result); //get address information that matches specific criteria.
	if (status != 0)
		throw ErrorException("getaddinfo failed");
}

void Server::socket_int()
{
	//create socket
	int reuseaddr = 1;
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd == -1) {
        throw ErrorException("Error: Could not create socket");}

	/* Set the SO_REUSEADDR option : to allow reusing local addresses.
	This is useful, for example, when restarting a server program
	 that needs to bind to the same port quickly after shutdown. */
    if (setsockopt(socket_fd , SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr))){
        throw ErrorException("setsockopt faild");}

	if (fcntl(socket_fd,F_SETFD ,O_NONBLOCK) == -1)
        throw ErrorException("fcntl faild");
}

void Server::Bind_listen()
{
	// Bind to an IP address and port
	if (bind(this->socket_fd, this->result->ai_addr, this->result->ai_addrlen) != 0)
		throw ErrorException("Bind failed");
	
	// set socket to listinig 
	if(listen(this->socket_fd, 10) != 0)
		throw ErrorException("listen faild");
}

void Server::poll_fd()
{
	/*The returned value indicates the total number of 
	file descriptors for which events were reported.*/
	this->num_poll = poll(this->pfds, this->num_fd, -1);

	// chcking ? 
	if (num_poll == -1)
		throw ErrorException("");
}

void Server::add_new_client()
{
	// Accept a new client connection
	struct sockaddr_in client_addr;
	socklen_t client_addrlen = sizeof(client_addr);
	this->client_sockfd = accept(this->socket_fd, (struct sockaddr *)&client_addr, &client_addrlen);
	if (this->client_sockfd == -1)
		throw ErrorException("accept faild");
	
	//resize pfds array if it reach max size 
	if(this->num_fd == this->fd_size)
	{
		this->fd_size++;
		struct pollfd *tmp = new struct pollfd[this->fd_size];
		for (int i = 0; i < this->fd_size; i++)
			tmp[i] = this->pfds[i];
		delete[] this->pfds;
		this->pfds = tmp;
	}

	// Add the new client socket to the array
	pfds[num_fd].fd = this->client_sockfd;
	pfds[num_fd].events = POLLIN | POLLOUT; //BOTH READ AND WRITE EAVENT EILL BE CHECKED
	this->num_fd++;
	clients_map[this->client_sockfd] = new Client(this->client_sockfd, inet_ntoa(client_addr.sin_addr));
}


void Server::read_message(int fd, Parse *parse)
{
	client_msg = "";
	char buffer[1024];
	ssize_t num_bytes = recv(pfds[fd].fd, buffer, sizeof(buffer), 0);
	if (num_bytes == 0)
	{
		// Connection closed by the client
		// Remove the client socket from the array
		close(pfds[fd].fd);
		for (int j = fd; j < num_fd; ++j)
			pfds[j] = pfds[j + 1];
		num_fd--;

		Client * client = clients_map.find(fd)->second;

		std::cout << "bye bye" << std::endl;

		if (!this->channel_map.empty()) {
		for (std::map<int, Channel *>::iterator it = this->channel_map.begin(); it != this->channel_map.end(); it++) {
			if (it->second->FindClient(client->getNickname()))
				it->second->sendToall(client->getNickname() + "!" + client->getUsername() + "@localhost QUIT : Disconnected from client");
		}
		clients_map.erase(this->client_sockfd);
	}
	}
	else
	{
		// buffer[num_bytes] = '\0';
		client_msg += buffer;
		memset(buffer, 0, sizeof(buffer));
		parse->getCommandInfo(client_msg, clients_map[this->client_sockfd]);
		parse->printcommandinfo();
		IRC::Commands().executeCommand(parse,clients_map[this->client_sockfd], this);
	}
	// printClients();
	parse->_messages.clear();
}

////////---------->>>>>>>> Start IRC server<<<<<<<<<<--------------/////////


void Server::signalHandler(int signum) 
{
    std::cout << "\nInterrupt signal (" << signum << ") received." << std::endl;
	signal_flag = 1;
}


void Server::start_IRC()
{
	Parse parse;
	GetaddrInfo();
	socket_int();
	Bind_listen();
	pfds[0].fd = socket_fd;
	pfds[0].events = POLLIN;
	while(true)
	{
		if (signal_flag == 1)
			throw ErrorException("");
		poll_fd();
		// Check for events on the server socket
		if (pfds[0].revents & POLLIN)
			add_new_client();
		// Check for events on client sockets
		for (int i = 1; i < this->num_fd; i++)
		{
			this->client_sockfd = pfds[i].fd;
			if (pfds[i].revents & POLLIN)
			{
				// parse.Debug_msg("test");
				// Read data from the client
				// Parse parse;
				read_message(i, &parse);
			}
			else if (pfds[i].revents & POLLOUT)
			{
				parse.sendToClient(clients_map[this->client_sockfd]);
			}
		}
	}
}

////////---------->>>>>>>> Setters <<<<<<<<<<--------------/////////

void Server::setPort(std::string port)
{
	if (std::strtol(port.c_str(), NULL, 10) >= 0 && std::strtol(port.c_str(), NULL, 10) <= 1023)
		throw ErrorException("port num is within resevrd range(0 - 1023)");
	this->port = port;
}

void Server::setPass(std::string pass) {this->_ServerPass = pass; }

std::string Server::getServerPass() { return this->_ServerPass;}

void Server::setNumFds(int size) {
	this->num_fd = size;
}

////////---------->>>>>>>> Getters <<<<<<<<<<--------------/////////

int Server::getFdSize() const {
	return this->fd_size;
}

int Server::getNumFds() const {
	return this->num_fd;
}

void Server::printClients()
{
	std::map<int , Client *>::iterator it;

	if (!clients_map.empty())
	{
		for (it = this->clients_map.begin(); it != this->clients_map.end() ;++it)
		{
			if (it->first && it->second)
				std::cout << it->second->getClientFd() << " " << it->second->getUsername() << std::endl; 
		}
	}

}


