#ifndef CORE_H
# define CORE_H
# define SDL_MAIN_HANDLED

# include "libgfx.h"
# include "./engine/game/srcs/doom.h"
# include <time.h>

# define INT_MAX 2147483647
# define INT_MIN -2147483648
# define EMPTY_VEC (t_vector){INT_MAX, INT_MAX, INT_MAX}

typedef	struct	s_sector	t_sector;
typedef	struct	s_wall		t_wall;
typedef	struct	s_point		t_point;
typedef struct	s_entity	t_entity;
typedef struct	s_texture	t_texture;
typedef struct	s_game		t_game;
typedef	struct	s_player	t_player;

struct	s_point
{
	unsigned int	id;
	t_vector		pos;
};

typedef struct	s_sprite
{
	t_vector		pos;
	double			w;
	double			h;
	short int		sprite_id;
}					t_sprite;

struct				s_wall
{
	unsigned int	id;
	t_point			*orig;
	t_point			*dest;
	short int		texture_id;
	t_list			*sprites;
	int				neighbor;
};

struct				s_sector
{
	unsigned int	id;
	t_list			*walls;
	short int		floor_height;
	short int		ceiling_height;
	short int		floor_texture;
	short int		ceiling_texture;
	short int		gravity;		// this is short int but in the real game a float (from 0.10 - 0.01)
	short int		light_level;	// this is short int but in the real game a float (from 1.0 - 0.1)
	//t_list			*neighbors;

	t_point			*first_point;
	Uint32			color;
	t_vector		lowest_pos; // these 2 vectors are so you can calculate...
	t_vector		highest_pos; // ...the middle of the sector.
};

typedef struct		s_weapon
{
	char			*name;
	int				damage;
	int				firerate;
	int				ammo_capacity;
	int				ammo_current;
	int				projectile_speed;
	int				reload_speed;
	int				equip_speed;
	int				range;
	short int		texture_id;
	int				price;
	int				upgrade_price_modifier;
	char			*ui_image;
}					t_weapon;

typedef	struct		s_perk
{
	char			*name;
	char			*description;
	int				type;			// player = 0; weapon = 1;
	int				tier;
	int				price;
	void			(*f)(t_game *, t_player *);
}					t_perk;

struct				s_entity
{
	unsigned int	id;
	t_vector		pos;
	t_vector		dir;
	short int		sprite_id;
	int				max_health;
	int				speed;
	int				armor;
	int				type; // 2 = neutral, 1 = friendly, 0 = enemy
	//int				health;
	//int				speed_modifier;
	// t_weapon			weapon;

	int				statique; // if npc or static sprite
};

struct				s_texture
{
	SDL_Surface		*surface;
	Uint32			*pixels;
	int				**position;
	int				x_img;
	int				y_img;
	int				max_textures;
	int				x_size;
	int				y_size;
};

typedef	struct		s_inventory
{
	int				money;
}					t_inventory;

struct				s_player
{
	int				health;
	int				armor;
	int				ammo;

	int				max_health;
	int				max_armor;
	int				max_ammo;
	t_inventory		inventory;
	t_weapon		*weapon_equipped;

	int				damage_modifier;
	int				firerate_modifier;
	float			lifesteal; // %
	float			reload_speed_modifier;
	t_list			*perks; // list of t_perk's
	t_list			*weapons;
};

struct				s_game
{
	time_t			timer_start;
	t_doom			*doom;
	t_player		player;
	t_list			*all_weapons; // list of t_weapons?
	t_list			*perks; // list of all perks (t_perk *)
};

/*
** Textures
*/
void	load_texture(t_texture *t, char *dir, int x_size, int y_size);
void	split_texture(t_texture *t);

/*
** Help
*/
void	add_to_list(t_list **list, void *new_v, size_t size);
void	free_array(char **arr);

/*
** Perks
*/
void	init_perks(t_game *game);
t_perk	*get_perk(t_list **perks, char *name);

#endif
