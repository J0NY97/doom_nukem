# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/10 10:44:15 by nneronin          #+#    #+#              #
#    Updated: 2021/08/01 17:12:18 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED				:= "\e[0;31m"
GREEN			:= "\e[0;32m"
YELLOW			:= "\e[0;33m"
BLUE			:= "\e[0;34m"
MAGENTA			:= "\e[0;35m"
CYAN			:= "\e[0;36m"
RESET			:= "\e[0m"

IDIR 	=	.
NAME	=	libbxpm.a
SRCS	=	read_bmp.c \
			read_bxpm.c \
			surf_to_bmp.c \
			surf_to_bxpm.c \
			bmp_to_bxpm.c \
			write_bmp.c \
			write_bxpm.c \
			byte_utils.c \
			free.c

OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -Wextra -Werror -Wunused-variable -Wno-unused-result
CFLAGS += -I $(IDIR) -I ../libft -I ../libpf 

all: $(NAME)
	@printf $(CYAN)"[INFO]	$(NAME) is up to date!\n"$(RESET)

$(NAME): $(SRCS)
	@gcc -c $(SRCS) $(CFLAGS)
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)
	@rm -f $(OBJS)
	@printf $(GREEN)"Compiling $(NAME)\n"$(RESET)

clean:
	@rm -f $(OBJS)
	@printf $(CYAN)"[INFO]	$(NAME) cleaned\n"$(RESET)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean, all, fclean, re
