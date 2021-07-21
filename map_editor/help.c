/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/19 17:02:16 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

Uint32	random_color(void)
{
	Uint32		color;

	color = rgba_to_hex((t_rgba) {
			.a = 255, .r = rand() % 255,
			.g = rand() % 255, .b = 245});
	return (color);
}

int	vector_is_empty(t_vector v)
{
	return (vector_compare(v, EMPTY_VEC));
}

int	pointer_compare(void *p1, void *p2)
{
	return (p1 == p2);
}

int	vector_compare(t_vector v1, t_vector v2)
{
	if ((int)v1.x == (int)v2.x &&
	(int)v1.y == (int)v2.y &&
	(int)v1.z == (int)v2.z)
		return (1);
	return (0);
}

int	wall_compare(void *v1, void *v2)
{
	if ((t_wall *)v1 == (t_wall *)v2)
		return (1);
	return (0);
}

float	get_wall_length(t_wall *wall)
{
	return (gfx_distance(wall->orig->pos, wall->dest->pos));
}

t_list	*get_nth_from_list(t_list **list, int index)
{
	int	count;
	t_list	*curr;

	curr = *list;
	count = 0;
	while (curr)
	{
		if (count == index)
			return (curr);
		count++;
		curr = curr->next;
	}
	return (NULL);
}

t_sprite	*new_sprite(void)
{
	t_sprite *sprite;

	sprite = malloc(sizeof(t_sprite));
	memset(sprite, 0, sizeof(t_sprite));
	sprite->sprite_id = 0;
	sprite->scale = 1.0f;
	sprite->real_x = 0;
	sprite->real_y = 0;
	sprite->coord.x = 0;
	sprite->coord.y = 0;
	sprite->coord.w = 0;
	sprite->coord.h = 0;
	return (sprite);
}

void	free_sprite(void *content, size_t size)
{
	ft_putstr("[free_sprite]\n");
	t_sprite *sprite;

	(void)size;
	if (content == NULL)
		return ;
	sprite = content;
	free(sprite);
}

t_point	*new_point(t_vector pos)
{
	t_point *point;

	point = malloc(sizeof(t_point));
	point->pos = pos;
	return (point);
}

t_wall	*new_wall(t_point *orig, t_point *dest)
{
	t_wall *new_wall;

	new_wall = ft_memalloc(sizeof(t_wall));
	new_wall->dest = dest;
	new_wall->orig = orig;
	new_wall->texture_scale = 1;
	new_wall->texture_id = 0;
	new_wall->portal_texture_id = 0;
	new_wall->solid = 1; 
	new_wall->portal = -1; 
	new_wall->sprites = NULL;
	new_wall->neighbor = -1;
	return (new_wall);
}

void	free_point(void *content, size_t size)
{
	t_point *point;

	point = content;
	if (point == NULL)
		return ;
	(void)size;
	ft_memdel(&content);
}

void	free_wall(void *content, size_t size)
{
	t_wall *wall;
   
	wall = content;
	(void)size;
	if (wall == NULL)
		return ;
	if (wall->sprites)
		ft_lstdel(&wall->sprites, &free_sprite);
	ft_memdel(&content);
}

t_sector	*new_sector(int id)
{
	t_sector *sector;

	sector = ft_memalloc(sizeof(t_sector));
	sector->id = id;
	sector->walls = NULL;
	sector->first_point = NULL;
	sector->floor_height = 0;
	sector->ceiling_height = 20;
	sector->floor_texture = 0;
	sector->ceiling_texture = 0;
	sector->floor_texture_scale = 1.0f;
	sector->ceiling_texture_scale = 1.0f;
	sector->gravity = 9;
	sector->light_level = 10;
	sector->lowest_pos = gfx_new_vector(INT_MAX, INT_MAX, INT_MAX);
	sector->highest_pos = gfx_new_vector(INT_MIN, INT_MIN, INT_MIN);
	sector->color = random_color();
	return (sector);
}

void	free_sector(void *content, size_t size)
{
	ft_putstr("[free_sector]\n");
	(void)size;
	if (content == NULL)
		return ;
	t_sector *sector = content;

	ft_putstr("Starting to free walls from sector.\n");
	if (sector->walls)
		ft_lstdel(&sector->walls, &dummy_free_er);
	ft_putstr("All walls from sector freed.\n");
	free(sector);
}

t_entity	*new_entity(int id, t_vector pos)
{
	t_entity *entity;

	entity = ft_memalloc(sizeof(t_entity));
	entity->id = id;
	entity->pos = pos;
	entity->direction = 90;
	entity->preset = NULL;
	return (entity);
}

void	free_entity(void *content, size_t size)
{
	(void)size;
	if (content == NULL)
		return ;
	t_entity *entity = content;
	ft_memdel((void **)&entity);
}

t_entity_preset	*new_entity_preset(char *name)
{
	t_entity_preset *preset;

	preset = ft_memalloc(sizeof(t_entity_preset));
	preset->name = ft_strdup(name);
	preset->mood = ENTITY_TYPE_NEUTRAL;
	preset->texture = NULL;
	return (preset);
}

void	free_entity_preset(void *content, size_t size)
{
	(void)size;
	t_entity_preset *ent;
   
	ent = content;
	if (ent == NULL)
		return ;
	ft_strdel(&ent->name);
	if (ent->texture)
		SDL_FreeSurface(ent->texture);
	ft_memdel((void **)&ent);
}

// TODO: Get from list if function.
t_entity_preset	*get_entity_preset_with_name(t_list *list, char *name)
{
	t_list *curr;
	t_entity_preset *preset;

	curr = list;
	while (curr)
	{
		preset = curr->content;
		if (ft_strcmp(preset->name, name) == 0) 
			return (preset);
		curr = curr->next;
	}
	return (NULL);
}

t_entity	*get_entity_from_list_at_pos(t_list *list, t_vector pos)
{
	t_list *curr;

	curr = list;
	while (curr)
	{
		if (vector_compare(pos, ((t_entity *)curr->content)->pos))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

void	remove_from_points(t_list **points, t_point *v)
{
	remove_from_list_if_with(points, v, &pointer_compare, &free_point);
}

void	remove_from_walls(t_list **walls, t_wall *wall)
{
	remove_from_list_if_with(walls, wall, &pointer_compare, &free_wall);
}

void	remove_from_walls_non_free(t_list **walls, t_wall *wall)
{
	t_list *curr;
	t_list *prev;

	curr = *walls;
	if (curr == NULL)
		return ;
	if (curr->content == wall)
	{
		*walls = curr->next;
		free(curr);
	}
	else
	{
		while (curr)
		{
			if (curr->content == wall)
			{
				prev->next = curr->next;
				free(curr);
			}
			else
				prev = curr;
			curr = prev->next;
		}
	}
}

int	sprite_compare(t_sprite *bubble, t_sprite *gum)
{
	if (bubble->sprite_id == gum->sprite_id &&
	bubble->coord.x == gum->coord.x &&
	bubble->coord.y == gum->coord.y &&
	bubble->coord.w == gum->coord.w &&
	bubble->coord.h == gum->coord.h &&
	bubble->scale == gum->scale)
		return (1);
	return (0);

}

void	remove_from_list_if_with(t_list **list, void *s, int (*cmp)(void *, void *), void (*del)(void *, size_t))
{
	t_list *curr;
	t_list *prev;

	curr = *list;
	if (curr == NULL)
		return ;
	if (cmp(curr->content, s))
	{
		*list = curr->next;
		ft_lstdelone(&curr, del);
	}
	else
	{
		while (curr)
		{
			if (cmp(curr->content, s))
			{
				prev->next = curr->next;
				ft_lstdelone(&curr, del);
			}
			else
				prev = curr;
			curr = prev->next;
		}
	}
}

void	remove_from_sprites(t_list **list, t_sprite *s)
{
	remove_from_list_if_with(list, s, &pointer_compare, &free_sprite);
}


t_sprite	*get_sprite_from_list(t_list **list, int x, int y)
{
	t_list *curr;
	t_sprite *sprite;
	int i; // NOTE: this i looks to only be used in the printf

	i = 0;
	curr = *list;

	ft_printf("Searching for sprite on coords: %d %d\n", x, y);
	while (curr)
	{
		sprite = curr->content;
		ft_printf("%d: %d %d %d %d\n", i, sprite->coord.x, sprite->coord.y, sprite->coord.w, sprite->coord.h);
		if (sprite->coord.x <= x &&
		sprite->coord.x + sprite->coord.w >= x &&
		sprite->coord.y <= y &&
		sprite->coord.y + sprite->coord.h >= y)
		{
			return (curr->content);
		}
		curr = curr->next;
		i++;
	}
	return (NULL);
}

void   remove_wall_from_its_sector(t_grid *grid, t_wall *wall)
{
	t_list  *curr_sec;
	t_list  *curr_wall;
	t_list  *prev_wall = NULL;

	curr_sec = grid->sectors;
	while (curr_sec)
	{
		curr_wall = ((t_sector *)curr_sec->content)->walls;
		if (wall_compare(curr_wall->content, wall))
	    {
			ft_printf("Wall removed from its sector.\n");
			((t_sector *)curr_sec->content)->walls = curr_wall->next;
	    	free(curr_wall);
			return ;
	    }
	    else
	    {
			while (curr_wall)
			{
				if (wall_compare(curr_wall->content, wall))
				{
					ft_printf("Wall removed from its sector.\n");
					prev_wall->next = curr_wall->next;
					free(curr_wall);
					return ;
				}
				else
					prev_wall = curr_wall;
				curr_wall = prev_wall->next;
			}
		}
		curr_sec = curr_sec->next;
	}
 }

// this is the only thign that can be compared with id
int		sector_compare(t_sector *s1, t_sector *s2)
{
	if (s1->id == s2->id)
		return (1);
	return (0);
}

void	remove_from_sectors(t_list **sectors, t_sector *sec)
{
	t_list *curr;
	t_list *prev;

	curr = *sectors;
	if (curr && sector_compare(curr->content, sec))
	{
		*sectors = curr->next;
		free_sector(curr->content, 0);
		free(curr);
		return ;
	}
	else
	{
		while (curr)
		{
			if (sector_compare(curr->content, sec))
			{
				prev->next = curr->next;
				free_sector(curr->content, 0);
				free(curr);
				return ;
			}
			else
				prev = curr;
			curr = prev->next;
		}
	}
}

void	remove_everything_from_list(t_list **list)
{
	t_list *curr;
	t_list *prev;

	curr = *list;
	while (curr)
	{
		prev = curr->next;
		free(curr);
		curr = prev;
	}
	*list = NULL;
ft_printf("Everything removed from list.\n");
}

void	remove_all_points_not_a_part_of_a_wall(t_list **points, t_list **walls)
{
	t_list *point;
	t_list *wall;
	int		found;

	point = *points;
	while (point)
	{
		found = 0;
		wall = *walls;
		while (wall)
		{
			if (vector_in_wall(((t_point *)point->content)->pos, wall->content))
				found = 1;
			wall = wall->next;
		}
		if (found == 0)
			remove_from_points(points, point->content);
		point = point->next;
	}
}

void			remove_all_non_existing_portals(t_list **sectors)
{
	t_list *s;
	t_list *w;
	t_list *s2;
	int found;

	s = *sectors;
	while (s)
	{
		w = ((t_sector *)s->content)->walls;
		while (w)
		{
			found = 0;
			s2 = *sectors;
			while (s2)
			{
				if (((t_wall *)w->content)->neighbor == (int)((t_sector *)s2->content)->id)
					found = 1;
				s2 = s2->next;
			}
			if (found == 0)
				((t_wall *)w->content)->neighbor = -1;
			w = w->next;
		}
		s = s->next;
	}
}

int				wall_in_list(t_wall *wall, t_list *list)
{
	t_list *curr;

	curr = list;
	while (curr)
	{
		if (wall_compare(wall, curr->content))
			return (1);
		curr = curr->next;
	}
	return (0);
}

int				wall_in_sector(t_wall *wall, t_sector *sector)
{
	return (wall_in_list(wall, sector->walls));
	/* OLD, if the code on top of this doesnt work, uncomment.
	t_list *curr;

	curr = sector->walls;
	while (curr)
	{
		if (wall_compare(wall, curr->content))
			return (1);
		curr = curr->next;
	}
	return (0);
	*/
}

/*
 ** All walls not a part of a sector.
*/
void	remove_all_lonely_walls(t_list **walls, t_list **sectors)
{
	t_list	*wall;
	t_list	*sec;
	int	found;

	wall = *walls;
	while (wall)
	{
		found = 0;
		sec = *sectors;
		while (sec)
		{
			if (wall_in_sector(wall->content, sec->content))
				found = 1;
			sec = sec->next;
		}
		if (found == 0)
			remove_from_walls(walls, wall->content);
		wall = wall->next;
	}
}

/* This might not be needed. */
// NOTE: This spaghett has to be added so that niklas renderer can work. 
//
// 1. loop through all sectors walls
// 2. save either wall, and loop through all the walls again and search for a wall with common vertex. that is not itself 
// 3. when found, check that if its the same vertex aka v1 == v1 or v2 == v2, if not switcheroo.
// 4. remove all the old walls and set sector - walls to the new lst.
//
// NOTE: we always check wall->dest
void	sort_sector_wall_list(t_sector *sector)
{
	t_list *sorted_list = NULL;
	t_list *rucc;
	t_wall *wall;

	if (!sector)
		return ;
	ft_putstr("didnt return1");
	if (sector->walls == NULL)
		return ;
	ft_putstr("didnt return2");
	wall = sector->walls->content;
	add_to_list(&sorted_list, wall, sizeof(t_wall));		
	rucc = sector->walls;
	while (rucc)
	{
		if ((vector_compare(wall->dest->pos, ((t_wall *)rucc->content)->orig->pos) || vector_compare(wall->dest->pos, ((t_wall *)rucc->content)->dest->pos)) &&
			!wall_compare(wall, rucc->content)) 
		{
			if (vector_compare(wall->dest->pos, ((t_wall *)rucc->content)->dest->pos))
			{
				t_point *temp = ((t_wall *)rucc->content)->dest;
				((t_wall *)rucc->content)->dest = ((t_wall *)rucc->content)->orig;
				((t_wall *)rucc->content)->orig = temp;
			}
			wall = rucc->content;
			add_to_list(&sorted_list, wall, sizeof(t_wall));
			if (vector_compare(((t_wall *)sector->walls->content)->orig->pos, wall->dest->pos))
				break ;
			rucc = sector->walls;
		}
		else
			rucc = rucc->next;
	}

	ft_lstdel(&sector->walls, &dummy_free_er);
	sector->walls = sorted_list;

	ft_putstr("We are done sorting the sector.\n");
}

t_sector	*get_sector_with_wall(t_list **sector_list, t_wall *wall)
{
	t_list *curr;
	t_list *rruc;

	curr = *sector_list;
	while (curr)
	{
		rruc = ((t_sector *)curr->content)->walls;
		while (rruc)
		{
			if (wall_compare(rruc->content, wall))
				return (curr->content);
			rruc = rruc->next;
		}
		curr = curr->next;
	}
	// Weird that there is a wall that isnt a part of a sector.... so if we get here, look into it.
	return (NULL);
}
