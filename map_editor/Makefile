NAME = doom_editor
SRCS = ./main.c \
	   ./args_parser.c \
	   ./inits.c \
	   ./button_functions.c \
	   ./draw_functions.c \
	   ./select_functions.c \
	   ./set_map.c \
	   ./get_map.c \
	   ../texture.c \
	   ./help.c \
	   ./edit_functions.c \
	   ../core.c
OBJS = $(SRCS:.c=.o)
INCLUDES = -I ../libft -L ../libft -I ../libui -L ../libui -I ../ft_printf -L ../ft_printf -I ../libgfx -L ../libgfx -I ../ -L ../
LIBS = -l libft -l libprintf -l libui -l libgfx 
FLAGS = -Wall
#-Wextra -Werror

all: $(NAME)

$(NAME): $(SRCS)
	@gcc -o $(NAME) $(SRCS) $(FLAGS) -I ../ -L ../ -I ../libgfx -L ../libgfx -l libgfx -I ../libui -L ../libui -l libui -I ../ft_printf -L ../ft_printf -l libprintf -I ../libft -L ../libft -l libft -I ../include/SDL2 -L ../lib -l SDL2 -l SDL2_ttf -l SDL2_image
	@echo "$(NAME) was successfully created."

clean:
	@rm -f $(OBJS)
	@echo "$(NAME) was cleaned."

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean, all, re, fclean
