#include "../../include/IRC.hpp"

IRC::Mode::Mode(){}

IRC::Mode::~Mode(){}

void IRC::Mode::excuteMode(Parse *parse, Client* client, Server* server)
{
	(void)client;
	(void)server;
    (void)parse;

}




//  i: Set/remove Invite-only channel
// · t: Set/remove the restrictions of the TOPIC command to channel
// operators
// · k: Set/remove the channel key (password)
// · o: Give/take channel operator privilege
// 5
// · l: Set/remove the user limit to channel