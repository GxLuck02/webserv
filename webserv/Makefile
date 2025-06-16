NAME     := webserv
CC       := c++
CFLAGS   := -Wall -Wextra -Werror -std=c++98 -I$(INC_DIR)

SRC_DIR  := Srcs/
INC_DIR  := Includes/
OBJ_DIR  := obj/

SRCS     := $(wildcard $(SRC_DIR)*.cpp)
OBJS     := $(SRCS:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

# Règle principale
all: $(NAME)

# Lien final
$(NAME): $(OBJS)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Compilation des .cpp en .o dans OBJ_DIR
$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des objets
clean:
	@echo "Cleaning object files..."
	rm -rf $(OBJ_DIR)

# Nettoyage complet
fclean: clean
	@echo "Removing executable..."
	rm -f $(NAME)

# Recompilation complète
re: fclean all

.PHONY: all clean fclean re
