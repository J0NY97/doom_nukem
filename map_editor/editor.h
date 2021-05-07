#ifndef EDITOR_H
# define EDITOR_H
# define SDL_MAIN_HANDLED

# include "../core.h"
# include "libft.h"
# include "better_libui.h"
# include "libgfx.h"
# include "ft_printf.h"
# include "../path.h"

// TODO: portal button in the wall edit window. som en tickbox
//
// texture
// < 0  == skybox
// 0 == monochorme
// > 0 == textures

// Check funtion speed.
/*before
 clock_gettime(_CLOCK_MONOTONIC, &start);

 * after
    clock_gettime(_CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("%f\n", elapsed);*/

// TODO: hotkeys for all the buttons.
//
// TODO: when in selection mode. ctrl or (drag and highlight) to choose multiple points, walls, sectors, entities for deletion. 
//
// TODO: draw the grid only once on the surface of the grid, and only update it when you zoom.
//
// TODO: split wall doesnt workin (the question is, is that important?)
//
// TODO: when edit view is open dont enable keypresses in the grid thinga majig (done in libui?)
//
// TODO: on the right side of the wall editor, you can add a list for all the wall sprites and from there choose the sprite
// 	you want to edit (move or rechoose texture)
// 	maybe add this to the sector editing, where you can choose each wall for editiing.
//
// TODO: on the right side of the sector editor, list all the walls that are in taht sector then you can choose from there to edit them?
//
// MAJOR TODO: if you can figure out how to remove sectors until you have the file, it would be insane. and make this project
// 		so much better and probably less complicated.
//
// TODO: right click draws, left click selects, double click edits. (instead of exactly this, do this only for selecting)
// 	draw mode; (select mode not draw :))
// 		-vertex, adds vertex where you click.
// 		-line, draws line between 2 points you click.
// 		-sector, functions the same as the current type of drawing.
// 		-thing, adds things that you can edit at a later time.
// 		-create sector from, creates a sector from a compilation of lines.... havent thought enought about this mode.
// 	i have rethunk this idea, not quite the same as what this text says but idgaf to change it, i hope i will remember
// 	my new idea when i actually do it.
//
// TODO: having different modes for selecting, so that you can just return the closest whatever (depending on the mode) to
// 	where you clicked. (calc the distance from mouse to the point youre checking)
// 	NOTE: read the TODO above
//
// TODO: scrolling actually scrolls towards where youre scrolling, i have done this in the mandelbrot thing.
//
// TODO: temporary save the file everytime you have completed a new sector, so that you can go back.
//
// TODO: status/info area, where you can send stuff that are important for the user, (when you save it tells you, if you cant
// 	save it tells you, etc...)
//
// TODO: not letting user save in some cases (if  there is no spawn (this breaks the game))
//
// TODO: a way of removing wall_sprites
//
// TODO: instead of having some default images on the texture chagner actually show all the textures you can choose from,
// 	be able to give a texture pack and/or load textures that you can set as "wall textures"/"portal textures"/"wall sprites"
// 	t_texture *load_texture(TEXTURE_WALL/TEXTURE_PORTAL/TEXTURE_SPRITE, char *path);
// 	and depending ont the type you give in it will save in a list of textures, from where the editor window will take
// 	the textures for the buttons from, i hope this makes sense.
//
// TODO: instead of zoom changing 1 pixel at a time +/-, you should make it change procentuellt.

// Remove this if not used in the final version.
enum e_select_mode
{
	VERTEX_MODE,
	LINE_MODE,
	SECTOR_MODE,
	CREATE_SECTOR_MODE // not sure what this does yet
};

enum e_entity
{
	ENTITY_TYPE_HOSTILE,
	ENTITY_TYPE_FRIENDLY,
	ENTITY_TYPE_NEUTRAL,
	ENTITY_STYLE_NONE = -1,
	ENTITY_STYLE_MELEE,
	ENTITY_STYLE_RANGED
};

typedef	struct	s_editor			t_editor;

typedef struct	s_color_palette
{
	int			win;
	int			win_elem;
	int			elem_elem;
	int			elem_elem_elem;

	// Coolors.co
	int	granny_smith_apple;
	int	peach_crayola;
	int	light_blue;
}				t_color_palette;

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
	int		direction; // degrees
}				t_spawn;

// TODO: at some point make this struct redundant
typedef	struct	s_grid
{
	t_bui_element	*elem; // the actual element where you draw everything.

	// Note: this is like this because its easier when you finally save the points to map file.
	t_vector	hover; // this is calculating from the coordinate system used in this program.

	t_xywh		coords;
	int			x;
	int			y;
	int			gap;
	t_vector	selected1;
	t_vector	selected2;
// these are the information for when you want to edit
	t_point		*modify_point;
	t_wall		*modify_wall;
	t_sector	*modify_sector;
	t_entity	*modify_entity;
	t_sprite	*modify_sprite;

// the information for the output when you save the map
	t_list		*points;
	int		point_amount;
	t_list		*walls;
	int		wall_amount;
	t_list		*sectors;
	int		sector_amount;
	t_list		*entities;
	int		entity_amount;

	int		wall_sprite_amount;
	// t_list		*portals;
	t_xywh			dimensions;
}				t_grid;

typedef	struct		s_changer_prefab
{
	t_bui_element	*menu;
	t_bui_element	*text;
	t_bui_element	*sub_button;
	t_bui_element	*value;
	t_bui_element	*add_button;
}			t_changer_prefab;

struct			s_editor
{
	t_bui_libui	*libui;

	t_bui_window	*window;
	t_bui_element	*toolbox; // menu / surface
	t_bui_element	*info_area; // menu / surface / located on toolbox
	t_bui_element	*button_remove;
	t_bui_element	*button_save;
	t_bui_element	*button_edit;
	t_bui_element	*hover_info;
	t_bui_element	*selected_sector_info;
	t_bui_element	*selected_vector_info;

	// Entity presets
	t_list *entity_presets; // list of t_entity_preset , you can find it in ../core.h
	t_entity_preset *default_entity;

	// scale changer
	int scale;
	t_changer_prefab *scaler;

	t_grid		grid;
	char		*fullpath;
	char		*mapname;
// bui_elements
	t_bui_element	*button_draw;
	t_color_palette	palette;
	t_texture	textures[1];
	t_bui_element	**texture_buttons;

	t_map		map;
	t_spawn		spawn;

	t_texture	sprites[1];
	t_bui_element	**sprite_buttons;

	t_texture	entity_sprites[1];
	t_bui_element	**entity_sprite_buttons;

	// New stuff
	t_list		*all_textures; // list of t_editor_texture (note: wall, portal and wall_sprite textures take their tex from here)
	//////////////////
	// Draw Mode
	//////////////////
	t_bui_element *draw_mode;

	//////////////////
	// Selection Mode
	//////////////////
	t_bui_element *select_mode;
	t_list *select_mode_buttons; // list of t_bui_element *
	t_bui_element *active_select_mode;
	t_bui_element *select_mode_vertex;
	t_bui_element *select_mode_wall;
	t_bui_element *select_mode_sector;
	t_bui_element *select_mode_entity;

	///////////////////
	// Other Mode
	//////////////////
	t_bui_element *other_mode;

	/////////////////
	// Buttons
	/////////////////
	t_bui_element *map_name_input;

	t_bui_element *endless_tickbox;
	t_bui_element *story_tickbox;
	t_bui_element *active_map_type;
	t_list *map_type_tickboxes; // list of t_bui_element * 

	// New edit window
	t_bui_window	*new_edit_window;

	///////////////////
	// Wall elements
	///////////////////
	t_bui_element *edit_toolbox_wall;
	t_bui_element *edit_view_wall;

	// toolbox texture adding tabsystem
	t_preset_tab *wall_tab;

	t_bui_element *wall_texture_view;
	t_bui_element *portal_texture_view;
	t_bui_element *wall_sprite_view;
	
	// TODO: init the active to null
	t_list *wall_texture_buttons;	// list of t_bui_element * of the texture buttons for wall
	t_bui_element *active_wall_texture;
	t_list *portal_texture_buttons; // list of t_bui_element * of the texture buttons for wall
	t_bui_element *active_portal_texture;
	t_list *wall_sprite_buttons;	// list of t_bui_element * of the sprite buttons for wall
	t_bui_element *active_wall_sprite;

	t_bui_element *add_wall_sprite_button;
	t_bui_element *remove_wall_sprite_button;
	
	// Scale for the wall texture. texture_scale, this is here because of search keyword.
	t_bui_element *wall_scale;
	t_bui_element *wall_scale_value;
	t_bui_element *wall_scale_add;
	t_bui_element *wall_scale_sub;

	// solidity tick box for wall
	t_bui_element *wall_solid;
	t_bui_element *wall_solid_tick;

	// portal tick box for wall
	t_bui_element *wall_portal;
	t_bui_element *wall_portal_tick;

	// temporary variable for the current wall sprite youre editing.
	//t_wall_sprite *active_wall_sprite; // disabled?
	t_bui_element *sprite_scale;
	t_bui_element *sprite_scale_value;
	t_bui_element *sprite_scale_add;
	t_bui_element *sprite_scale_sub;

	///////////////////
	// Sector elements,
	///////////////////
	t_bui_element	*edit_toolbox_sector;
	t_bui_element	*edit_view_sector;

	// toolbox sector edit buttons
	t_changer_prefab	*floor_height;
	t_changer_prefab	*ceiling_height;
	t_changer_prefab	*gravity;
	t_changer_prefab	*lighting;

	t_changer_prefab	*floor_scale;
	t_changer_prefab	*ceiling_scale;

	// sector editing stuff
	t_bui_element	*sector_ceiling_menu;
	t_bui_element	*sector_floor_menu;
	t_list		*ceiling_texture_buttons; // list of t_bui_element * of the texture buttons for ceiling
	t_list		*floor_texture_buttons;	  // list of t_bui_element * of the texture buttons for floor
	t_bui_element	*active_floor_texture;
	t_bui_element	*active_ceiling_texture;


	///////////////////
	// Entity elements
	// ////////////////
	t_bui_element *edit_toolbox_entity;
	t_bui_element *edit_view_entity; 	

	t_preset_dropdown *entity_type_drop;

	t_list	*entity_direction_radio_buttons; // list of t_bui_element, these are the "radio" buttons for the direction of entity
	t_bui_element *active_direction_button;
};

// NOTE: this is the function that you call in the other program.
void			map_editor(char *map);
void			editor_free(t_editor *editor);
// Rewrites
void			edit_window_init(t_editor *editor, t_bui_libui *libui);
void			init_sector_editor(t_editor *editor);
void			init_wall_editor(t_editor *editor);
void			init_entity_editor(t_editor *doom);
void			init_entity_presets(t_list **list, char *path);
t_changer_prefab	*new_changer_prefab(t_bui_element *parent_menu, char *title, t_xywh coord);
void			changer_prefab_events(t_changer_prefab *changer, int *current_value, int change_amount);
void			changer_prefab_events_float(t_changer_prefab *changer, float *current_value, float change_amount);
void			remove_from_sprites(t_list **list, t_sprite *s);
t_entity		*get_entity_from_list_at_pos(t_list *list, t_vector pos);

void			mode_functions(t_editor *editor);
void			draw_all_points(SDL_Surface *surface, t_list *points);

void			color_palette_init(t_color_palette *pal);
void			window_init(t_editor *doom, t_bui_libui *libui);
void			grid_init(t_editor *doom);
void			toolbox_init(t_editor *doom);
void			button_init(t_editor *doom);
void			draw_grid(t_editor *doom, t_grid *grid);
void			click_calc(t_editor *doom, t_grid *grid);
void			check_selected(t_editor *doom, t_grid *grid);
void			unselect_selected(t_editor *doom, t_grid *grid, SDL_Event *e);
void			hover_calc(t_editor *doom, t_grid *grid);
void			draw_sectors(t_editor *doom, t_grid *grid);
void			draw_walls(t_grid *grid, t_list **walls, Uint32 color);
void			draw_points(t_editor *doom, t_grid *grid, t_list *points);
void			draw_entities(t_editor *doom, t_grid *grid);
void			draw_hover_info(t_editor *doom, t_grid *grid);
void			draw_selected_sector_info(t_editor *doom, t_grid *grid);
void			loop_buttons(t_editor *doom);
void			draw_selected_button(t_editor *doom);
void			sector_edit_buttons_init(t_editor *doom);
void			entity_edit_button_init(t_editor *doom);
void			scale_changer_events(t_bui_libui *libui, t_editor *editor);

void			selection(t_editor *doom, t_grid *grid, SDL_Event *e);
void			select_point(t_editor *doom, t_grid *grid);
void			select_wall(t_editor *doom, t_grid *grid);
void			select_sector(t_editor *doom, t_grid *grid);
void			select_entity(t_editor *doom, t_grid *grid);

void			drag_calc(t_editor *doom, t_grid *grid, SDL_Event *e);
void			draw_selected_point(t_editor *doom, t_grid *grid);
void			draw_selected_wall(t_editor *doom, t_grid *grid);
void			draw_selected_sector(t_editor *doom, t_grid *grid);
void			draw_selected_entity(t_editor *doom, t_grid *grid);
void			boundaries(t_editor *doom, t_grid *grid);
void			selection_mode_buttons(t_editor *doom, t_grid *grid);
void			show_selected_wall_texture(t_editor *doom, t_grid *grid);
void			selected_option_menu(t_editor *doom, t_grid *grid, t_bui_libui *libui);

int				args_parser(t_editor *editor, int ac, char **av);
t_point			*get_point_from_list(t_list *list, t_point *v);
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
int				get_sector_wall_amount(t_sector *sector);

/* Point shit */
void			remove_all_points_not_a_part_of_a_wall(t_list **points, t_list **walls);
int				get_point_connection_amount(t_list **walls, t_point *point);

/* Wall shit */
void			remove_all_walls_not_a_part_of_a_sector(t_list **walls, t_list **sectors);

void			texture_init(t_editor *doom);
void			texture_buttons(t_editor *doom, t_grid *grid);
void			sprite_init(t_editor *doom);
void			entity_sprite_init(t_editor *doom);
void			entity_sprite_buttons(t_editor *doom, t_grid *grid);
void			add_portal(t_editor *doom, t_grid *grid);
void			remove_portal(t_editor *doom, t_grid *grid);

/*
** Help
*/
int				vector_is_empty(t_vector v);
int				vector_compare(t_vector v1, t_vector v2);
int				wall_compare(t_wall *v1, t_wall *v2);
void			add_to_list(t_list **list, void *new_v, size_t size);
t_list			*get_nth_from_list(t_list **list, int index);
t_wall			*new_wall(t_point *orig, t_point *dest);
t_sprite		*new_sprite(void);
t_sector		*new_sector(int id);
t_entity		*new_entity(int id, t_vector pos);
t_entity_preset		*new_entity_preset(void);
t_entity_preset		*get_entity_preset_from_list_with_name(t_list *list, char *name);
void			free_point(void *content, size_t size);
void			free_sprite(void *content, size_t size);
void			free_wall(void *content, size_t size);
void			free_sector(void *content, size_t size);
void			free_sprite(void *content, size_t size);
void			free_entity(void *content, size_t size);
void			free_entity_preset(void *content, size_t size);
Uint32			random_color(void);

void			set_map(t_editor *doom);
void			read_map_file(t_editor *doom);

#endif
