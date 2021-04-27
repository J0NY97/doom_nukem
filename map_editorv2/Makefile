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
INCLUDES = -I../../libft-windows \
	   -I../../ft_printf-windows \
	   -I../../better_libui-windows \
	   -I../../libgfx-windows \
	   -I../../include

LIB_FOLDERS = -L../../libft-windows \
		-L../../ft_printf-windows \
		-L../../better_libui-windows \
		-L../../libgfx-windows \
		-L../../lib

LIBFT = ../../libft-windows
BETTER_LIBUI = ../../better_libui-windows
LIBGFX = ../../libgfx-windows
FT_PRINTF = ../../ft_printf-windows

LIBS = \
	$(LIBGFX)/libgfx.a \
	$(BETTER_LIBUI)/better_libui.a \
	$(FT_PRINTF)/libprintf.a \
	$(LIBFT)/libft.a \
	-lSDL2 -lSDL2_ttf -lSDL2_image

	#-llibgfx \
	-lbetter_libui \
	-llibprintf \
	-llibft \
	-lSDL2 -lSDL2_ttf -lSDL2_image

FLAGS = -Wall

all: $(LIBS) $(NAME)

$(LIBS): $(LIBFT) $(BETTER_LIBUI) $(LIBGFX) $(FT_PRINTF)
	make -C ../../libft-windows
	make -C ../../ft_printf-windows
	make -C ../../libgfx-windows
	make -C ../../better_libui-windows

$(LIBFT):
	git clone https://github.com/J0NY97/libft-windows.git $(LIBFT) 

$(BETTER_LIBUI):
	git clone https://github.com/J0NY97/better_libui-windows.git  $(BETTER_LIBUI)

$(LIBGFX):
	git clone https://github.com/J0NY97/libgfx-windows.git $(LIBGFX)

$(FT_PRINTF):
	git clone https://github.com/J0NY97/ft_printf-windows.git $(FT_PRINTF)

$(NAME): $(OBJS)
	@gcc -o $(NAME)	$(INCLUDES) $(LIB_FOLDERS) $(LIBS) $(SRCS) $(FLAGS)
	@echo "$(NAME) was successfully created."

clean:
	@rm -f $(OBJS)
	@echo "$(NAME) was cleaned."

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean, all, re, fclean
