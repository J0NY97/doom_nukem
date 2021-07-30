/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   better_libui.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/29 12:07:09 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/29 12:55:25 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BETTER_LIBUI_H
# define BETTER_LIBUI_H
# define SDL_MAIN_HANDLED
# define GLEW_STATIC
# include <stdio.h>
# include "SDL.h"
# include "SDL_ttf.h"
# include "SDL_image.h"
# include "libft.h"
# include "libpf.h"
# include <dirent.h>
# include "bui_path.h"

# define DEBUG 0

enum e_types
{
	ELEMENT_DEFAULT = 0,
	ELEMENT_HOVER = 1,
	ELEMENT_CLICK = 2,
	ELEMENT_ALL = 4
};

enum e_element_types
{
	TYPE_MENU,
	TYPE_ELEMENT
};

# define BUI_WINDOW_DONT_UPDATE 0x00000001
# define BUI_WINDOW_UPDATE 0x00000002

# define BUI_ELEMENT_DONT_UPDATE 0x00000001
# define BUI_ELEMENT_UPDATE 0x00000002
# define BUI_ELEMENT_DONT_UPDATE_STATE 0x00000010
# define BUI_ELEMENT_UPDATE_STATE 0x00000020
# define BUI_ELEMENT_DONT_SHOW 0x00000100
# define BUI_ELEMENT_SHOW 0x00000200
# define BUI_ELEMENT_DONT_CLEAR 0x00001000
# define BUI_ELEMENT_CLEAR 0x00002000

typedef struct s_bui_libui			t_bui_libui;
typedef struct s_xywh				t_xywh;
typedef struct s_bui_element		t_bui_element;
typedef struct s_bui_window			t_bui_window;
typedef struct s_rgba				t_rgba;

struct s_rgba
{
	int	a;
	int	r;
	int	g;
	int	b;
};

struct s_xywh
{
	int	x;
	int	y;
	int	w;
	int	h;
};

typedef struct s_text_recipe
{
	char		*text;
	char		*font_name;
	int			font_style;
	int			max_w;
	Uint32		text_color;
	Uint32		font_size;
}				t_text_recipe;
SDL_Surface			*bui_make_text_surface_from_recipe(t_text_recipe *recipe);

/*
 * t_list	*children; list of t_bui_element
*/
struct s_bui_element
{
	SDL_Surface	*active_surface;
	SDL_Surface	*surface[3];
	t_xywh		position;
	t_xywh		screen_pos;
	Uint32		color;
	t_rgba		color_rgba;
	Uint32		color_light;
	Uint32		color_dark;
	void		*parent;
	t_list		*children;
	t_bui_libui	*libui;
	int			state;
	int			already_clicked;
	int			show;
	int			was_clicked_last_frame;
	int			was_hovered_last_frame;
	int			toggle;
	int			type;
	char		*id;
	int			update;
	int			update_state;
	int			clear;
	int			remove;
	char		*text;
	int			text_x;
	int			text_y;
	int			text_w;
	int			text_h;
	Uint32		text_color;
	Uint32		font_size;
	SDL_Surface	*text_surface;
	TTF_Font	*font;
	char		*font_name;
	int			font_style;
};
void				bui_element_print(t_bui_element *elem);

/*
 * t_list	*menu; list of t_bui_element
*/
struct s_bui_window
{
	SDL_Window	*win;
	SDL_Surface	*active_surface;
	t_xywh		position;
	Uint32		color;
	t_bui_libui	*libui;
	t_list		*menus;
	int			update;
	int			free_window;
	int			free_window_surface;
};

/*
 * t_list	*windows; list of t_bui_window
 * t_list	*menus; list of t_bui_element
 * char		*keys; https://wiki.libsdl.org/SDL_Scancode
 * char		*mouse_buttons; https://wiki.libsdl.org/SDL_MouseButtonEvent
*/
struct s_bui_libui
{
	SDL_Event	event;
	char		keys[SDL_NUM_SCANCODES];
	char		mouse_buttons[5];
	char		*last_key;
	int			last_mouse_x;
	int			last_mouse_y;
	int			mouse_x;
	int			mouse_y;
	int			mouse_click_x;
	int			mouse_click_y;
	int			mouse_down;
	int			mouse_down_last_frame;
	int			mouse_drag;
	int			mouse_wheel_x;
	int			mouse_wheel_y;
	int			last_relative_mouse_x;
	int			last_relative_mouse_y;
	int			relative_mouse_x;
	int			relative_mouse_y;
	int			last_global_mouse_x;
	int			last_global_mouse_y;
	int			global_mouse_x;
	int			global_mouse_y;
	int			run;
	t_list		*windows;
	t_list		*elements;
};

typedef struct s_shapes
{
	int			x1;
	int			y1;
	int			x2;
	int			y2;
	int			fill;
	int			size;
	int			color;

}		t_shapes;

typedef struct s_fps
{
	float	curr;
	float	prev;
	int		fps;
	int		count;
}		t_fps;

void				bui_fps_func(t_fps *fps);

// Drawing stuff
void				ft_create_circle(
						SDL_Surface *surface, Uint32 color, t_shapes c);
void				ft_create_line(SDL_Surface *surf, Uint32 color, t_shapes l);

// Libui stuff
t_bui_libui			*bui_new_libui(void);
void				libui_events_clear(t_bui_libui *libui);

// Window stuff
t_bui_window		*bui_new_window(
						t_bui_libui *libui, char *name, t_xywh pos,
						Uint32 flags);
t_bui_window		*bui_new_window_from_window(
						t_bui_libui *libui, SDL_Window *win,
						SDL_Surface *surface);
void				bui_set_window_flags(t_bui_window *win, Uint32 flags);
void				bui_set_window_color(t_bui_window *win, Uint32 color);
void				bui_set_window_icon(t_bui_window *window, char *image_path);

// Render stuff
void				bui_render_to_screen(t_bui_element *elem);

// Event handling
void				bui_event_handler(t_bui_libui *libui);
void				key_was(t_bui_libui *libui, SDL_Keysym key, int type);
void				mouse_key_was(t_bui_libui *libui, Uint8 button, int type);
int					key_pressed(t_bui_libui *libui, int key);
int					key_press(t_bui_libui *libui, char *key);
int					mouse_pressed(t_bui_libui *libui, int key);
void				libui_events_check(t_bui_libui *libui);
int					mouse_moved(t_bui_libui *libui);
int					mouse_wheel(t_bui_libui *libui);
int					mouse_hover(t_bui_libui *libui, t_xywh rect);
int					alpha_pressed(t_bui_libui *libui);
int					number_pressed(t_bui_libui *libui);

// Menubar stuff
t_bui_element		*bui_new_menu(t_bui_window *win, char *name, t_xywh pos);
void				update_element(t_bui_element *elem, SDL_Window *win);

// Element stuff
t_bui_element		*bui_new_element_defaults(
						t_bui_libui *libui, char *text, t_xywh pos);
t_bui_element		*bui_new_element(
						t_bui_element *menu, char *name, t_xywh pos);
void				bui_set_element_state(t_bui_element *elem, int state);
int					bui_button(t_bui_element *button);
int					bui_button_allow_duplicate(t_bui_element *button);
int					bui_button_toggle(t_bui_element *button);
void				bui_set_element_color(t_bui_element *elem, Uint32 color);
void				bui_set_element_list_color(t_list *list, Uint32 color);
void				bui_set_element_border(
						t_bui_element *elem, int thiccness, Uint32 color);
void				bui_set_element_state_border(
						t_bui_element *elem, int thiccness, Uint32 color,
						int state);
// Text
void				bui_set_element_text(t_bui_element *elem, char *text);
void				bui_set_element_text_position(
						t_bui_element *elem, int x, int y);
void				bui_change_element_text(t_bui_element *elem, char *text);
void				bui_set_element_text_color(
						t_bui_element *elem, Uint32 color);
void				bui_set_element_text_font(
						t_bui_element *elem, char *font, Uint32 size,
						Uint32 color);
void				bui_element_set_text_style(t_bui_element *elem, int style);
void				bui_element_set_text_font(
						t_bui_element *elem, char *font_name);
void				bui_set_element_text_size(t_bui_element *elem, Uint32 size);
void				bui_remove_element_text(t_bui_element *elem);
void				bui_center_element_text_x(t_bui_element *elem);
void				bui_center_element_text_y(t_bui_element *elem);
void				bui_center_element_text(t_bui_element *elem);

void				bui_element_update_text(t_bui_element *elem);

int					bui_input(t_bui_element *input);
void				bui_set_element_image_from_path(
						t_bui_element *element, int type, char *path,
						t_xywh *pos);
void				bui_set_element_image(
						t_bui_element *element, int type, SDL_Surface *image,
						t_xywh *pos);
void				bui_set_element_image_all_states_image(
						t_bui_element *element, SDL_Surface *zeroth,
						SDL_Surface *first, SDL_Surface *second);
void				bui_set_element_image_to_states(
						t_bui_element *element, char *zeroth, char *first,
						char *second);
void				bui_element_resize(t_bui_element *element, t_xywh coord);
void				bui_set_element_flags(t_bui_element *elem, Uint32 flags);

// Slider
int					bui_get_slider_value(int min, int max, int pos_x, int w);
int					bui_set_slider_value(int value, int min, int max, int w);

// Text Stuff
SDL_Surface			*bui_new_text(
						char *text, TTF_Font *font, Uint32 font_color,
						int max_w);

// Tab Widgeta majig
t_bui_element		*bui_new_tab(t_list **group, char *name, Uint32 color);
t_bui_element		*bui_tab(t_bui_libui *libui, t_list **group);

// Help Functions
void				set_pixel(SDL_Surface *surface, int x, int y, Uint32 color);
void				fill_surface(SDL_Surface *surface, Uint32 color);
SDL_Surface			*create_surface(int w, int h);
void				clear_surface(SDL_Surface *surface);
t_rgba				hex_to_rgba(Uint32 color_hex);
Uint32				rgba_to_hex(t_rgba rgba);
void				draw_rect(
						SDL_Surface *surface, t_xywh c, Uint32 color, int fill);
void				draw_rect_border(
						SDL_Surface *surface, t_xywh c, Uint32 color,
						unsigned int size);
void				draw_surface_border(
						SDL_Surface *surface, Uint32 color, int thiccness);
int					hitbox_rect(int x, int y, t_xywh rect);
t_xywh				ui_init_coords(int x, int y, int w, int h);
t_xywh				new_xywh(int x, int y, int w, int h);
void				xywh_print(t_xywh c);
t_list				*dir_open(char *folder_path, unsigned char type, int *size);
SDL_Surface			*load_image(char *file);
t_bui_element		*bui_get_element_with_text(t_bui_libui *libui, char *text);
t_bui_element		*bui_get_element_with_text_from_list(
						t_list *list, char *text);

// Testing purposeum
void				dummy_free_er(void *dum, size_t my);
void				bui_render_the_event(t_bui_libui *libui);
void				bui_libui_quit(t_bui_libui *libui);
void				bui_element_free(void *elem, size_t size);
// New testing purposeum
void				bui_event_handler(t_bui_libui *libui);
void				bui_render(t_bui_libui *libui);

/////////////////
// PRESET STUFF
/////////////////
typedef struct s_preset_menu		t_preset_menu;
typedef struct s_preset_console		t_preset_console;
typedef struct s_preset_slider		t_preset_slider;
typedef struct s_preset_scrollbar	t_preset_scrollbar;
typedef struct s_preset_dropdown	t_preset_dropdown;
typedef struct s_preset_tab			t_preset_tab;

struct s_preset_menu
{
	t_bui_element	*menu;
	t_bui_element	*quit;
	t_bui_element	*title_bar;
};

struct s_preset_console
{
	t_preset_menu	*menu;
	t_bui_element	*carrot;
	t_bui_element	*input;
	t_bui_element	*send;
	t_bui_element	*view;
	char			*latest_command;
	unsigned int	lines;
};

struct s_preset_slider
{
	t_bui_element	*slider;
	t_bui_element	*button;
	int				value;
	int				min;
	int				max;
};

struct s_preset_scrollbar
{
	t_bui_element	*scrollbar;
	t_bui_element	*button;
	t_bui_element	*target;
	int				min;
	int				max;
	int				value;
};

/*
 * t_list	*elements; list of t_bui_elements that are inside the bass
*/
struct s_preset_dropdown
{
	t_bui_element	*drop;
	t_bui_element	*arrow;
	t_bui_element	*bass;
	short int		count;
	t_list			*elements;
	t_bui_element	*active;
};

/*
 * NOTE: tab and view has the same index
 * t_list		*tabs; list of t_bui_element
 * t_list		*views; list of t_bui_element
 * short int	tab_count; amount of both tabs and views
*/
struct s_preset_tab
{
	t_bui_element	*tabsystem;
	t_list			*tabs;
	t_list			*views;
	t_bui_element	*active;
	short int		tab_count;
};

// Menu
t_preset_menu		*bui_new_menu_preset(
						t_bui_window *win, char *title, t_xywh pos);
void				preset_menu_events(t_preset_menu *menu);
// Console
t_preset_console	*bui_new_console_preset(t_bui_window *win, t_xywh pos);
void				preset_console_events(t_preset_console *console);
void				console_log(t_preset_console *console, char *text);
// Slider
t_preset_slider		*bui_new_slider_preset(t_bui_element *menu, t_xywh pos);
void				preset_slider_events(t_preset_slider *slider);
void				bui_set_slider_values(
						t_preset_slider *slider, int value, int min, int max);
void				preset_slider_set_button(t_preset_slider *slider);
// Scrollbar
t_preset_scrollbar	*bui_new_scrollbar_preset(
						t_bui_element *menu, t_xywh pos, t_bui_element *target);
void				preset_scrollbar_events(t_preset_scrollbar *scrollbar);
// Drop down
t_preset_dropdown	*bui_new_dropdown_preset(
						t_bui_element *menu, char *text, t_xywh pos);
void				dropdown_preset_free(void *preset, size_t size);
int					preset_dropdown_events(t_preset_dropdown *dropdown);
t_bui_element		*preset_dropdown_add_element(
						t_preset_dropdown *dropdown, char *text);
// Tab
t_preset_tab		*bui_new_tab_preset(
						t_bui_element *menu, char *text, t_xywh pos);
void				tab_preset_free(void *preset, size_t size);
void				preset_tab_events(t_preset_tab *tab);
t_bui_element		**preset_tab_add(t_preset_tab *tab, char *text);

// Other
int					only_one_button_toggled_at_a_time(
						t_list *list, t_bui_element **active);
void				toggle_on_element_with_text(
						t_list *elements, t_bui_element **active, char *text);

#endif
