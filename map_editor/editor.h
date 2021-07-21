#ifndef EDITOR_H
# define EDITOR_H
# define SDL_MAIN_HANDLED

# include "core.h"
# include "libft.h"
# include "better_libui.h"
# include "libgfx.h"
# include "ft_printf.h"
# include "path.h"
# include "bxpm.h"
# include "enum.h"
# include "resources.h"

enum e_entity
{
	ENTITY_TYPE_HOSTILE,
	ENTITY_TYPE_FRIENDLY,
	ENTITY_TYPE_NEUTRAL
};

typedef	struct	s_editor			t_editor;

typedef struct	s_color_palette
{
	int			win;
	int			win_elem;
	int			elem_elem;
	int			elem_elem_elem;
	int			light_gray;
	int			granny_smith_apple;
	int			peach_crayola;
	int			light_blue;
}				t_color_palette;

typedef	struct	s_spawn
{
	t_vector	pos;
	int			direction; // degrees
}				t_spawn;

typedef	struct	s_grid
{
	t_bui_element	*elem; // the actual element where you draw everything.

	t_vector		hover; // this is calculating from the coordinate system used in this program.
	t_vector		last_hover; // last frame grid->hover

	t_xywh			coords;
	int				x;
	int				y;
	int				gap;
	t_vector		selected1;
	t_vector		selected2;

// these are the information for when you want to edit
	t_point			*modify_point;
	t_wall			*modify_wall;
	t_sector		*modify_sector;
	t_entity		*modify_entity;
	t_sprite		*modify_sprite;

// the information for the output when you save the map
	t_list			*points;
	int				point_amount;
	t_list			*walls;
	int				wall_amount;
	t_list			*sectors;
	int				sector_amount;
	t_list			*entities;
	int				entity_amount;

	int				wall_sprite_amount;
	t_xywh			dimensions;

	TTF_Font		*font;
}				t_grid;

typedef	struct		s_changer_prefab
{
	t_bui_element	*menu;
	t_bui_element	*text;
	t_bui_element	*sub_button;
	t_bui_element	*value;
	t_bui_element	*add_button;
}					t_changer_prefab;

struct			s_editor
{
	t_bui_libui			*libui;

	t_bui_window		*window;
	t_bui_element		*toolbox;
	t_bui_element		*info_area;
	t_bui_element		*button_save;
	t_bui_element		*hover_info;

	// Entity presets
	t_list				*entity_presets; // list of t_entity_preset , you can find it in ../core.h
	t_entity_preset		*default_entity;

	// scale changer
	int					scale;
	t_changer_prefab	*scaler;

	t_grid				grid;
	char				*fullpath;
	char				*mapname;
// bui_elements
	t_bui_element		*button_draw;
	t_color_palette		palette;

	t_spawn				spawn;

	//////////////////
	// Info area elements
	//////////////////
	t_bui_element		*button_remove;
	t_bui_element		*button_edit;

	t_bui_element		*general_info;
	t_bui_element		*selected_sector_info;
	t_bui_element		*selected_vector_info;

	t_bui_element		*info_box;
	Uint32				info_box_start_timer;
	Uint32				info_box_timer; // everytime you add something to the info box, it will replace whatever is in it and reset timer.
									// otherwise if the timer ends before youa dd new thing, it will jsut remove the current thing.
	//////////////////
	// Selection Mode
	//////////////////
	t_bui_element		*select_mode;
	t_bui_element		*active_select_mode;
	t_bui_element		*select_mode_vertex;
	t_bui_element		*select_mode_wall;
	t_bui_element		*select_mode_sector;
	t_bui_element		*select_mode_entity;
	t_list				*select_mode_buttons; // list of t_bui_element *

	///////////////////
	// Other Mode
	//////////////////
	t_bui_element		*other_mode;

	/////////////////
	// Buttons
	/////////////////
	t_bui_element		*map_name_input;

	t_bui_element		*endless_tickbox;
	t_bui_element		*story_tickbox;
	t_bui_element		*active_map_type;
	t_list				*map_type_tickboxes; // list of t_bui_element *

	// New edit window
	t_bui_window		*new_edit_window;

	///////////////////
	// Wall elements
	///////////////////
	t_bui_element		*edit_toolbox_wall;
	t_bui_element		*edit_view_wall;

	// toolbox texture adding tabsystem
	t_preset_tab		*wall_tab;

	t_bui_element		*wall_texture_view;
	t_bui_element		*portal_texture_view;
	t_bui_element		*wall_sprite_view;

	t_list				*wall_texture_buttons;	// list of t_bui_element * of the texture buttons for wall
	t_bui_element		*active_wall_texture;
	t_list				*portal_texture_buttons; // list of t_bui_element * of the texture buttons for wall
	t_bui_element		*active_portal_texture;
	t_list				*wall_sprite_buttons;	// list of t_bui_element * of the sprite buttons for wall
	t_bui_element		*active_wall_sprite;

	t_bui_element		*add_wall_sprite_button;
	t_bui_element		*remove_wall_sprite_button;

	// Scale for the wall texture. texture_scale, this is here because of search keyword.
	t_changer_prefab	*texture_scale_changer;

	// solidity tick box for wall
	t_bui_element		*wall_solid;
	t_bui_element		*wall_solid_tick;

	// portal tick box for wall
	t_bui_element		*wall_portal;
	t_bui_element		*wall_portal_tick;

	// temporary variable for the current wall sprite youre editing.
	//t_wall_sprite *active_wall_sprite; // disabled?
	t_changer_prefab	*sprite_scale_changer;
	t_changer_prefab	*sprite_changer;
	int					selected_sprite;

	///////////////////
	// Sector elements,
	///////////////////
	t_bui_element		*edit_toolbox_sector;

	// toolbox sector edit buttons
	t_changer_prefab	*floor_height;
	t_changer_prefab	*ceiling_height;
	t_changer_prefab	*gravity;
	t_changer_prefab	*lighting;

	t_changer_prefab	*floor_scale;
	t_changer_prefab	*ceiling_scale;

	// sector editing stuff
	t_bui_element		*sector_texture_menu;
	t_list				*sector_texture_buttons; // list of t_bui_element * of the texture buttons
	t_bui_element		*active_floor_texture;
	t_bui_element		*active_ceiling_texture;
	t_bui_element		*floor_texture_title;
	t_bui_element		*ceiling_texture_title;

	// slope thingy majig
	t_bui_element		*slope_edit_menu;
	t_bui_element		*slope_sector;
		// floor
	t_bui_element		*slope_floor_title;
	t_changer_prefab	*slope_floor_wall_changer;
	t_changer_prefab	*slope_floor_angle_changer;
		// ceiling
	t_bui_element		*slope_ceiling_title;
	t_changer_prefab	*slope_ceiling_wall_changer;
	t_changer_prefab	*slope_ceiling_angle_changer;

	///////////////////
	// Entity elements
	// ////////////////
	t_bui_element		*edit_toolbox_entity;
	t_bui_element		*edit_view_entity;

	t_preset_dropdown	*entity_type_drop;

	t_bui_element		*edit_entity_direction;
	t_list				*entity_direction_radio_buttons; // list of t_bui_element *
	t_bui_element		*active_direction_button;

	/////////////////////
	// TEXTURES
	/////////////////////
	SDL_Surface			**texture_textures; // these are used for wall_textures, portal_textures, floor- & ceiling textures and wall sprite textures.
	int					texture_amount;
};

void				fps_func(t_fps *fps);

void				add_text_to_info_box(t_editor *editor, char *text);
// NOTE: this is the function that you call in the other program.
void				map_editor(char *map);
void				editor_free(t_editor *editor);

void				edit_window_init(t_editor *editor, t_bui_libui *libui);
void				init_sector_editor(t_editor *editor);
void				init_wall_editor(t_editor *editor);
void				init_entity_editor(t_editor *doom);
void				init_entity_presets(t_list **list);
t_changer_prefab	*new_changer_prefab(t_bui_element *parent_menu, char *title, t_xywh coord);
void				changer_prefab_events(t_changer_prefab *changer, int *current_value, int change_amount);
void				changer_prefab_events_float(t_changer_prefab *changer, float *current_value, float change_amount);
void				remove_from_sprites(t_list **list, t_sprite *s);
t_entity			*get_entity_from_list_at_pos(t_list *list, t_vector pos);
void				sort_sector_wall_list(t_sector *sector);
void				info_area_init(t_editor *editor, t_xywh c);

void				mode_functions(t_editor *editor);
void				draw_all_points(SDL_Surface *surface, t_list *points);

void				color_palette_init(t_color_palette *pal);
void				window_init(t_editor *doom, t_bui_libui *libui);
void				grid_init1(t_editor *doom);
void				grid_init(t_editor *doom);
void				toolbox_init(t_editor *doom);
void				button_init(t_editor *doom);
void				draw_grid(t_editor *doom, t_grid *grid);
void				click_calc(t_editor *doom, t_grid *grid);
void				check_selected(t_grid *grid);
void				unselect_selected(t_editor *editor, t_grid *grid);
void				hover_calc(t_editor *doom, t_grid *grid);
void				draw_sectors(t_grid *grid);
void				draw_wall(t_wall *wall, t_grid *grid, Uint32 color); // note already takes grid->gap into consideration
void				draw_walls(t_grid *grid, t_list **walls, Uint32 color);
void				draw_points(t_grid *grid, t_list *points);
void				draw_entities(t_editor *editor);
void				draw_hover_info(t_editor *doom, t_grid *grid);
void				draw_selected_sector_info(t_editor *doom, t_grid *grid);
void				loop_buttons(t_editor *doom);
void				draw_selected_button(t_editor *doom);
void				sector_edit_buttons_init(t_editor *doom);
void				entity_edit_button_init(t_editor *doom);
void				scale_changer_events(t_bui_libui *libui, t_editor *editor);

void				selection(t_editor *doom, t_grid *grid);
t_point				*get_point_from_list_around_radius(t_list *points, t_vector pos, float allowed_radius);
t_entity			*get_entity_from_list_around_radius(t_list *entities, t_vector pos, float allowed_radius);
t_wall				*get_wall_from_list_around_radius(t_list *walls, t_vector pos, float allowed_radius);
void				select_sector(t_grid *grid);

void				drag_calc(t_editor *doom, t_grid *grid);
void				draw_selected_point(t_editor *doom, t_grid *grid);
void				draw_selected_wall(t_grid *grid);
void				draw_selected_sector(t_editor *doom, t_grid *grid);
void				draw_selected_entity(t_grid *grid);
void				boundaries(t_editor *doom, t_grid *grid);
void				selection_mode_buttons(t_editor *doom, t_grid *grid);
void				show_selected_wall_texture(t_editor *doom, t_grid *grid);
void				selected_option_menu(t_editor *doom, t_grid *grid);

char				*args_parser(int ac, char **av);
t_point				*get_point_from_list_at_pos(t_list *list, t_vector v);
t_wall				*get_wall_from_list(t_list **list, t_point *v1, t_point *v2);
t_sprite			*get_sprite_from_list(t_list **list, int x, int y);
void				update_real_dimensions(t_grid *grid);
void				remove_from_walls(t_list **walls, t_wall *wall);
void				remove_from_points(t_list **points, t_point *v);
int					vector_in_wall(t_vector v, t_wall *vec);
int					vector_on_wall(t_vector v, t_wall *line);

void				remove_everything_from_list(t_list **list);

void				recount_everything(t_editor *doom);
void				update_general_info_element(t_editor *editor);

/* Sector */
t_sector			*get_sector_with_wall(t_list **sector_list, t_wall *wall);
void				check_sector_wanter(t_editor *doom, SDL_Event *e);
void				remove_wall_from_sector(t_sector **sector, t_wall *wall); // use this if you want to remove a specific wall from a specific sector
void				remove_from_sectors(t_list **sectors, t_sector *sec); // use this if you want to remove t_sector from t_list of t_sectors
void				remove_from_walls_non_free(t_list **walls, t_wall *wall); // use this if you want to remove the t_wall from a t_list without freeing the actual t_wall
void				remove_wall_from_all_sectors(t_list **sectors, t_wall *wall); // use this if you wanna remove the wall from all the sectors that have it
void   				remove_wall_from_its_sector(t_grid *grid, t_wall *wall); // use this if you dont know which sector the wall is a part of.. and only if there is only one sector with that wall
void				remove_all_non_existing_portals(t_list **sectors);
int					get_sector_wall_amount(t_sector *sector);

/* Point */
void				remove_all_points_not_a_part_of_a_wall(t_list **points, t_list **walls);
int					get_point_connection_amount(t_list **walls, t_point *point);

/* Wall */
void				remove_all_lonely_walls(t_list **walls, t_list **sectors);

void				texture_init(t_editor *doom);
void				texture_buttons(t_editor *doom, t_grid *grid);
void				sprite_init(t_editor *doom);
void				entity_sprite_init(t_editor *doom);
void				entity_sprite_buttons(t_editor *doom, t_grid *grid);
void				add_portal(t_grid *grid);
void				remove_portal(t_grid *grid);

/*
** Help
*/
void				remove_all_non_existing_sectors(t_editor *editor);
void				remove_all_non_existing_portals(t_list **sectors);
void				remove_all_lonely_walls(t_list **walls, t_list **sectors);
void				remove_all_lonely_points(t_editor *editor);

void				remove_from_list_if_with(t_list **list, void *s, int (*cmp)(void *, void *), void (*del)(void *, size_t));
float				get_wall_length(t_wall *wall);
int					get_list_len(t_list **list);
int					vector_is_empty(t_vector v);
int					pointer_compare(void *p1, void *p2);
int					vector_compare(t_vector v1, t_vector v2);
int					wall_compare(void *v1, void *v2);
void				add_to_list(t_list **list, void *new_v, size_t size);
t_list				*get_nth_from_list(t_list **list, int index);
t_point				*new_point(t_vector pos);
t_wall				*new_wall(t_point *orig, t_point *dest);
t_sprite			*new_sprite(void);
t_sector			*new_sector(int id);
t_entity			*new_entity(int id, t_vector pos);
t_entity_preset		*new_entity_preset(char *name);
t_entity_preset		*get_entity_preset_with_name(t_list *list, char *name);
void				free_point(void *content, size_t size);
void				free_sprite(void *content, size_t size);
void				free_wall(void *content, size_t size);
void				free_sector(void *content, size_t size);
void				free_sprite(void *content, size_t size);
void				free_entity(void *content, size_t size);
void				free_entity_preset(void *content, size_t size);
Uint32				random_color(void);
SDL_Surface			*load_bxpm_to_surface(char *bxpm_file);

void				set_map(t_editor *doom);
void				read_map_file(t_editor *doom);

#endif
