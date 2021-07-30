/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   portal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:44:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:44:26 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

int	tttt(t_sector *s2, t_wall *w, t_sector *s, int *found)
{
	t_list	*w2;

	w2 = s2->walls;
	while (w2)
	{
		if (w->neighbor_sector == s2
			&& ((t_wall *)w2->content)->neighbor_sector == s)
		{
			*found = 1;
			return (1);
		}
		w2 = w2->next;
	}
	*found = 0;
	return (0);
}

void	remove_all_non_existing_portals(t_list **sectors)
{
	t_list	*s;
	t_list	*w;
	t_list	*s2;
	int		found;

	found = 0;
	s = *sectors;
	while (s)
	{
		w = ((t_sector *)s->content)->walls;
		while (w)
		{
			s2 = *sectors;
			while (s2)
			{
				if (tttt(s2->content, w->content, s->content, &found))
					break ;
				s2 = s2->next;
			}
			if (found == 0)
				((t_wall *)w->content)->neighbor_sector = NULL;
			w = w->next;
		}
		s = s->next;
	}
}

void	check_if_we_can_make_portal(
		t_grid *grid, t_wall **other,
		t_sector **wall_1_sec, t_sector **wall_2_sec)
{
	t_list	*sec;
	t_list	*wall;

	sec = grid->sectors;
	while (sec)
	{
		wall = ((t_sector *)sec->content)->walls;
		while (wall)
		{
			if (wall_has_same_coords(grid->modify_wall, wall->content))
			{
				if (grid->modify_wall == wall->content)
					*wall_1_sec = sec->content;
				else
				{
					*other = wall->content;
					*wall_2_sec = sec->content;
				}
			}
			wall = wall->next;
		}
		sec = sec->next;
	}
}

void	add_portal(t_grid *grid)
{
	t_wall		*other;
	t_sector	*wall_1_sec;
	t_sector	*wall_2_sec;

	other = NULL;
	wall_1_sec = NULL;
	wall_2_sec = NULL;
	check_if_we_can_make_portal(grid, &other, &wall_1_sec, &wall_2_sec);
	if (wall_1_sec == NULL || wall_2_sec == NULL)
		return ;
	other->neighbor_sector = wall_1_sec;
	grid->modify_wall->neighbor_sector = wall_2_sec;
	if (other->neighbor_sector != NULL)
		other->solid = 0;
	if (grid->modify_wall->neighbor_sector != NULL)
		grid->modify_wall->solid = 0;
}

void	remove_portal(t_grid *grid)
{
	t_list	*s;
	t_list	*w;

	if (grid->modify_wall == NULL)
		return ;
	s = grid->sectors;
	while (s)
	{
		w = ((t_sector *)s->content)->walls;
		while (w)
		{
			if (w->content == grid->modify_wall)
			{
				((t_wall *)w->content)->neighbor_sector = NULL;
				((t_wall *)w->content)->solid = 1;
				remove_all_non_existing_portals(&grid->sectors);
			}
			w = w->next;
		}
		s = s->next;
	}
}
