# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/09 10:44:31 by nneronin          #+#    #+#              #
#    Updated: 2021/05/08 13:56:14 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED				:= "\e[0;31m"
GREEN			:= "\e[0;32m"
YELLOW			:= "\e[0;33m"
BLUE			:= "\e[0;34m"
MAGENTA			:= "\e[0;35m"
CYAN			:= "\e[0;36m"
RESET			:= "\e[0m"

NAME	= libtp.a
FLAGS	= -Wall -Wextra -Werror
SRCS	=	init_tpool.c\
	  	tpool_free.c\
		tpool.c\
		tpool_error.c\

OBJS = $(SRCS:.c=.o)

all: $(NAME)
	@printf $(CYAN)"[INFO]	$(NAME) ready!\n"$(RESET)

$(NAME): $(SRCS)
	@gcc -c $(SRCS) -pthread
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)
	@/bin/rm -f $(OBJS)
	@printf $(GREEN)"Compiling $(NAME)\n"$(RESET)

clean:
	@/bin/rm -f $(OBJS)
	@printf $(CYAN)"[INFO]	$(NAME) cleaned\n"$(RESET)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY: clean, all, fclean, re
