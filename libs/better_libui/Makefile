NAME = libui.a
SRCS = \
       ./bui_element.c \
       ./bui_slider.c \
	   ./bui_draw.c \
       ./bui_event_handler.c \
       ./bui_libui.c \
       ./bui_preset.c \
       ./bui_text.c \
       ./bui_window.c \
       ./dir_open.c \
       ./load_image.c \
       ./bui_quit.c \
       ./bui_draw_circle.c \
       ./bui_draw_line.c \
       ./bui_render.c \
       ./bui_events.c \
       ./bui_input.c \
       ./bui_image.c \
       ./bui_set.c \
       ./bui_get.c \
       ./bui_print.c \
       ./bui_edit.c \
	   ./bui_fps.c \
	   ./bui_color.c \
       ./bui_preset_console.c \
       ./bui_preset_dropdown.c \
       ./bui_preset_menu.c \
       ./bui_preset_scrollbar.c \
       ./bui_preset_slider.c \
       ./bui_preset_tab.c \
       ./help.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(SRCS)
#      @make -C ../libpf
#      @make -C ../libft
	@sh bui_path.sh
	@gcc -c $(SRCS) \
		-I../libpf \
		-I../libft \
		-I../SDL2/includes
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)
	@echo "$(NAME) was successfully created."

clean:
	@rm -f $(OBJS)
	@echo "$(NAME) was cleaned."

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: clean, all, re, fclean



