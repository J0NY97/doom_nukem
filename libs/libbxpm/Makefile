# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/10 10:44:15 by nneronin          #+#    #+#              #
#    Updated: 2021/09/03 13:05:51 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED		:= "\e[0;31m"
GREEN	:= "\e[0;32m"
YELLOW	:= "\e[0;33m"
BLUE	:= "\e[0;34m"
MAGENTA	:= "\e[0;35m"
CYAN	:= "\e[0;36m"
RESET	:= "\e[0m"

RAW	=	read_bmp.c \
		read_bxpm.c \
		surf_to_bmp.c \
		surf_to_bxpm.c \
		bmp_to_bxpm.c \
		write_bmp.c \
		write_bxpm.c \
		byte_utils.c \
		free.c

NAME = libbxpm.a
IDIR = .
ODIR = objs
CDIR = srcs
SRCS = $(addprefix $(CDIR)/, $(RAW))
OBJS = $(addprefix $(ODIR)/, $(RAW:.c=.o))
CFLAGS = -Wall -Wextra -Werror -MMD -O3

all: $(NAME)
	@printf ${GREEN}"> $(NAME) ready!\n"${RESET}

$(ODIR):
	@mkdir -p $(ODIR)
	@printf "\n"

$(ODIR)/%.o: $(CDIR)/%.c
	@gcc -c $< -o $@ $(CFLAGS) -I . -I ../libft -I ../libpf
	@printf $(YELLOW)"\e[F\e[JCompiling $<\n"$(RESET)

$(NAME): $(ODIR) $(OBJS)
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)
	@printf ${CYAN}"\e[F\e[J[INFO] Linked $(NAME) with success.\n"${RESET}

clean: 
	@printf ${CYAN}"[INFO] Removing $(NAME) object files.\n"${RESET}
	@rm -rf $(ODIR)

fclean: clean
	@printf ${CYAN}"[INFO] Removing $(NAME)\n"${RESET}
	@rm $(NAME)

re: fclean all

.PHONY: fclean all clean libft
