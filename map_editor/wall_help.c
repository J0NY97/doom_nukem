/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:44:50 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:44:51 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

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
			remove_from_list_if_with(walls, wall->content,
				&pointer_compare, &free_wall);
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
