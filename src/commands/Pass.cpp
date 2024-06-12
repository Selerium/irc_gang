#include "../../include/IRC.hpp"

IRC::Pass::Pass(){}
IRC::Pass::~Pass(){}

void IRC::Pass::excutePass(Parse *parse,Client* client, Server* server, int client_fd)
{
	// 1) ---> set pass
	std::vector<std::string> parameters = parse->getParameters();
	if (parameters.empty())
	{
		Parse().sendToClient(ERROR_461, client_fd, "");
		return;
	}
	else
		setClientPass(parameters[0]);

	// 2) ---> check if client already authanticated??
	if (ClientisAuthanticated(client, server, client_fd))
	{
		Parse().sendToClient(ERROR_462, client_fd, parse->getCommand());
		return;
	}

	if (checkClientPass(server))
	{
		Parse().sendToClient(ERROR_464, client_fd, parse->getCommand());
		return;
	}

		// ADD CLIENT TO CLIENT MAP USING NEW ?
		// server->clients_map[client_fd] = new Client(client_fd, 1);
}

void IRC::Pass::setClientPass(std::string pass) {this->_clientPass = pass;}

std::string IRC::Pass::getClientPass() { return this->_clientPass;}

bool IRC::Pass::checkClientPass(Server* server)
{
	if (getClientPass() != server->getchannelPass())
		return true;
	return false;

}

bool IRC::Pass::ClientisAuthanticated(Client* client, Server* server, int client_fd)
{
	(void)client;
    // ---> Check if the client_fd exists in the clients_map

    if (server->clients_map.find(client_fd) != server->clients_map.end())
        return true;
    // ---> Client not found, so it is not authenticated
    return false;
}

/*      Command: PASS
   Parameters: <password>

   The PASS command is used to set a 'connection password'.  The
   password can and must be set before any attempt to register the
   connection is made.  Currently this requires that clients send a PASS
   command before sending the NICK/USER combination and servers *must*
   send a PASS command before any SERVER command.  The password supplied
   must match the one contained in the C/N lines (for servers) or I
   lines (for clients).  It is possible to send multiple PASS commands
   before registering but only the last one sent is used for
   verification and it may not be changed once registered.  Numeric
   Replies:

           ERR_NEEDMOREPARAMS              ERR_ALREADYREGISTRED
		   
*/

/*ERR_NEEDMOREPARAMS
                        "<command> :Not enough parameters"*/

/*462     ERR_ALREADYREGISTRED
                        ":You may not reregister"*/