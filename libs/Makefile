# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/26 09:01:18 by nneronin          #+#    #+#              #
#    Updated: 2021/07/26 09:02:37 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all:
	@make -C ./libft
	@make -C ./libpf
	@make -C ./libgfx
	@make -C ./libbxpm
	@make -C ./better_libui

clean:
	@make clean -C ./libft
	@make clean -C ./libpf
	@make clean -C ./libgfx
	@make clean -C ./libbxpm
	@make clean -C ./better_libui

fclean:
	@make fclean -C ./libft
	@make fclean -C ./libpf
	@make fclean -C ./libgfx
	@make fclean -C ./libbxpm
	@make fclean -C ./better_libui

re: fclean all

.PHONY: clean, all, fclean, re
