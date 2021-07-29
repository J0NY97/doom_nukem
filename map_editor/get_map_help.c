/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:39:05 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 13:39:06 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_point	*get_point_with_id(t_list *points, int id)
{
	t_list	*curr;

	curr = points;
	while (curr)
	{
		if (((t_point *)curr->content)->id == id)
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

t_wall	*get_wall_with_id(t_list *walls, int id)
{
	t_list	*curr;

	curr = walls;
	while (curr)
	{
		if (((t_wall *)curr->content)->id == id)
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

t_sector	*get_sector_with_id(t_list *sectors, int id)
{
	t_list	*curr;

	curr = sectors;
	while (curr)
	{
		if (((t_sector *)curr->content)->id == id)
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}
