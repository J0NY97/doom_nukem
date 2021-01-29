#ifndef EDITOR_H
# define EDITOR_H
# define SDL_MAIN_HANDLED

# include "core.h"
# include "libft.h"
# include "libui.h"
# include "libgfx.h"
# include "ft_printf.h"

typedef	struct	s_editor			t_editor;

typedef struct	s_color_palette
{
	int			win;
	int			win_elem;
	int			elem_elem;
	int			elem_elem_elem;
}				t_color_palette;

typedef	struct	s_real_map
{
	int			x;
	int			y;
	int			w;
	int			h;
}				t_real_map;

typedef	struct	s_map
{
	int			w;
	int			h;
	char		*texture_pack;
	char		*name;
}				t_map;

typedef	struct	s_spawn
{
	t_vector	pos;
}				t_spawn;

typedef	struct	s_grid
{
	t_vector	hover;

	int			x;
	int			y;
	int			gap;
	t_vector	selected1;
	t_vector	selected2;
	t_element	*elem;
// these are the information for when you want to edit
	t_point		*modify_point;
	t_wall		*modify_wall;
	t_sector	*modify_sector;
	t_entity	*modify_entity;

// the information for the output when you save the map
	t_list		*points;
	int			point_amount;
	t_list		*walls; // try to make the walls list redundant, they should only be stored in the sectors.
	int			wall_amount;
	t_list		*sectors;
	int			sector_amount;
	t_list		*entities;
	int			entity_amount;
	// t_list		*portals;
	t_real_map	dimensions;
}				t_grid;

typedef	struct	s_sector_edit
{
	t_element	*text;
	t_element	*sub_button;
	t_element	*amount;
	short int	*f_amount;
	t_element	*add_button;
}				t_sector_edit;

typedef	struct	s_wall_edit
{
	t_element	*menu;
	t_element	*title;
	t_element	*view;
	t_element	*info;
	t_element	*add_view;

// wall editing shit
	t_element	*texture_button;
	t_element	*textures;

	t_element	*add_button;
	t_element	*sprites;	// the surface with all of the sprite buttons on it
	short int	selected_sprite;

	t_element	*show_render;
	t_sprite	*modify_sprite;

// sector editing shit
	t_list		*sector_edit_buttons; // t_list of s_sector_edit;
// entity editing shit
	t_element	*ent_sprite_button;
	t_element	*ent_sprites;
	t_element	*ent_render_sprite;

	t_element	*ent_info_button;
	t_element	*ent_info_menu;

	t_element	*ent_info_id_text;

	t_element	*ent_info_health_text;
	t_element	*ent_info_health_text_area;

	t_element	*ent_info_speed_text;
	t_element	*ent_info_speed_text_area;

	t_element	*ent_info_armor_text;
	t_element	*ent_info_armor_text_area;

	t_element	*type_dropdown; // neutral, friendly, enemy
}				t_wall_edit;

struct			s_editor
{
	int map_w;
	int map_h;
	t_window	*window;
	t_element	*toolbox; // menu / surface
	t_element	*info_area; // menu / surface / located on toolbox
	t_element	*button_remove;
	t_element	*button_save;
	t_element	*button_edit;
	t_element	*button_add;
	t_element	*button_remove_portal;
	t_grid		grid;
	char		*filename;
// elements
	t_element	*button_draw;
	t_element	*button_select;
	t_color_palette	palette;
	t_texture	textures[1];
	t_element	**texture_buttons;

	t_map		map;
	t_spawn		spawn;

	t_texture	sprites[1];
	t_element	**sprite_buttons;

	t_texture	entity_sprites[1];
	t_element	**entity_sprite_buttons;
	// edit window
	t_window	*edit_window;
	t_wall_edit	option;
};

void			color_palette_init(t_color_palette *pal);
void			window_init(t_editor *doom, t_libui *libui);
void			grid_init(t_editor *doom);
void			toolbox_init(t_editor *doom);
void			button_init(t_editor *doom);
void			draw_grid(t_editor *doom, t_grid *grid);
void			click_calc(t_editor *doom, t_grid *grid, SDL_Event *e);
void			check_selected(t_editor *doom, t_grid *grid);
void			unselect_selected(t_editor *doom, t_grid *grid, SDL_Event *e);
void			hover_calc(t_editor *doom, t_grid *grid);
void			draw_sectors(t_editor *doom, t_grid *grid);
void			draw_walls(t_grid *grid, t_list **walls, Uint32 color);
void			draw_points(t_editor *doom, t_grid *grid);
void			draw_entities(t_editor *doom, t_grid *grid);
void			draw_hover_info(t_editor *doom, t_grid *grid);
void			draw_selected_sector_info(t_editor *doom, t_grid *grid);
void			loop_buttons(t_editor *doom);
void			draw_selected_button(t_editor *doom);
void			option_menu_init(t_editor *doom);
void			sector_edit_buttons_init(t_editor *doom);
void			entity_edit_button_init(t_editor *doom);

void			selection(t_editor *doom, t_grid *grid, SDL_Event *e);
void			select_point(t_editor *doom, t_grid *grid, SDL_Event *e);
void			select_wall(t_editor *doom, t_grid *grid, SDL_Event *e);
void			select_sector(t_editor *doom, t_grid *grid, SDL_Event *e);
void			select_entity(t_editor *doom, t_grid *grid, SDL_Event *e);

void			drag_calc(t_editor *doom, t_grid *grid, SDL_Event *e);
void			draw_selected_point(t_editor *doom, t_grid *grid);
void			draw_selected_wall(t_editor *doom, t_grid *grid);
void			draw_selected_sector(t_editor *doom, t_grid *grid);
void			draw_selected_entity(t_editor *doom, t_grid *grid);
void			boundaries(t_editor *doom, t_grid *grid);
void			selection_mode_buttons(t_editor *doom, t_grid *grid);
void			show_selected_wall_texture(t_editor *doom, t_grid *grid);
void			selected_option_menu(t_editor *doom, t_grid *grid, SDL_Event *e);

int				args_parser(char **filename, int ac, char **av);
t_point			*get_point_from_list(t_grid *grid, t_point *v);
t_wall			*get_wall_from_list(t_list **list, t_point *v1, t_point *v2);
t_sprite		*get_sprite_from_list(t_list **list, int x, int y);
void			update_real_dimensions(t_editor *doom, t_grid *grid);
void			remove_from_walls(t_list **walls, t_wall *wall);
void			remove_from_points(t_list **points, t_point *v);
int				vector_in_wall(t_vector v, t_wall *vec);
int				vector_on_wall(t_vector v, t_wall *line);

void			remove_everything_from_list(t_list **list);

void			recount_everything(t_editor *doom);

/* Sector shit */
void			check_sector_wanter(t_editor *doom, SDL_Event *e);
void			remove_wall_from_sector(t_sector **sector, t_wall *wall); // use this if you want to remove a specific wall from a specific sector
void			remove_from_sectors(t_list **sectors, t_sector *sec); // use this if you want to remove t_sector from t_list of t_sectors
void			remove_from_walls_non_free(t_list **walls, t_wall *wall); // use this if you want to remove the t_wall from a t_list without freeing the actual t_wall
void			remove_wall_from_all_sectors(t_list **sectors, t_wall *wall); // use this if you wanna remove the wall from all the sectors that have it
void   			remove_wall_from_its_sector(t_editor *doom, t_grid *grid, t_wall *wall); // use this if you dont know which sector the wall is a part of.. and only if there is only one sector with that wall
void			remove_all_non_existing_portals(t_list **sectors);

/* Point shit */
void			remove_all_points_not_a_part_of_a_wall(t_list **points, t_list **walls);
int				get_point_connection_amount(t_list **walls, t_point *point);

/* Wall shit */
void			remove_all_walls_not_a_part_of_a_sector(t_list **walls, t_list **sectors);

void			texture_init(t_editor *doom);
void			texture_buttons(t_editor *doom, t_grid *grid);
void			sprite_init(t_editor *doom);
void			sprite_buttons(t_editor *doom, t_grid *grid);
void			entity_sprite_init(t_editor *doom);
void			entity_sprite_buttons(t_editor *doom, t_grid *grid);

/*
** Help
*/
int				vector_is_empty(t_vector v);
int				vector_compare(t_vector v1, t_vector v2);
int				wall_compare(t_wall *v1, t_wall *v2);
void			add_to_list(t_list **list, void *new_v, size_t size);
t_list			*get_nth_from_list(t_list **list, int index);
t_wall			*new_wall(t_point *orig, t_point *dest);
t_sector		*new_sector(int id);
t_entity		*new_entity(int id, t_vector pos);
Uint32			random_color(void);

void			set_map(t_editor *doom);
void			read_map_file(t_editor *doom);

#endif
