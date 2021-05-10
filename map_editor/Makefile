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

LIB = -lSDL2 -lSDL2_ttf -lSDL2_image -lft -lgfx -lui -lprintf

LIBFT = ../libft
BETTER_LIBUI = ../better_libui
LIBGFX = ../libgfx
FT_PRINTF = ../ft_printf

LIBS = \
	$(LIBGFX)/libgfx.a \
	$(BETTER_LIBUI)/libui.a \
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

$(NAME): $(SRCS)
	@gcc -o $(NAME)	$(INCLUDES) $(LIB_FOLDERS) $(LIB) $(SRCS) $(FLAGS)
	@echo "$(NAME) was successfully created."

fclean_libs:
	@make -C $(LIBFT) fclean 
	@make -C $(LIBGFX) fclean
	@make -C $(BETTER_LIBUI) fclean
	@make -C $(FT_PRINTF) fclean

clean:
	@rm -f $(OBJS)
	@make -C $(LIBFT) clean
	@make -C $(LIBGFX) clean
	@make -C $(BETTER_LIBUI) clean
	@make -C $(FT_PRINTF) clean
	@echo "$(NAME) was cleaned."

fclean: fclean_libs clean
	@rm -f $(NAME)

re: fclean fclean_libs all
	
.PHONY: clean, all, re, fclean
