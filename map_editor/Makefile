NAME = doom_editor
SRCS = \
	./main.c \
	./point.c \
	./wall.c \
	./wall_help.c \
	./sector.c \
	./entity.c \
	./sprite.c \
	./entity_preset.c \
	./portal.c \
	./vector.c \
	./grid_adding.c \
	./other.c \
	./wall_edit.c \
	./wall_render.c \
	./wall_sprite_edit.c \
	./sector_edit.c \
	./entity_edit.c \
	./fandc_edit.c \
	./args_parser.c \
	./inits.c \
	./button_functions.c \
	./button_events.c \
	./draw_functions.c \
	./sector_creation.c \
	./select_functions.c \
	./set_map.c \
	./get_map.c \
	./help.c \
	./edit_functions.c \
	./free.c \
	./map_editor.c \
	../libs/libbxpm/byte_utils.c \
	../libs/libbxpm/read_bxpm.c \
	./core.c

OBJS = $(SRCS:.c=.o)

INCLUDES = \
	-I../libs/libft \
	-I../libs/libpf \
	-I../libs/better_libui \
	-I../libs/libgfx \
	-I../libs/SDL2/includes \
	-I../libs/libbxpm \
	-I../game/inc

LIB_FOLDERS = \
	-L../libs/libft \
	-L../libs/libpf \
	-L../libs/better_libui \
	-L../libs/libgfx \
	-L../libs/SDL2/lib

LIB = -lui -lSDL2 -lSDL2_ttf -lSDL2_image -lpf -lft -lgfx

FLAGS = -Wall -Wextra

all: $(NAME)

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
