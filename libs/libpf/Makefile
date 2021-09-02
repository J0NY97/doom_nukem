# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/23 13:48:12 by nneronin          #+#    #+#              #
#    Updated: 2021/09/02 10:56:42 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

RED		:= "\e[0;31m"
GREEN	:= "\e[0;32m"
YELLOW	:= "\e[0;33m"
BLUE	:= "\e[0;34m"
MAGENTA	:= "\e[0;35m"
CYAN	:= "\e[0;36m"
RESET	:= "\e[0m"

RAW =	pf_printf.c\
	   	pf_format.c\
	   	pf_formats1.c\
	   	pf_formats2.c\
	   	pf_padding.c\
	   	pf_flags.c\
	   	pf_buffer.c\
	   	pf_nbr.c\
	   	pf_str.c\
	   	pf_char.c\
	   	pf_nbr_u.c\
	   	pf_nbr_o.c\
	   	pf_nbr_x.c\
		pf_nbr_p.c\
	   	pf_percent.c\
	   	pf_float.c\
	   	pf_base.c\
	   	pf_nbr_inf.c\
	   	pf_reset.c\
		pf_specials.c\
		error_msg.c

NAME = libpf.a
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
	@gcc -c $< -o $@ $(CFLAGS) -I . -I ../libft
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
