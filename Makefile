# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: brendos- <brendos-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/08 15:40:21 by brendos-          #+#    #+#              #
#    Updated: 2025/11/09 13:55:41 by brendos-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g

SRC = client.c server.c utils.c
CLIENT = client
SERVER = server

.PHONY: all clean fclean re

all: $(CLIENT) $(SERVER)

client.o: client.c utils.c
	$(CC) $(CFLAGS) -c client.c -o client.o

server.o: server.c utils.c
	$(CC) $(CFLAGS) $(SERVER_DEFS) -c server.c -o server.o

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c -o utils.o

$(CLIENT): client.o utils.o
	$(CC) $(CFLAGS) client.o utils.o -o client

$(SERVER): server.o utils.o
	$(CC) $(CFLAGS) server.o utils.o $(SERVER_DEFS) -o server

clean:
	rm -f *.o

fclean: clean
	rm -f client server

re: fclean all