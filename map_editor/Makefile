NAME = doom_editor
SRCS = ./main.c \
	   ./args_parser.c \
	   ./inits.c \
	   ./button_functions.c \
	   ./draw_functions.c \
	   ./select_functions.c \
	   ./set_map.c \
	   ./get_map.c \
	   ./help.c \
	   ./edit_functions.c \
	   ./free.c \
	   ./map_editor.c \
	   ../game/lib/libbxpm/byte_utils.c \
	   ../game/lib/libbxpm/read_bxpm.c \
	   ../core.c
OBJS = $(SRCS:.c=.o)

INCLUDES = \
	-I../libft \
	-I../ft_printf \
	-I../better_libui \
	-I../libgfx \
	-I../SDL2/includes \
	-I../game/lib/libbxpm \
	-I../

LIB_FOLDERS = \
	-L../libft \
	-L../ft_printf \
	-L../better_libui \
	-L../libgfx \
	-L../SDL2/lib

LIB = -lui -lSDL2 -lSDL2_ttf -lSDL2_image -lprintf -lft -lgfx 
LIBFT = ../libft
BETTER_LIBUI = ../better_libui
LIBGFX = ../libgfx
FT_PRINTF = ../ft_printf

#remove this.
LIBS = \
	$(LIBGFX)/libgfx.a \
	$(BETTER_LIBUI)/libui.a \
	$(FT_PRINTF)/libprintf.a \
	$(LIBFT)/libft.a \
	-lSDL2 -lSDL2_ttf -lSDL2_image

FLAGS = -Wall -Wextra

#all: $(LIBS) $(NAME)
all: $(NAME)

$(LIBS): $(LIBFT) $(BETTER_LIBUI) $(LIBGFX) $(FT_PRINTF)
	mingw32-make -C ../libft
	mingw32-make -C ../ft_printf
	mingw32-make -C ../libgfx
	mingw32-make -C ../better_libui

$(NAME): $(SRCS)
	@gcc -g -o $(NAME) $(SRCS) $(INCLUDES) $(LIB_FOLDERS) $(LIB) $(FLAGS)
	@echo "$(NAME) was successfully created."

fclean_libs:
	@mingw32-make -C $(LIBFT) fclean
	@mingw32-make -C $(LIBGFX) fclean
	@mingw32-make -C $(BETTER_LIBUI) fclean
	@mingw32-make -C $(FT_PRINTF) fclean

clean:
	@rm -f $(OBJS)
	@make -C $(LIBFT) clean
	@make -C $(LIBGFX) clean
	@make -C $(BETTER_LIBUI) clean
	@make -C $(FT_PRINTF) clean
	@echo "$(NAME) was cleaned."

fclean: fclean_libs
	@rm -f $(NAME)

re: fclean fclean_libs $(LIBS) all

.PHONY: clean, all, re, fclean
