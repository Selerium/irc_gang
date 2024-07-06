#include "../include/Client.hpp"

Client::Client(int fd, std::string hostname) : _Username(""), _Nickname(""), _hostName(hostname), _RealName(""), _ClientFd(fd),
	_welcomMsgsent(false), _isAuthanticated(false), _ClientToServer(""), _ServerToClient(""), UserSet(false)
{

}

int Client::getClientFd() {return this->_ClientFd; }

bool Client::getAuthantication() {return this->_isAuthanticated; }

std::string Client::getUsername() { return this->_Username;}

std::string Client::getNickname() { return this->_Nickname;}

std::string Client::getHostname() { return this->_hostName;}

std::string Client::getRealname() { return this->_RealName;}

void Client::setAuthantication(bool flag) {this->_isAuthanticated = flag;}

bool Client::getWelcomeMsg(){return this->_welcomMsgsent;}

void Client::setWelcomeMsg(bool flag) {this->_welcomMsgsent = flag;}

void Client::setNickname(std::string name) {this->_Nickname = name;}
void Client::setUsername(std::string name) {this->_Username = name;}

void Client::setRealname(std::string name) {this->_RealName = name;}

void Client::SendServerToClient(std::string msg)
{
	std::cout << "HEELLLLPPPP " << msg << std::endl;
	this->_ServerToClient += msg + "\r\n";
}


bool Client::isregisterd()
{
	std::cout << getNickname() << " + " << getUsername() << std::endl;
	if (getNickname() != "" && getUsername() != "")
		return true;
	return false;
}
