# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/02 08:42:30 by nneronin          #+#    #+#              #
#    Updated: 2021/07/02 08:49:36 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

all:
	@make -C ./better_libui
	@make -C ./ft_printf
	@make -C ./libft
	@make -C ./libgfx
	@make -C ./map_editor
	@make -C ./ui

clean:
	@make clean -C ./better_libui
	@make clean -C ./ft_printf
	@make clean -C ./libft
	@make clean -C ./libgfx
	@make clean -C ./map_editor
	@make clean -C ./ui

fclean:
	@make fclean -C ./better_libui
	@make fclean -C ./ft_printf
	@make fclean -C ./libft
	@make fclean -C ./libgfx
	@make fclean -C ./map_editor
	@make fclean -C ./ui

re: fclean all

.PHONY: clean, all, fclean, re
