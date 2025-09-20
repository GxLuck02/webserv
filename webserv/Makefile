NAME        := webserv
CC          := c++
INC_DIR     := Includes
CFLAGS      := -Wall -Wextra -Werror -std=c++98 -I$(INC_DIR)

SRC_DIRS    := Srcs Srcs/Clients Srcs/Server Srcs/request Srcs/response Srcs/ConfigParser Srcs/Cgi
OBJ_DIR     := obj

# Trouve tous les fichiers .cpp dans les SRC_DIRS
SRCS        := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))

# Pour chaque .cpp => crée le chemin obj/FICHIER.o correspondant
OBJS        := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(NAME)

# Lien final
$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Removing executable..."
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
