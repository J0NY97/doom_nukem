# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/11/09 10:44:31 by nneronin          #+#    #+#              #
#    Updated: 2021/09/03 13:02:51 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED		:= "\e[0;31m"
GREEN	:= "\e[0;32m"
YELLOW	:= "\e[0;33m"
BLUE	:= "\e[0;34m"
MAGENTA	:= "\e[0;35m"
CYAN	:= "\e[0;36m"
RESET	:= "\e[0m"

RAW	=	init_tpool.c\
	  	tpool_free.c\
		tpool.c\
		tpool_error.c

NAME = libtp.a
IDIR = .
ODIR = objs
CDIR = srcs
SRCS = $(addprefix $(CDIR)/, $(RAW))
OBJS = $(addprefix $(ODIR)/, $(RAW:.c=.o))
CFLAGS = -Wall -Wextra -Werror -MMD -O3
BIN_DIR = .

all: $(BIN_DIR)/$(NAME)
	@printf ${GREEN}"> $(NAME) ready!\n"${RESET}

$(ODIR):
	@mkdir -p $(ODIR)
	@printf "\n"

$(ODIR)/%.o: $(CDIR)/%.c
	@gcc -c $< -o $@ $(CFLAGS) -I .
	@printf $(YELLOW)"\e[F\e[JCompiling $<\n"$(RESET)

$(BIN_DIR)/$(NAME): $(ODIR) $(OBJS)
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)
	@printf ${CYAN}"\e[F\e[J[INFO] Linked $(NAME) with success.\n"${RESET}

clean: 
	@printf ${CYAN}"[INFO] Removing objs from the libft.\n"${RESET}
	@rm -rf $(ODIR)

fclean: clean
	@printf ${CYAN}"[INFO] Removing $(NAME) from the libft.\n"${RESET}
	@rm $(NAME)

re: fclean all

.PHONY: fclean all clean libft