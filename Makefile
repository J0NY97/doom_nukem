# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/26 11:13:50 by nneronin          #+#    #+#              #
#    Updated: 2021/07/26 13:44:31 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL_NAME	:= $(shell uname -s)

RED			:=	"\e[0;31m"
GREEN		:=	"\x1b[38;5;119m"
ORANGE		:=	"\x1b[38;5;208m"
YELLOW		:=	"\e[0;33m"
BLUE		:=	"\e[0;34m"
MAGENTA		:=	"\e[0;35m"
CYAN		:=	"\e[0;36m"
UNDERLINE	:=	"\x1b[4m"
RESET		:=	"\e[0m"

name = dontcare

all: framework  
	@make -C ./libs/libft
	@make -C ./libs/libpf
	@make -C ./libs/libgfx
	@make -C ./libs/libbxpm
ifeq ($(SHELL_NAME), Darwin)
	@make -C ./libs/libtp
endif
	@make -C ./libs/better_libui
	@make -C ./map_editor
	@make -C ./launcher
	@make -C ./game
	@mv ./launcher/wolf3d .

clean:
	@make clean -C ./libs/libft
	@make clean -C ./libs/libpf
	@make clean -C ./libs/libgfx
	@make clean -C ./libs/libbxpm
ifeq ($(SHELL_NAME), Darwin)
	@make clean -C ./libs/libtp
endif
	@make clean -C ./libs/better_libui
	@make clean -C ./map_editor
	@make clean -C ./launcher
	@make clean -C ./game
	
fclean:
	@make fclean -C ./libs/libft
	@make fclean -C ./libs/libpf
	@make fclean -C ./libs/libgfx
	@make fclean -C ./libs/libbxpm
ifeq ($(SHELL_NAME), Darwin)
	@make fclean -C ./libs/libtp
endif
	@make fclean -C ./libs/better_libui
	@make fclean -C ./map_editor
	@make fclean -C ./launcher
	@make fclean -C ./game

re: fclean all

SDL_DIR		:= ./libs/SDL_Frameworks

framework:
ifeq ($(SHELL_NAME), Darwin)
	@mkdir -p ~/Library/Frameworks
ifeq ("$(wildcard ~/Library/Frameworks/SDL2*.framework)","")
	@cp -Rf $(SDL_DIR)/SDL2.framework ~/Library/Frameworks/
	@cp -Rf $(SDL_DIR)/SDL2_ttf.framework ~/Library/Frameworks/
	@cp -Rf $(SDL_DIR)/SDL2_image.framework ~/Library/Frameworks/
	@cp -Rf $(SDL_DIR)/SDL2_mixer.framework ~/Library/Frameworks/
	@printf $(CYAN)"[INFO]	Moving Frameworks\n"$(RESET)
else
	@printf $(CYAN)"[INFO]	Frameworks Exists\n"$(RESET)
endif
endif

framework_del:
	@rm -rf ~/Library/Frameworks/SDL2*.framework
	@/bin/rm -f $(NAME)
	@printf $(CYAN)"[INFO]	Deleted SDL2 Frameworks from ~/Library/Frameworks\n"$(RESET)

framework_re: framework_del
	@cp -Rf $(SDL_DIR)/SDL2.framework ~/Library/Frameworks/
	@cp -Rf $(SDL_DIR)/SDL2_ttf.framework ~/Library/Frameworks/
	@cp -Rf $(SDL_DIR)/SDL2_image.framework ~/Library/Frameworks/
	@cp -Rf $(SDL_DIR)/SDL2_mixer.framework ~/Library/Frameworks/
	@printf $(CYAN)"[INFO]	Mooving SDL2 Frameworks to ~/Library/Frameworks\n"$(RESET)

.PHONY: clean, all, re, fclean, framework, framework_del, frameworks_re
