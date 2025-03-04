To use Irrsi in 42 lab you need Docker to use it

Commands:

PASS {password}		              : First command that should be entered by users who join the IRC server.
NICK {Nickname}	              	: Sets the user's nickname to what they entered. If the username is already taken, then the already set nickname doesn't change.
JOIN {Channel Name} {Password}	: If the Channel Name doesn't exist then a channel is created with the Channel name entered. If the Channel Name doesn't exist and there was a password entered, then a channel is created with the Channel Name and password entered. If the Channel exists and there is a password condition to join the channel, then the user would need to enter the correct password to join the channel. If the Channel exists and there is an invite-only condition, then the user would need to be invited by an operator in the channel to join the channel. Both the password and invite-only conditions could be set, meaning the user would need to meet both those conditions to join the server.
KICK {Nickname}		              : This is to kick a user from a channel, doing this would remove them from a channel despite their status as an operator or not. Can only be used by operators.
INVITE {Nickname}	              : Invites a user to a channel which would allow them to join the channel if it's invite only, function is useless if the channel is not invite only. Can only be used by operators.
TOPIC {New Topic}              	: Changes the topic of a channel, if set mode was set to +T by an operator then only operators can change the topic.

MODE: all of the following commands can only be used by channel operators (caps lock doesn't matter for the type of mode)

  MODE +L {Limiter}	  : Adds a limiter to the amount of people who can join a channel.
	MODE -L			      	: Removes the limiter to join a channel.
	MODE +K {Password}	: Adds a password to a channel.
	MODE -K			       	: Removes channel password.
	MODE +I		      		: Adds an invite only condition to join the channel.
	MODE -I		      		: Removes the invite only condition to join the channel.
	MODE +O {Nickname}	: Finds the user inside a channel and gives them operator privileges.
	MODE -O {Nickname}	: Finds the user inside a channel and removes their operator privileges.
	MODE +T		    	  	: Allows any user in the channel to change the topic.
	MODE -T		      		: Removes permission for all users in the channel to change the topic, can only be changed by channel operators.
