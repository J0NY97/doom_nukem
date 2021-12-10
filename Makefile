# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/26 11:13:50 by nneronin          #+#    #+#              #
#    Updated: 2021/12/09 13:31:28 by nneronin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ORANGE	:= "\x1b[38;5;208m"
RESET	:= "\x1b[0m"

SHELL_NAME	:= $(shell uname -s)

all: 
ifeq ($(SHELL_NAME), Darwin)
	@echo $(ORANGE)"OS: $(SHELL_NAME)\n Making mac version."$(RESET)
	@make -f Makefile-mac
else
	@echo "OS: $(SHELL_NAME)"
	@echo "Making windows version."
	@mingw32-make -f Makefile-win
endif

clean:
ifeq ($(SHELL_NAME), Darwin)
	@make clean -f Makefile-mac
else
	@mingw32-make clean -f Makefile-win 
endif
	
fclean:
ifeq ($(SHELL_NAME), Darwin)
	@make fclean -f Makefile-mac
else
	@mingw32-make fclean -f Makefile-win 
endif

re: fclean all

.PHONY: clean, all, re, fclean, framework, framework_del, frameworks_re
