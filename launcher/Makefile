NAME = wolf3d 
SRCS = \
       ./launcher.c \
       ./get_maps.c \
       ./play_menu.c \
       ./settings_menu.c \
       ./editor_menu.c \
       ./launcher_help.c \
       ./main_menu.c
OBJS = $(SRCS:.c=.o)
INCLUDES = -I../libDoom/libft \
	   -I../libDoom/better_libui \
	   -I../libDoom/libpf \
	   -I../libDoom/libgfx \
	   -I../libDoom/SDL2/includes \
	   -I../ \
	   -I../game/inc
LIBS = -L../libDoom/libft -L../libDoom/better_libui -L../libDoom/libpf -L../libDoom/libgfx -L../libDoom/SDL2/lib \
       -lui -lSDL2 -lSDL2_ttf -lSDL2_image -lpf -lft -lgfx 
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
