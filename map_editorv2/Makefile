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
	   ./free.c \
	   ../core.c
OBJS = $(SRCS:.c=.o)
INCLUDES = -I ../../libft-windows \
	   -I ../../ft_printf-windows \
	   -I ../../better_libui-windows \
	   -I ../../libgfx-windows \
	   -I ../../include
LIB_FOLDERS = -L ../../libft-windows \
		-L ../../ft_printf-windows \
		-L ../../better_libui-windows \
		-L ../../libgfx-windows \
		-L ../../lib

LIBS = \
	../../libgfx-windows/libgfx.a \
	../../better_libui-windows/better_libui.a \
	../../ft_printf-windows/libprintf.a \
	../../libft-windows/libft.a \
	-l SDL2 -l SDL2_ttf -l SDL2_image
FLAGS = -Wall
LIBFT = ../../libft-windows
BETTER_LIBUI = ../../better_libui-windows
LIBGFX = ../../libgfx-windows
FT_PRINTF = ../../ft_printf-windows

all: $(ALL_LIBS) $(NAME)

$(ALL_LIBS): $(LIBFT) $(BETTER_LIBUI) $(LIBGFX) $(FT_PRINTF)

$(LIBFT):
	@git clone -q https://github.com/J0NY97/libft-windows.git ../../libft-windows

$(BETTER_LIBUI):
	@git clone -q https://github.com/J0NY97/better_libui-windows.git ../../better_libui-windows

$(LIBGFX):
	@git clone -q https://github.com/J0NY97/libgfx-windows.git ../../libgfx-windows

$(FT_PRINTF):
	@git clone -q https://github.com/J0NY97/ft_printf-windows.git ../../ft_printf-windows

$(NAME): $(OBJS)
	@gcc -o $(NAME)	$(SRCS) $(FLAGS) $(INCLUDES) $(LIB_FOLDERS) $(LIBS)
	@echo "$(NAME) was successfully created."

clean:
	@rm -f $(OBJS)
	@echo "$(NAME) was cleaned."

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean, all, re, fclean
