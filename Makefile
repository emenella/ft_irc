# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: emenella <emenella@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/14 11:09:51 by bmangin           #+#    #+#              #
#    Updated: 2022/09/19 18:26:50 by emenella         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

override PATH_B		:= .bin
override PATH_I		:= includes
override PATH_S		:= srcs
override PATH_IRC	:= irc
override PATH_SOCK	:= socket
override PATH_U		:= client
override PATH_C		:= ${PATH_U}/command

override VPATH		:= ${addprefix ${PATH_S}/, ${PATH_IRC}} \
					${addprefix ${PATH_S}/, ${PATH_SOCK}} \
					${addprefix ${PATH_S}/, ${PATH_U}} \
					${addprefix ${PATH_S}/, ${PATH_C}} \
					${PATH_S}

NAME := ircserv

FILES_IRC			:= Server Channel
FILES_SOCK			:= Socket SocketConnection SocketListener SocketServer
FILES_USER			:= Client ACommand AuthenticationCommand
FILES_COMMAND		:= NICK PASS JOIN USER PING PART NAMES TOPIC PRIVMSG LIST QUIT MODE INVITE NOTICE KICK

FILES				= ${addprefix ${PATH_IRC}/, ${FILES_IRC}} \
					${addprefix ${PATH_SOCK}/, ${FILES_SOCK}} \
					${addprefix ${PATH_U}/, ${FILES_USER}} \
					${addprefix ${PATH_C}/, ${FILES_COMMAND}}

SRCS				= ${addprefix ${PATH_S}/, ${addsuffix .cpp, ${FILES}}} main.cpp
HEADER				:= ${addprefix ${PATH_I}/, ${addsuffix .hpp, ${FILES}}} includes/client/REPLY.hpp
OBJS				:= ${addprefix ${PATH_B}/, ${notdir ${SRCS:.cpp=.o}}}

INC		:= -I${PATH_I}
CC		:= g++
FLAG	:= -Wall -Werror -Wextra
CPP_V	:= -std=c++98
CCF		:= ${CC} ${FLAG} ${CPP_V} ${INC}
CCFS	:= ${CC} ${FLAG} ${CPP_V} ${FS} ${INC} ${DEB}
CCD		:= ${CC} ${FLAG} ${CPP_V} ${INC} ${DEB}
RM		:= rm -rf

all: bin $(NAME)

bin:
	mkdir -p ${PATH_B}

$(NAME): $(OBJS)
	$(CCF) -o $(NAME) $(OBJS)
	
${PATH_B}/%.o: %.cpp ${HEADER}
	$(CCF) -o $@ -c $<

clean:
	${RM} $(OBJS) 

fclean:
	${RM} $(NAME) $(OBJS)

re: fclean all