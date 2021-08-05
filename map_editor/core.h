/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 16:49:18 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 16:49:19 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H
# define SDL_MAIN_HANDLED
# include "libgfx.h"
# include <time.h>
# define INT_MAX 2147483647
# define INT_MIN -2147483648

typedef struct s_sector			t_sector;
typedef struct s_event			t_event;
typedef struct s_wall			t_wall;
typedef struct s_point			t_point;
typedef struct s_entity			t_entity;
typedef struct s_entity_preset	t_entity_preset;

struct s_point
{
	int				id;
	t_vector		pos;
};

typedef struct s_sprite
{
	t_vector		pos;
	double			w;
	double			h;
	short int		sprite_id;
	float			real_x;
	float			real_y;
	t_xywh			coord;
	float			scale;
}					t_sprite;

/*
 * t_list	*sprites; list of t_sprite
*/
struct s_wall
{
	t_point			*orig;
	t_point			*dest;
	int				id;
	float			texture_scale;
	short int		texture_id;
	short int		portal_texture_id;
	int				solid;
	t_list			*sprites;
	int				neighbor;
	t_sector		*neighbor_sector;
};

/*
 * int				floor_slope_wall_id;	wall id from which the angle starts
 * int				floor_slope; 			in degrees
 * t_vector			center;					the center of the sector.
*/
struct s_sector
{
	int				id;
	t_list			*walls;
	int				floor_height;
	int				ceiling_height;
	int				floor_texture;
	int				ceiling_texture;
	float			floor_texture_scale;
	float			ceiling_texture_scale;
	int				floor_slope_wall_id;
	int				ceiling_slope_wall_id;
	int				floor_slope;
	int				ceiling_slope;
	int				gravity;
	int				light_level;
	t_point			*first_point;
	Uint32			color;
	t_vector		center;
};

/*
 * char				*type;		type of event. e_event_type (but its a char * in the map file)
 * char				*action;	how to trigger event. e_player_action  (but its a char * in the map file)
 * int				id;			id of the sector / wall sprite, the trigger is on
 * t_sector			*sector;	if sector trigger, this is the sector
 * int				min;		min value of the whatever type
 * int				max;		max value of the whatever type
 * int				speed;		speed of the event
*/
struct s_event
{
	char			*type;
	char			*action;
	int				id;
	t_sector		*sector;
	int				min;
	int				max;
	int				speed;
};

/*
 * int				direction; in degrees 
*/
struct s_entity
{
	int				id;
	t_vector		pos;
	int				direction;
	t_entity_preset	*preset;
};

struct				s_entity_preset
{
	char			*name;
	int				mood;
	SDL_Surface		*texture;
};

void				add_to_list(t_list **list, void *new_v, size_t size);
void				free_array(char **arr);
t_event				*new_event(void);
void				free_event(void *content, size_t size);

#endif
