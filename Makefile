NAME	= ircserv

SRC		=	srcs/main.cpp			\
			srcs/Client.cpp			\
			srcs/Channel.cpp		\
			srcs/quit.cpp			\
			srcs/user.cpp			\
			srcs/nick.cpp			\
			srcs/privmsg.cpp		\
			srcs/who.cpp			\
			srcs/mode.cpp			\
			srcs/join.cpp			\
			srcs/pass.cpp			\
			srcs/invite.cpp			\
			srcs/kick.cpp			\
			srcs/kill.cpp			\
			srcs/list.cpp			\
			srcs/names.cpp			\
			srcs/oper.cpp			\
			srcs/part.cpp			\
			srcs/ping.cpp			\
			srcs/topic.cpp			\
			srcs/tools.cpp			\


OBJ	= $(SRC:%.cpp=%.o)

CCXX		= c++ 
CXXFLAGS	= -Wall -Wextra -Werror -g -std=c++98

SRCDIR	= ./src/
INCDIR	= ./includes/

all: $(OBJ) $(NAME)

%.o: %.cpp
	$(CCXX) $(CXXFLAGS) -o $@ -c $<

$(NAME): $(OBJ)
	$(CCXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

config:
	@touch .IRCd-config
	@echo "operator_host = 127.0.0.1 localhost\noperator_name = \noperator_password = " > .IRCd-config

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
