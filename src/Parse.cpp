#include "../include/Parse.hpp"


Parse::Parse(){}
Parse::~Parse(){}

void Parse::getCommandInfo(const std::string& clientMsg, Client *client) 
{
    std::string messageLine, command, para, buffer;
    std::vector<std::string> parameters;

    buffer = clientMsg;

    size_t pos;
    while ((pos = buffer.find("\n")) != std::string::npos || buffer.size() > 0) 
	{
        if (pos != std::string::npos) {
            messageLine = buffer.substr(0, pos);
			client->_ClientToServer += buffer.substr(0, pos);
            buffer.erase(0, pos + 1);
        } else {
			client->_ClientToServer += buffer;
            messageLine = buffer;
            buffer.clear();
			break;
        }
        if (client->_ClientToServer.empty() || client->_ClientToServer.length() == 1)
            continue;
        std::istringstream lineStream(client->_ClientToServer);
        lineStream >> command;

        while (lineStream >> para)
            parameters.push_back(para);

        _messages.push_back(std::make_pair(command, parameters));
        parameters.clear();
		client->_ClientToServer.clear();
    }
}

void Parse::Debug_msg(std::string msg)
{
	std::cout << RED << "-----------> Debug MSG <-------------" << RESET << std::endl;
	std::cout  << msg << std::endl << std::endl;
	std::cout << RED << "-------------------------------------" << RESET << std::endl;

	}

void Parse::printcommandinfo()
{
    std::vector<std::pair<std::string, std::vector<std::string> > >::iterator it;
    std::vector<std::string>::iterator it2;
	// int i = 0;
	std::cout << BLUE << "-----------> command info <-------------" << RESET << std::endl;
    // for (it = _messages.begin(); it != _messages.end(); ++it)
    // {
    //     std::cout << "Command: " << it->first << std::endl;
	// 	i = 0;
    //     for (it2 = it->second.begin(); it2 != it->second.end(); ++it2)
    //     {
    //         std::cout << "Parameter "<< i << ": "<< *it2 << std::endl;
	// 		i++;
    //     }
    // }
	std::cout << BLUE << "-------------------------------------" << RESET << std::endl;

}


std::vector<std::string> Parse::getParameters()
{
	return (_messages.front().second);
}

std::string Parse::getCommand()
{
	if (!_messages.empty())
        return _messages.front().first;
	else
        return "";
}

void Parse::sendToClient(Client *client) 
{
	if (client->_ServerToClient != "")
	{
		std::string finalMsg = client->_ServerToClient;
		if (send(client->getClientFd(), finalMsg.c_str(), finalMsg.size(), 0) == -1)
			Debug_msg("Failed to send message to the client");
		client->_ServerToClient = "";
	}
}
