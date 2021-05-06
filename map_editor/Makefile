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
	   ../core.c
OBJS = $(SRCS:.c=.o)

INCLUDES = \
	-I../libft \
	-I../ft_printf \
	-I../better_libui \
	-I../libgfx \
	-I../SDL2/includes

LIB_FOLDERS = \
	-L../libft \
	-L../ft_printf \
	-L../better_libui \
	-L../libgfx \
	-L../SDL2/lib

LIB = -lSDL2 -lSDL2_ttf -lSDL2_image -llibft.a -llibgfx.a -lbetter_libui.a -llibprintf.a

LIBFT = ../libft
BETTER_LIBUI = ../better_libui
LIBGFX = ../libgfx
FT_PRINTF = ../ft_printf

LIBS = \
	$(LIBGFX)/libgfx.a \
	$(BETTER_LIBUI)/better_libui.a \
	$(FT_PRINTF)/libprintf.a \
	$(LIBFT)/libft.a \
	-lSDL2 -lSDL2_ttf -lSDL2_image

FLAGS = -Wall

all: $(LIBS) $(NAME)

$(LIBS): $(LIBFT) $(BETTER_LIBUI) $(LIBGFX) $(FT_PRINTF)
	make -C ../libft
	make -C ../ft_printf
	make -C ../libgfx
	make -C ../better_libui

$(NAME): $(OBJS)
	@gcc -o $(NAME)	$(INCLUDES) $(LIB_FOLDERS) $(LIBS) $(OBJS) $(FLAGS)
	@echo "$(NAME) was successfully created."

clean:
	@rm -f $(OBJS)
	@make -C $(LIBFT) clean
	@make -C $(LIBGFX) clean
	@make -C $(BETTER_LIBUI) clean
	@make -C $(FT_PRINTF) clean
	@echo "$(NAME) was cleaned."

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT) fclean
	@make -C $(LIBGFX) fclean
	@make -C $(BETTER_LIBUI) fclean
	@make -C $(FT_PRINTF) fclean

re: fclean all
	@make -C $(LIBFT) re
	@make -C $(LIBGFX) re
	@make -C $(BETTER_LIBUI) re
	@make -C $(FT_PRINTF) re

.PHONY: clean, all, re, fclean
