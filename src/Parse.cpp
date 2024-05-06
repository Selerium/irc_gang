#include "../include/Parse.hpp"


Parse::Parse(){}
Parse::~Parse(){}

void Parse::getCommandInfo(const std::string& clientMsg) 
{

	std::string messageLine, command, para;
	std::vector<std::string> parameters;

	std::istringstream messageStream(clientMsg);

	while (std::getline(messageStream, messageLine)) {
		std::istringstream lineStream(messageLine);
		lineStream >> command;

		while (lineStream >> para)
			parameters.push_back(para);
		_messages.push_back(std::make_pair(command, parameters));
		parameters.clear();
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
	int i = 0;
	std::cout << BLUE << "-----------> command info <-------------" << RESET << std::endl;
    for (it = _messages.begin(); it != _messages.end(); ++it)
    {
        std::cout << "Command: " << it->first << std::endl;
		i = 0;
        for (it2 = it->second.begin(); it2 != it->second.end(); ++it2)
        {
            std::cout << "Parameter "<< i << ": "<< *it2 << std::endl;
			i++;
        }
    }
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

void Parse::sendToClient(std::string msg, int clientFd, std::string command) 
{
	std::string finalMsg = command + msg + "\n";
	if (send(clientFd, finalMsg.c_str(), finalMsg.size(), 0) == -1)
		Debug_msg("Failed to send message to the client");
}
