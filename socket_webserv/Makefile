NAME= webserv
CC= c++ -Wall -Wextra -Werror -std=c++98
SRCS= $(shell find . -name "*.cpp")
OBJS= $(SRCS:.cpp=.o)

all: $(NAME)
	$(CC) $(SRCS) -o $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re