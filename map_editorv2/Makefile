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
INCLUDES = -I ../../libft \
	   -I ../../ft_printf \
	   -I ../../better_libui \
	   -I ../../libgfx \
	   -I ../../include/SDL2
LIB_FOLDERS = -L ../../libft \
		-L ../../ft_printf \
		-L ../../better_libui \
		-L ../../libgfx \
		-L ../../lib

LIBS = \
	-l libgfx \
	-l better_libui \
	-l libprintf \
	-l libft \
	-l SDL2 -l SDL2_ttf -l SDL2_image
FLAGS = -Wall

all: $(NAME)

$(NAME): $(SRCS)
	@gcc -o $(NAME)	$(SRCS) $(FLAGS) $(INCLUDES) $(LIB_FOLDERS) $(LIBS)
	@echo "$(NAME) was successfully created."

clean:
	@rm -f $(OBJS)
	@echo "$(NAME) was cleaned."

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean, all, re, fclean
