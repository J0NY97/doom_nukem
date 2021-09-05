# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/10 10:44:15 by nneronin          #+#    #+#              #
#    Updated: 2021/09/04 12:07:44 by nneronin         ###   ########.fr        #
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
		write_bxpm2.c \
		byte_utils.c \
		free.c
 
NAME = libbxpm.a
CDIR = srcs
IDIR = -I. -I ../libft -I ../libpf
ODIR = objs
SRCS = $(addprefix $(CDIR)/, $(RAW))
OBJS = $(addprefix $(ODIR)/, $(RAW:.c=.o))
CFLAGS = -Wall -Wextra -Werror -MMD -O3

all: $(NAME)
	@printf ${GREEN}"\e[J> $(NAME) ready!\n"${RESET}

$(ODIR):
	@mkdir -p $(ODIR)

$(ODIR)/%.o: $(CDIR)/%.c
	@printf $(YELLOW)"\e[JCompiling $<\n\e[F"$(RESET)
	@gcc -c $< -o $@ $(CFLAGS) $(IDIR)

$(NAME): $(ODIR) $(OBJS)
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)

clean: 
	@printf ${CYAN}"[INFO] Removing objs.\n"${RESET}
	@rm -rf $(ODIR)

fclean: clean
	@printf ${CYAN}"[INFO] Removing $(NAME)\n"${RESET}
	@rm $(NAME)

re: fclean all

.PHONY: fclean all clean re $(NAME)
