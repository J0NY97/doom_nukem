NAME = wolf3d
SRCS = \
       ./launcher.c \
       ./get_maps.c \
       ./play_menu.c \
       ./play_menu2.c \
       ./settings_menu.c \
       ./settings_menu2.c \
       ./editor_menu.c \
       ./launcher_help.c \
       ./dropdown.c \
	   ./free.c \
       ./main_menu.c
OBJS = $(SRCS:.c=.o)
INCLUDES = -I../libs/libft \
	   -I../libs/better_libui \
	   -I../libs/libpf \
	   -I../libs/libgfx \
	   -I../libs/SDL2/includes \
	   -I../ \
	   -I../game/inc
LIBS = -L../libs/libft -L../libs/better_libui -L../libs/libpf -L../libs/libgfx -L../libs/SDL2/lib \
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
