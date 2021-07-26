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
	   ../game/libbxpm/byte_utils.c \
	   ../game/libbxpm/read_bxpm.c \
	   ../core.c
OBJS = $(SRCS:.c=.o)

INCLUDES = \
	-I../libft \
	-I../ft_printf \
	-I../better_libui \
	-I../libgfx \
	-I../SDL2/includes \
	-I../game/libbxpm \
	-I../game/inc \
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

FLAGS = -Wall -Wextra

all: $(NAME)

LIBS: $(LIBFT) $(BETTER_LIBUI) $(LIBGFX) $(FT_PRINTF)
	@mingw32-make -C ../libft
	@mingw32-make -C ../ft_printf
	@mingw32-make -C ../libgfx
	@mingw32-make -C ../better_libui

$(NAME): $(SRCS)
	@gcc -o $(NAME) $(SRCS) $(INCLUDES) $(LIB_FOLDERS) $(LIB) $(FLAGS)
	@echo "$(NAME) was successfully created."

clean:
	@rm -f $(OBJS)
	@echo "$(NAME) was cleaned."

fclean:
	@rm -f $(NAME)
	@echo "$(NAME) was fcleaned."

re: fclean $(LIBS) all

.PHONY: clean, all, re, fclean
