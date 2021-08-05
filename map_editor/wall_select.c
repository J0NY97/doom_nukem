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

int	vector_in_rect_of_radius(t_vector p, t_vector v1, t_vector v2, float radius)
{
	float	min;
	float	max;

	min = fmin(v1.x, v2.x);
	max = fmax(v1.x, v2.x);
	if (p.x >= min - radius
		&& p.x <= max + radius)
	{
		min = fmin(v1.y, v2.y);
		max = fmax(v1.y, v2.y);
		if (p.y >= min - radius
			&& p.y <= max + radius)
			return (1);
	}
	return (0);
}

t_wall	*get_wall_around_radius_of_pos(
		t_editor *editor, float allowed_radius, t_vector pos)
{
	t_list		*curr;
	t_point		wall_orig;
	t_point		wall_dest;
	float		dist;

	curr = editor->grid.walls;
	while (curr)
	{
		wall_orig = (t_point){.pos = gfx_vector_multiply(
				((t_wall *)curr->content)->orig->pos, editor->grid.gap)};
		wall_dest = (t_point){.pos = gfx_vector_multiply(
				((t_wall *)curr->content)->dest->pos, editor->grid.gap)};
		dist = fabs(distance_from_vector_to_wall(pos,
					&(t_wall){.orig = &wall_orig, .dest = &wall_dest}));
		if (dist <= allowed_radius)
			if (vector_in_rect_of_radius(pos, wall_orig.pos,
					wall_dest.pos, allowed_radius))
				return (curr->content);
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

	wall = get_wall_around_radius_of_pos(editor, 10.0f,
			(t_vector){editor->libui->mouse_x - editor->grid.elem->position.x,
			editor->libui->mouse_y - editor->grid.elem->position.y, 0});
	if (wall)
	{
		editor->grid.modify_wall = wall;
		editor->grid.modify_sprite = NULL;
	}
}
