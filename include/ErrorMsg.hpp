#ifndef ERRORMSG_HPP
#define ERRORMSG_HPP


#define ERROR_461 " :Not enough parameters"
#define ERROR_462 ":You may not reregister"
#define ERROR_464 ":Password incorrect"


#define ERROR_451 " :You have not registered"
#define ERROR_421 " :Unknown command"

#define RPL_WELCOME "001" 
#define ERROR_002 "002"
#define ERROR_003 "003"
#define ERROR_004 "004"
#define ERROR_005 "005"



#define ERR_NOSUCHNICK "401"  
#define ERR_NOSUCHSERVER "402" 
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_TOOMANYTARGETS "407" 
#define ERR_NORECIPIENT "411" 
#define ERR_NOTEXTTOSEND "412" 
#define ERR_NOTOPLEVEL "413"
#define ERR_WILDTOPLEVEL "414"
#define RPL_AWAY "301"


#define ERR_NEEDMOREPARAMS "461"
#define ERR_NOORIGIN "409"


#define ERR_NONICKNAMEGIVEN "431"

#define RPL_WHOISREGNICK "307" 
#define RPL_WHOISUSER "311" 
#define RPL_ENDOFWHOIS "318"


#define ERROR_431 " :No nickname given"
#define ERROR_432 " :Erroneus nickname"
#define ERROR_433 " :Nickname is already in use"

#define ERROR_475 " :Cannot join channel"



#define ERROR_451 " :You have not registered"
#define ERROR_421 " :Unknown command"
/*
001: RPL_WELCOME

Sent by the server to welcome a client after successful registration.
Indicates that the client has successfully connected to the server.
002: RPL_YOURHOST

Sent by the server to provide information about the server's host and version.
003: RPL_CREATED

Sent by the server to provide the server's creation date or other related information.
004: RPL_MYINFO

Sent by the server to provide information about the server's supported user modes, channel modes, and other related details.
005: RPL_ISUPPORT
*/



#endif 