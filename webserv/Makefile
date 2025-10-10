NAME        = webserv
CC          = c++
INC_DIR     = Includes
RM			= rm -f
CFLAGS      = -Wall -Wextra -Werror -std=c++98 -I$(INC_DIR)

DIRS_SRCS	= Srcs
DIRS_CLIENTS = $(DIRS_SRCS)/Clients
DIRS_SERVER  = $(DIRS_SRCS)/Server
DIRS_REQUEST = $(DIRS_SRCS)/request
DIRS_RESPONSE= $(DIRS_SRCS)/response
DIRS_CONFIG  = $(DIRS_SRCS)/ConfigParser
DIRS_CGI     = $(DIRS_SRCS)/Cgi


OBJ_DIR     = obj

# Trouve tous les fichiers .cpp dans les SRC_DIRS
SRCS        = 	$(DIRS_SRCS)/main.cpp $(DIRS_CLIENTS)/Client.cpp $(DIRS_CGI)/ParserCgiResponse.cpp $(DIRS_CGI)/RecieveCgi.cpp $(DIRS_CONFIG)/Parser.cpp \
			   	$(DIRS_REQUEST)/DeleteRequest.cpp $(DIRS_REQUEST)/FillBodyInCreatedFile.cpp $(DIRS_REQUEST)/GetRequest.cpp $(DIRS_REQUEST)/ParseJpeg.cpp \
			   	$(DIRS_REQUEST)/ParseMultipartFormData.cpp $(DIRS_REQUEST)/ParseRequest.cpp $(DIRS_REQUEST)/RecieveRequest.cpp $(DIRS_REQUEST)/Request.cpp \
			 	$(DIRS_RESPONSE)/Response.cpp  $(DIRS_RESPONSE)/SendResponse.cpp  $(DIRS_RESPONSE)/StatusCode.cpp $(DIRS_SERVER)/Server.cpp $(DIRS_SERVER)/Server_configue.cpp


OBJS = ${SRCS:.cpp=.o}

INCLUDE = -I include

.cpp.o:
		${CC} ${CFLAGS} ${INCLUDE} -c $< -o ${<:.cpp=.o}

$(NAME): ${OBJS}
		${CC} ${CFLAGS} ${INCLUDE} ${OBJS} -o ${NAME}

all:	${NAME}

clean:
		${RM} ${OBJS}

fclean:	clean
		${RM} ${NAME}

re: clean all

.PHONY: all clean re
