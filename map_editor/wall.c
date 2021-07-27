#include "editor.h"

t_wall	*new_wall(t_point *orig, t_point *dest)
{
	t_wall	*new_wall;

	new_wall = ft_memalloc(sizeof(t_wall));
	new_wall->dest = dest;
	new_wall->orig = orig;
	new_wall->texture_scale = 1;
	new_wall->texture_id = 0;
	new_wall->portal_texture_id = 0;
	new_wall->solid = 1;
	new_wall->sprites = NULL;
	new_wall->neighbor = -1;
	new_wall->neighbor_sector = NULL;
	return (new_wall);
}

void	free_wall(void *content, size_t size)
{
	t_wall	*wall;

	wall = content;
	(void)size;
	if (wall == NULL)
		return ;
	if (wall->sprites)
		ft_lstdel(&wall->sprites, &free_sprite);
	ft_memdel(&content);
}

/*
 ** All walls not a part of a sector.
*/
void	remove_all_lonely_walls(t_list **walls, t_list **sectors)
{
	t_list	*wall;
	t_list	*sec;
	int		found;

	wall = *walls;
	while (wall)
	{
		found = 0;
		sec = *sectors;
		while (sec)
		{
			if (in_list(wall->content, ((t_sector *)sec->content)->walls))
				found = 1;
			sec = sec->next;
		}
		if (found == 0)
			remove_from_walls(walls, wall->content);
		wall = wall->next;
	}
}

void	remove_wall_from_its_sector(t_grid *grid, t_wall *wall)
{
	t_list	*curr_sec;

	curr_sec = grid->sectors;
	while (curr_sec)
	{
		remove_from_list_if_with(&((t_sector *)curr_sec->content)->walls, wall,
			&pointer_compare, &dummy_free_er);
		curr_sec = curr_sec->next;
	}
}

void	remove_from_walls(t_list **walls, t_wall *wall)
{
	remove_from_list_if_with(walls, wall, &pointer_compare, &free_wall);
}

float	get_wall_length(t_wall *wall)
{
	return (gfx_distance(wall->orig->pos, wall->dest->pos));
}

int	wall_has_same_coords(t_wall *w1, t_wall *w2)
{
	if ((vector_compare(w1->orig->pos, w2->orig->pos)
			&& vector_compare(w1->dest->pos, w2->dest->pos))
		|| (vector_compare(w1->orig->pos, w2->dest->pos)
			&& vector_compare(w1->dest->pos, w2->orig->pos)))
		return (1);
	return (0);
}

t_wall	*get_wall_from_list(t_list **list, t_point *v1, t_point *v2)
{
	t_list	*curr;
	t_wall	*wall;

	curr = *list;
	while (curr)
	{
		wall = curr->content;
		if ((vector_compare(wall->orig->pos, v1->pos)
				&& vector_compare(wall->dest->pos, v2->pos))
			|| (vector_compare(wall->orig->pos, v2->pos)
				&& vector_compare(wall->dest->pos, v1->pos)))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

void	draw_wall(t_wall *wall, t_grid *grid, Uint32 color)
{
	t_vector	orig_vec;
	t_vector	dest_vec;

	orig_vec = gfx_vector_multiply(wall->orig->pos, grid->gap);
	dest_vec = gfx_vector_multiply(wall->dest->pos, grid->gap);
	gfx_draw_line(grid->elem->active_surface, color, orig_vec, dest_vec);
}

void	draw_walls(t_grid *grid, t_list **walls, Uint32 color)
{
	t_list	*curr;

	curr = *walls;
	while (curr)
	{
		gfx_draw_line(grid->elem->active_surface, color,
			((t_wall *)curr->content)->orig->pos,
			((t_wall *)curr->content)->dest->pos);
		curr = curr->next;
	}
}
