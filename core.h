#ifndef CORE_H
# define CORE_H
# define SDL_MAIN_HANDLED

# include "libgfx.h"
# include <time.h>

# define INT_MAX 2147483647
# define INT_MIN -2147483648
# define EMPTY_VEC (t_vector){INT_MAX, INT_MAX, INT_MAX}

typedef	struct	s_sector	t_sector;
typedef	struct	s_wall		t_wall;
typedef	struct	s_point		t_point;
typedef struct	s_entity	t_entity;
typedef	struct	s_entity_preset	t_entity_preset;

struct	s_point
{
	int				id;
	t_vector		pos;
};

typedef struct	s_sprite
{
	t_vector		pos;
	double			w;
	double			h;
	short int		sprite_id;

	// New
	float			real_x;
	float			real_y;
	t_xywh			coord;
	float			scale;
}				t_sprite;

struct				s_wall
{
	int				id;
	t_point			*orig;
	t_point			*dest;
	float			texture_scale;
	short int		texture_id;
	short int		portal_texture_id;
	int				solid; // default 1 if wall, default 0 if portal
	t_list			*sprites; // list of t_sprite 
	int				neighbor;
	t_sector		*neighbor_sector;
};

struct				s_sector
{
	int				id;
	t_list			*walls;
	int		floor_height;
	int		ceiling_height;
	int		floor_texture;
	int		ceiling_texture;

	float			floor_texture_scale;
	float			ceiling_texture_scale;

	int	floor_slope_wall_id; // wall id from which the angle starts.
	int	ceiling_slope_wall_id;
	int	floor_slope; // the angle the slope is 0-90?
	int	ceiling_slope;

	int		gravity;		// this is short int but in the real game a float (from 0.10 - 0.01)
	int		light_level;	// this is short int but in the real game a float (from 1.0 - 0.1)

	t_point			*first_point;
	Uint32			color;

	// TODO: remove lowest_pos and highest_pos in the end if they are not used.
	t_vector		lowest_pos; // these 2 vectors are so you can calculate...
	t_vector		highest_pos; // ...the middle of the sector.

	t_vector		center; // the center of the sector.
};

struct				s_entity
{
	int				id; // might be useless
	t_vector		pos;
	int			direction; // in degrees 
	t_entity_preset			*preset;

	int				statique; // if npc or static sprite
};

struct				s_entity_preset
{
	char			*name;
	int			mood;
	SDL_Surface		*texture;
};

/*
** Help
*/
void	add_to_list(t_list **list, void *new_v, size_t size);
void	free_array(char **arr);

#endif
