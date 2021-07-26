NAME = menu
SRCS = \
       ./launcher.c \
       ./get_maps.c \
       ./play_menu.c \
       ./settings_menu.c \
       ./editor_menu.c \
       ./launcher_help.c \
       ./main_menu.c
OBJS = $(SRCS:.c=.o)
INCLUDES = -I../libft \
	   -I../better_libui \
	   -I../ft_printf \
	   -I../libgfx \
	   -I../SDL2/includes \
	   -I../ \
	   -I../game/inc
LIBS = -L../libft -L../better_libui -L../ft_printf -L../libgfx -L../SDL2/lib \
       -lui -lSDL2 -lSDL2_ttf -lSDL2_image -lprintf -lft -lgfx 
FLAGS = -Wall -Wextra
# -Werror

all: $(NAME)

$(NAME): $(SRCS)
	@gcc -o $(NAME) $(SRCS) $(FLAGS) $(INCLUDES) $(LIBS)
	@echo "$(NAME) was successfully created."

clean:
	@rm -f $(OBJS)
	@echo "$(NAME) was cleaned."

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean, all, re, fclean
