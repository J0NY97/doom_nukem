# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/09/23 13:48:12 by nneronin          #+#    #+#              #
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

NAME = libpf.a
LIBFT = libft
IDIR = .
DIR_S = srcs

SOURCES =	pf_printf.c\
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
	   		pf_percent.c\
	   		pf_float.c\
	   		pf_base.c\
	   		pf_nbr_inf.c\
	   		pf_colors.c\
	   		pf_reset.c\
			error_msg.c

SRCS = $(addprefix $(DIR_S)/,$(SOURCES))
OBJS = $(SOURCES:.c=.o)
CFLAGS = -Wall -Wextra -Werror -I $(IDIR) -I ../libft

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

.PHONY: fclean re norme all clean
