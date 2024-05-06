#include "../include/Client.hpp"

Client::Client(int fd, int isAuthanticated) : _Username(""), _Nickname(""), _hostName("") ,_ClientFd(fd) , _isAuthanticated(isAuthanticated)
{
}


int Client::getClientFd() {return this->_ClientFd; }

bool Client::getAuthantication() {return this->_isAuthanticated; }

std::string Client::getUsername() { return this->_Username;}

std::string Client::getNickname() { return this->_Nickname;}

std::string Client::getHostname() { return this->_hostName;}
