Name = IRC

FILES = src/main.cpp \
		src/Parse.cpp \
		src/Server.cpp \
		src/Client.cpp \
		src/Channel.cpp	\
		src/commands/Commands.cpp \
		src/commands/Pass.cpp \
		src/commands/Join.cpp \
		src/commands/Nick.cpp \
		src/commands/User.cpp \
		src/commands/Whois.cpp \
		src/commands/Mode.cpp \
		src/commands/Ping.cpp \
		src/commands/Cap.cpp \
		src/commands/Quit.cpp \
		src/commands/Privmsg.cpp \
		src/commands/Topic.cpp	\
		src/commands/Kick.cpp	\
		src/commands/Invite.cpp	\





CXXFLAGS = -Wall -Wextra -Werror -std=c++98

CXX = c++

OBJ = $(FILES:.cpp=.o)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(Name): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(Name)

all: $(Name)

clean:
	rm -rf $(OBJ)
	@echo "$(RE)Object files removed!$(RC)"

fclean: clean
	rm -rf $(Name)
	@echo "$(RE)Removing $(Name) and .o files$(RC)"

re: fclean all

.PHONY: all clean fclean re

# Colors
BA = \033[0;30m
RE = \033[0;31m
GR = \033[0;32m
YE = \033[0;33m
BL = \033[0;34m
PU = \033[0;35m
CY = \033[0;36m
WH = \033[0;37m
RC = \033[0m