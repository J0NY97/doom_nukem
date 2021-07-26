#ifndef MAIN_MENU_H
# define MAIN_MENU_H

# include "better_libui.h"
# include "libft.h"
# include "libpf.h"
# include "libgfx.h"
# include "SDL.h"
# include "SDL_ttf.h"
# include "SDL_image.h"
# include "path.h"

typedef struct		s_map_button_data
{
	t_list			**list;
	t_bui_element	*parent;
	char			*str;
	t_xywh			cap;
	int				i;
}					t_map_button_data;

typedef struct	s_ui_menu_settings
{
	t_bui_element *menu;
	
	t_bui_element *fov;
	t_bui_element *mouse_x;
	t_bui_element *mouse_y;
	t_bui_element *developer_mode;
	t_bui_element *texture_scale;
	t_bui_element *resolution;

	t_preset_slider *fov_slider;
	t_preset_slider *mouse_x_slider;
	t_preset_slider *mouse_y_slider;
	t_bui_element *developer_mode_toggle;
	t_preset_slider *texture_scale_slider;
	t_preset_dropdown *resolution_drop;
}		t_ui_menu_settings;

typedef struct	s_ui_menu_play
{
	t_bui_element *menu;

	t_preset_tab *play_tabs;

	t_bui_element *story_button;
	t_bui_element *story_view;
	t_bui_element *endless_button;
	t_bui_element *endless_view;

	// drop
	t_bui_element		*difficulty_text_elem;
	t_preset_dropdown	*difficulty_dropdown;

	t_bui_element *endless_maps_view;

	t_list *story_maps; // list of t_bui_element * , all the maps that end with .story
	t_list *endless_maps; // list of t_bui_element * , all the maps that end with .endless
}		t_ui_menu_play;

typedef struct	s_ui_menu_editor
{
	t_bui_element *menu;
	t_list *map_buttons; // list of t_bui_element * all buttons for the maps.
}		t_ui_menu_editor;

typedef struct	s_launcher
{
	t_fps	*fps;
	t_bui_element *menu;

	t_ui_menu_settings *settings_menu;
	t_bui_element *settings_button;

	t_ui_menu_play *play_menu;
	t_bui_element *play_button;

	t_bui_element *quit_button;

	t_ui_menu_editor *editor_menu;
	t_bui_element *editor_button;

	t_list		*main_menu_buttons; // list of t_bui_element
	t_bui_element	*active_button;

	// Colors
	Uint32	gray_blue;
	Uint32	greenish;

	// All maps
	char	*map_dir;
	t_list	*map_list; // list of char *
}		t_launcher;

// free
void			free_launcher(t_launcher *launcher);

// init
t_launcher		*launcher_init(t_bui_window *win);
void			main_menu_events(t_launcher *main_menu, t_bui_libui *libui);
t_list			*get_maps(char *directoy);

// Editor //
t_ui_menu_editor	*editor_menu_init(t_bui_element *main_menu);
void			init_editor_menu_map_buttons(t_ui_menu_editor *menu, t_list *maps);
void			editor_menu_events(t_launcher *launcher, t_ui_menu_editor *menu);

// Play //
void			init_play_menu_map_buttons(t_ui_menu_play *menu, t_list *maps);
t_ui_menu_play		*play_menu_init(t_bui_element *parent_elem);
void			play_menu_events(t_launcher *launcher, t_ui_menu_play *play_menu, t_ui_menu_settings *settings);

// Settings //
t_ui_menu_settings	*settings_menu_init(t_bui_element *parent_elem);
void			settings_menu_events(t_ui_menu_settings *settings_menu);

// HELP //
t_bui_element		*new_map_button(t_bui_element *parent, char *text, int i, t_xywh c);
t_bui_element		*new_main_menu_menu(t_bui_element *parent, char *text);
t_bui_element		*new_main_menu_button(t_launcher *menu, char *text, int i);
t_bui_element		*new_bass_element(t_preset_dropdown *drop, char *text);
char			*get_clicked_map_button(t_list *map_buttons);
t_xywh			get_next_pos(t_xywh start_pos, int i);

#endif
