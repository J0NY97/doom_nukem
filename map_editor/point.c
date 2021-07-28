/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:44:19 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 14:24:33 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_point	*new_point(t_vector pos)
{
	t_point	*point;

	point = malloc(sizeof(t_point));
	point->pos = pos;
	return (point);
}

void	free_point(void *content, size_t size)
{
	t_point	*point;

	point = content;
	if (point == NULL)
		return ;
	(void)size;
	ft_memdel(&content);
}

void	remove_selected_point_from_all_walls_and_sectors(t_editor *editor)
{
	t_list	*wall;
	t_list	*sec;

	wall = editor->grid.walls;
	while (wall)
	{
		if (vector_in_wall(editor->grid.modify_point->pos, wall->content))
		{
			sec = editor->grid.sectors;
			while (sec)
			{
				remove_from_list_if_with(&((t_sector *)sec->content)->walls,
					wall->content, &pointer_compare, &dummy_free_er);
				sec = sec->next;
			}
			remove_from_list_if_with(&editor->grid.walls, wall->content,
				&pointer_compare, &free_wall);
			wall = editor->grid.walls;
		}
		else
			wall = wall->next;
	}
	remove_from_list_if_with(&editor->grid.points, editor->grid.modify_point,
		&pointer_compare, &free_point);
}

t_point	*get_point_from_list_at_pos(t_list *list, t_vector v)
{
	t_list	*curr;

	curr = list;
	while (curr)
	{
		if (vector_compare(((t_point *)curr->content)->pos, v))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

void	draw_points(t_grid *grid, t_list *points)
{
	t_list	*curr;

	curr = points;
	while (curr)
	{
		gfx_draw_vector(grid->elem->active_surface, 0xff00ff00, 1,
			gfx_vector_multiply(((t_point *)curr->content)->pos,
				grid->gap));
		curr = curr->next;
	}
}
