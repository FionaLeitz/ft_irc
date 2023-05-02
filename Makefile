NAME	= test

SRC		=	test.cpp \

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

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re