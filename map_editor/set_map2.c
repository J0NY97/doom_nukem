/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:44:43 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 13:44:44 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

char	*set_point(t_editor *doom)
{
	int		id;
	char	*str;
	char	*temp;
	t_list	*curr;
	t_point	*point;

	id = 0;
	curr = doom->grid.points;
	str = ft_sprintf("type:vertex\tid\tx\ty\tz\n");
	while (curr)
	{
		point = curr->content;
		point->id = id;
		temp = ft_sprintf("%d\t%.1f\t%.1f\t%.1f\n",
				point->id, point->pos.x, point->pos.y, point->pos.z);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		point->id = id;
		id++;
		curr = curr->next;
	}
	return (str);
}

char	*set_all_sprites_for_wall(t_wall *wall, int wall_id, int *id)
{
	t_list		*curr_sprite;
	t_sprite	*sprite;
	char		*temp;
	char		*str;

	curr_sprite = wall->sprites;
	str = ft_strnew(0);
	while (curr_sprite)
	{
		sprite = curr_sprite->content;
		temp = ft_sprintf("%d\t%d\t%.3f\t%.3f\t%d\t%.3f\tSTATIC\n",
				*id, wall_id, sprite->real_x, sprite->real_y,
				sprite->sprite_id, sprite->scale);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		curr_sprite = curr_sprite->next;
		*id += 1;
	}
	return (str);
}

char	*set_sprite(t_editor *editor)
{
	t_list	*curr_wall;
	char	*str;
	char	*temp;
	int		wall_id;
	int		id;

	wall_id = 0;
	id = 0;
	str = ft_sprintf("type:wsprite\tid\twall_id\tx\ty\ttex\tscale\n");
	curr_wall = editor->grid.walls;
	while (curr_wall)
	{
		temp = set_all_sprites_for_wall(curr_wall->content, wall_id, &id);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		wall_id++;
		curr_wall = curr_wall->next;
	}
	return (str);
}

char	*set_wall(t_editor *doom)
{
	t_list	*curr;
	t_wall	*wall;
	char	*str;
	char	*temp;
	int		id;

	id = 0;
	str = ft_sprintf("type:wall\tid\tv1\tv2\twalltex"\
		"\tportaltex\tscale\tsolid\n");
	curr = doom->grid.walls;
	while (curr)
	{
		wall = curr->content;
		wall->id = id;
		temp = ft_sprintf("%d\t%d\t%d\t%d\t%d\t\t%.1f\t%d\n",
				wall->id, wall->orig->id, wall->dest->id,
				wall->texture_id, wall->portal_texture_id,
				wall->texture_scale, wall->solid);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		id++;
		curr = curr->next;
	}
	return (str);
}

char	*set_entities(t_editor *doom)
{
	char		*temp;
	char		*str;
	t_list		*curr;
	t_entity	*ent;

	curr = doom->grid.entities;
	str = ft_sprintf("type:entity\tid\tname\tx\ty\tz\tdirection\n");
	while (curr)
	{
		ent = curr->content;
		gfx_vector_string(ent->pos);
		temp = ft_sprintf("%d\t%s\t%.1f\t%.1f\t%.1f\t%d\n",
				ent->id, ent->preset->name, ent->pos.x, ent->pos.y,
				ent->pos.z, ent->direction);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (str);
}
