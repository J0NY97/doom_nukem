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
	   ../libDoom/libbxpm/byte_utils.c \
	   ../libDoom/libbxpm/read_bxpm.c \
	   ./core.c
OBJS = $(SRCS:.c=.o)

INCLUDES = \
	-I../libDoom/libft \
	-I../libDoom/libpf \
	-I../libDoom/better_libui \
	-I../libDoom/libgfx \
	-I../libDoom/SDL2/includes \
	-I../libDoom/libbxpm \
	-I../game/inc

LIB_FOLDERS = \
	-L../libDoom/libft \
	-L../libDoom/libpf \
	-L../libDoom/better_libui \
	-L../libDoom/libgfx \
	-L../libDoom/SDL2/lib

LIB = -lui -lSDL2 -lSDL2_ttf -lSDL2_image -lpf -lft -lgfx 
LIBFT = ../libft
BETTER_LIBUI = ../better_libui
LIBGFX = ../libgfx
FT_PRINTF = ../libDoom/libpf

FLAGS = -Wall -Wextra

all: $(NAME)

LIBS: $(LIBFT) $(BETTER_LIBUI) $(LIBGFX) $(FT_PRINTF)
	@mingw32-make -C ../libDoom/libft
	@mingw32-make -C ../libDoom/libpf
	@mingw32-make -C ../libDoom/libgfx
	@mingw32-make -C ../libDoom/better_libui

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
