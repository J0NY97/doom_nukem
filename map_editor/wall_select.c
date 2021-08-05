/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_select.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:43:07 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:43:09 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

float	distance_from_vector_to_wall(t_vector p0, t_wall *wall)
{
	t_vector	p1;
	t_vector	p2;
	float		dist;
	float		up;
	float		down;

	p1 = wall->orig->pos;
	p2 = wall->dest->pos;
	up = (p2.x - p1.x) * (p1.y - p0.y)
		- (p1.x - p0.x) * (p2.y - p1.y);
	down = sqrt(ft_pow(p2.x - p1.x, 2) + ft_pow(p2.y - p1.y, 2));
	dist = up / down;
	return (dist);
}

t_wall	*get_wall_from_list_around_radius(
		t_list *walls, t_vector pos, float allowed_radius)
{
	t_list		*curr;
	t_wall		*wall;
	float		dist;

	curr = walls;
	while (curr)
	{
		wall = curr->content;
		dist = fabs(distance_from_vector_to_wall(pos, curr->content));
		if (dist <= allowed_radius)
		{
			if (pos.x >= fmin(wall->orig->pos.x, wall->dest->pos.x)
				- allowed_radius && pos.x <= fmax(wall->orig->pos.x,
					wall->dest->pos.x) + allowed_radius)
			{
				if (pos.y >= fmin(wall->orig->pos.y,
						wall->dest->pos.y) - allowed_radius && pos.y
					<= fmax(wall->orig->pos.y,
						wall->dest->pos.y) + allowed_radius)
					return (curr->content);
			}
		}
		curr = curr->next;
	}
	return (NULL);
}

t_wall	*get_wall_around_mouse(t_editor *editor)
{
	t_list		*curr;
	t_wall		*wall;
	t_wall		*w;
	t_vector	pos;
	t_vector	wall_orig;
	t_vector	wall_dest;
	float		dist;
	float		allowed_radius = 10.0f;

	pos = gfx_new_vector(editor->grid.elem->libui->mouse_x - editor->grid.elem->position.x,
			editor->grid.elem->libui->mouse_y - editor->grid.elem->position.y, 0);
	curr = editor->grid.walls;
	while (curr)
	{
		w = curr->content;
		wall = new_wall(&(t_point){.pos = gfx_vector_multiply(w->orig->pos, editor->grid.gap)},
				&(t_point){.pos = gfx_vector_multiply(w->dest->pos, editor->grid.gap)});
		dist = fabs(distance_from_vector_to_wall(pos, wall));
		if (dist <= allowed_radius)
		{
			if (pos.x >= fmin(wall->orig->pos.x, wall->dest->pos.x)
				- allowed_radius && pos.x <= fmax(wall->orig->pos.x,
					wall->dest->pos.x) + allowed_radius)
			{
				if (pos.y >= fmin(wall->orig->pos.y,
						wall->dest->pos.y) - allowed_radius && pos.y
					<= fmax(wall->orig->pos.y,
						wall->dest->pos.y) + allowed_radius)
					return (curr->content);
			}
		}
		curr = curr->next;
	}
	return (NULL);
}


void	draw_selected_wall(t_grid *grid)
{
	if (grid->modify_wall == NULL)
		return ;
	draw_wall_as_selected(grid,
		grid->elem->active_surface, grid->modify_wall);
}

void	select_wall(t_editor *editor)
{
	t_wall	*wall;

	wall = get_wall_around_mouse(editor);
	if (wall)
	{
		editor->grid.modify_wall = wall;
		editor->grid.modify_sprite = NULL;
	}
}
