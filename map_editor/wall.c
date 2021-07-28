/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:44:47 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:44:48 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
