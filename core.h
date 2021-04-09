#ifndef CORE_H
# define CORE_H
# define SDL_MAIN_HANDLED

# include "libgfx.h"
//# include "./engine/game/srcs/doom.h"
# include <time.h>

# define INT_MAX 2147483647
# define INT_MIN -2147483648
# define EMPTY_VEC (t_vector){INT_MAX, INT_MAX, INT_MAX}

typedef	struct	s_sector	t_sector;
typedef	struct	s_wall		t_wall;
typedef	struct	s_point		t_point;
typedef struct	s_entity	t_entity;
typedef	struct	s_entity_preset	t_entity_preset;
typedef struct	s_texture	t_texture;
typedef struct	s_game		t_game;
typedef	struct	s_player	t_player;

// Remove s_xyz and s_doom and uncomment ./engine/game...
# define W				1920
typedef struct	s_xyz
{
	float	x;
	float	y;
	float	z;
}				t_xyz;

typedef struct		s_wall_a
{
	t_xyz			v1;
	t_xyz			v2;
	int				texture;
}			t_wall_a;	
typedef struct		s_entity_a
{
	t_xyz			where;
	float			dist;
}					t_entity_a;

typedef struct	s_sector_a
{
    float		floor;
	float		ceil;
    signed char *neighbors;
    signed char *textures;
    unsigned	npoints;
    t_xyz		*vertex;
	float		light;
	float		gravity;
	short		entity_nb;
	t_entity_a	entity[10];
} 				t_sector_a;

typedef struct	s_player_a
{
    t_xyz		where;
    t_xyz		velocity;
    float		angle;
	float		anglesin;
	float		anglecos;
	float 		yaw;
	int 		moving;
	int			falling;
	int			ground;
	int			ducking;
    unsigned	sector;
}				t_player_a;
typedef	struct		s_keys
{
	int				chr;
	short			num;
	int				fnc;
	int				w;
	int				a;
	int				s;
	int				d;
	int				t;
	int				space;
	int				l_ctrl;
	int				l_shift;
	int				tab;
}					t_keys;
typedef	struct		s_fps
{
	float		curr;
	float		prev;
	float		avg;
	float		delta;
	int		fps;
	int		count;
}					t_fps;
typedef struct	s_ab
{
	int			a1;
	int			a2;
	int			b1;
	int			b2;
}				t_ab;

typedef struct	s_height_info //yinfo
{
	float		yceil;
	float		yfloor;
	float		nyceil;
	float		nyfloor;
	t_ab		y;
	t_ab		ny;
}				t_height_info;


typedef struct	s_doom
{
	//window_init
	int					quit;
	char				*file;
	char				*name;
	SDL_Window			*win;
	SDL_Surface			*surface;
	//t_tpool				tpool;

	//read_map
	t_xyz				*vert;//is redundant after secotors are created
	t_wall_a				*walls; //temp
	t_sector_a			*sectors;
	t_entity_a			*entity;
	t_player_a			player;
	unsigned			num_sector;
	unsigned			num_sectors;

	//Input
	t_keys				key;

	//random
	int					start_x;
	int					end_x;
	float				yaw;
	t_fps				fps;
	short				ytop[W];
	short				ybottom[W];
	t_height_info		height_info;

	//Textures
	SDL_Surface			*texture[5];
	int					u0;
	int					u1;
	SDL_Surface			*imp;
	t_list *entities;

}		t_doom;

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

	// New
	t_xywh			coord;
	float			scale;
}				t_sprite;

struct				s_wall
{
	unsigned int	id;
	t_point			*orig;
	t_point			*dest;
	float			texture_scale;
	short int		texture_id;
	short int		portal_texture_id;
	int			solid; // default 1 if wall, default 0 if portal
	t_list			*sprites; // list of t_sprite 
	int				neighbor;
};

struct				s_sector
{
	unsigned int	id;
	t_list			*walls;
	int		floor_height;
	int		ceiling_height;
	int		floor_texture;
	int		ceiling_texture;

	float			floor_texture_scale;
	float			ceiling_texture_scale;

	int		gravity;		// this is short int but in the real game a float (from 0.10 - 0.01)
	int		light_level;	// this is short int but in the real game a float (from 1.0 - 0.1)
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

// TODO: try to remove all the extra info of this and store everything in the preset
// 	aka mood, name ,speed, health...
struct				s_entity
{
	unsigned int	id;
	t_vector		pos;
	int			direction; // in degrees 
	t_entity_preset			*preset;

	int				statique; // if npc or static sprite
};

struct				s_entity_preset
{
	char			*name;
	float			scale;
	int			mood;
	int			health;
	int			damage;
	int			speed;
	int			attack_style;
	int			flying;
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
