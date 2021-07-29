/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_crew.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:19:40 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 14:23:14 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	remove_all_lonely_points(t_editor *editor)
{
	t_list	*p;
	t_list	*wall;
	int		found;

	p = editor->grid.points;
	while (p)
	{
		found = 0;
		wall = editor->grid.walls;
		while (wall)
		{
			if (p->content == ((t_wall *)wall->content)->orig
				|| p->content == ((t_wall *)wall->content)->dest)
			{
				found = 1;
				break ;
			}
			wall = wall->next;
		}
		if (found == 0)
			remove_from_list_if_with(&editor->grid.points, p->content,
				&pointer_compare, &free_point);
		p = p->next;
	}
}

void	remove_all_non_existing_sectors(t_editor *editor)
{
	t_list		*sec;
	t_sector	*sector;

	sec = editor->grid.sectors;
	while (sec)
	{
		sector = sec->content;
		sec = sec->next;
		if (sector && ft_lstlen(sector->walls) == 0)
			remove_from_list_if_with(&editor->grid.sectors, sector,
				&pointer_compare, &free_sector);
	}
}

t_sector	*get_sector_with_wall(t_list **sector_list, t_wall *wall)
{
	t_list	*curr;
	t_list	*rruc;

	curr = *sector_list;
	while (curr)
	{
		rruc = ((t_sector *)curr->content)->walls;
		while (rruc)
		{
			if (rruc->content == wall)
				return (curr->content);
			rruc = rruc->next;
		}
		curr = curr->next;
	}
	return (NULL);
}
