/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2020/09/27 16:30:53 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

char	*set_spawn(t_editor *doom)
{
	char	*str;
	char	*temp;
	t_vector player;

	player = doom->spawn.pos;
	str = ft_sprintf("type:spawn\tx\ty\tz\n");
	temp = ft_sprintf("\t%.1f\t%.1f\t%.1f\n", player.x, player.y, player.z);
	ft_stradd(&str, temp);
	ft_strdel(&temp);
	return (str);
}

char	*set_point(t_editor *doom)
{
	t_list *v;
	t_point	*point;
	t_vector vec;
	char *str;
	char *temp;
	int id;

	v = doom->grid.points;
	id = 0;
	str = ft_sprintf("type:vertex\tid\tx\ty\tz\n");
	while (v)
	{
		point = v->content;
		point->id = id;
		vec = point->pos;
		temp = ft_sprintf("\t\t%d\t%.1f\t%.1f\t%.1f\n", point->id, vec.x,	vec.y, vec.z);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		point->id = id;
		id++;
		v = v->next;
	}
	return (str);
}

char	*set_sprite(t_editor *doom)
{
	t_list *curr_wall;
	t_list *curr_sprite;
	t_wall *wall;
	t_sprite *sprite;
	char *temp = NULL;
	char *str = ft_sprintf("type:wall_sprite\tid\twall_id\tx\ty\ttex\tscale\n");
	int id = 0;
	int wall_id = 0;

	curr_wall = doom->grid.walls;
	while (curr_wall)
	{
		wall = curr_wall->content;
		curr_sprite = wall->sprites;
		while (curr_sprite)
		{
			sprite = curr_sprite->content;
			// NOTE: if the wall id is not matching to the correct wall, then you have to rethink this,
			// 	but it should because youre looping in the same order as the set_wall function comes up with ids.
			temp = ft_sprintf("\t%d\t%d\t%d\t%d\t%d\t%.3f\n", id, wall_id, sprite->coord.x, sprite->coord.y, sprite->sprite_id, sprite->scale);
			// maybe remove this and just add it in the printf, actually no idea if its even faster
			ft_stradd(&str, temp);
			ft_strdel(&temp);
			id++;
			curr_sprite = curr_sprite->next;
		}
		wall_id++;
		curr_wall = curr_wall->next;
	}
	return (str);
}

char	*set_wall(t_editor *doom)
{
	t_list *v;
	char *str;
	char *temp;
	t_wall *wall;
	int id;

	v = doom->grid.walls;
	id = 0;
	str = ft_sprintf("type:wall\tid\tv1\tv2\twalltex\tportaltex\tscale\tsolid\n");
	while (v)
	{
		wall = v->content;
		wall->id = id;

		// default the soliditiy of the wall if not set (-1 means its not set (if youre curious))
		if (wall->solid == -1)
			wall->solid = wall->neighbor == -1;

		temp = ft_sprintf("\t\t%d\t%d\t%d\t%d\t%d\t\t%.1f\t%d\n", wall->id, wall->orig->id, wall->dest->id, wall->texture_id, wall->portal_texture_id, wall->texture_scale, wall->solid);
		ft_stradd(&str, temp);
		ft_strdel(&temp);

		id++;
		v = v->next;
	}
	return (str);
}

char	*set_sector(t_editor *doom)
{
	t_list *s;
	char *str;
	char *temp;
	t_sector *sec;
	t_list	*wall;
	t_wall *w;
	int id;

	s = doom->grid.sectors;
	id = 0;
	str = ft_sprintf("type:sector\tid\tfloor\troof\twall_id\tneighbors\tfloor_texture\troof_texture\tgravity\tlight\n");
	while (s)
	{
		sec = s->content;

		temp = ft_sprintf("\t\t%d\t%d\t%d\t", sec->id, sec->floor_height, sec->ceiling_height);
		ft_stradd(&str, temp);
		ft_strdel(&temp);

		wall = sec->walls;
		char *temp_wall;
		char *temp_neighbor;
		char *temp_w;
		char *temp_n;
		temp_w = NULL;
		temp_n = NULL;
		while (wall)
		{
			w = wall->content;
			if (temp_w == NULL)
				temp_wall = ft_sprintf("%d", w->id);
			else
				temp_wall = ft_sprintf("%d %s", w->id, temp_w);
			if (temp_n == NULL)
				temp_neighbor = ft_sprintf("%d", w->neighbor);
			else
				temp_neighbor = ft_sprintf("%d %s", w->neighbor, temp_n);
			ft_strdel(&temp_w);
			ft_strdel(&temp_n);
			temp_w = ft_strdup(temp_wall);
			temp_n = ft_strdup(temp_neighbor);
			ft_strdel(&temp_wall);
			ft_strdel(&temp_neighbor);
			wall = wall->next;
		}
		ft_stradd(&str, temp_w);
		temp = ft_sprintf("\t%s", temp_n);
		ft_stradd(&str, temp);
		ft_strdel(&temp_w);
		ft_strdel(&temp_n);
		ft_strdel(&temp);

		temp = ft_sprintf("\t%d\t%d\t%d\t%d\n", sec->floor_texture, sec->ceiling_texture, sec->gravity, sec->light_level);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		s = s->next;
		id++;
	}
	return (str);
}

char	*set_entities(t_editor *doom)
{
	t_list *e;
	t_entity *ent;
	char *temp;
	char *str;

	e = doom->grid.entities;
	str = ft_sprintf("type:entity\tid\tx\ty\tz\tname\tsprite_id\thealth\tspeed\tdirection\n");
	while (e)
	{
		ent = e->content;
		gfx_vector_string(ent->pos);
		temp = ft_sprintf("\t\t%d\t%.1f\t%.1f\t%.1f\t%s\t%d\t%d\t%d\t%d\n", ent->id, ent->pos.x, ent->pos.y, ent->pos.z, ent->preset->name, ent->sprite_id, ent->preset->health, ent->preset->speed, ent->direction);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		e = e->next;
	}
	return (str);
}

void	set_map(t_editor *doom)
{
	char	*str;
	int		fd;

	char	*divider;
	char	*map;
	char	*spawn;
	char	*point;
	char	*wall;
	char	*sprite;
	char	*sector;
	char	*entity;

	ft_putstr("Saving file.\n");

	//just to be sure you have the correct amount of things.
	// remove when niklas decides to fix his code.
	recount_everything(doom);

	divider = ft_strdup("-----------------------------------\n");
	map = ft_sprintf("type:map\tname\tscale\tvert\twall\twallsprite\tsec\tent\n\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n", doom->filename, 1, doom->grid.point_amount, doom->grid.wall_amount, doom->grid.wall_sprite_amount, doom->grid.sector_amount, doom->grid.entity_amount);
	spawn = set_spawn(doom);
	point = set_point(doom);
	wall = set_wall(doom);
	sprite = set_sprite(doom);
	sector = set_sector(doom);
	entity = set_entities(doom);
	// str = ft_sprintf("%s%s%s%s%s%s%s%s%s%s%s%s", map, divider, spawn, divider, point, divider, wall, divider, sector, divider, entity, divider);
	str = ft_strjoiner(map, divider, spawn, divider, point, divider, wall, divider, sprite, divider, sector, divider, entity, divider, NULL);

	fd = creat(doom->filename, S_IRUSR | S_IWUSR);
	if (fd > -1)
		ft_fprintf(fd, "%s", str);
	else
		ft_printf("Something went wrong with saving the file.\n");
	close(fd);

	ft_printf("%s", str);

	ft_strdel(&divider);
	ft_strdel(&map);
	ft_strdel(&spawn);
	ft_strdel(&point);
	ft_strdel(&wall);
	ft_strdel(&sprite);
	ft_strdel(&sector);
	ft_strdel(&entity);
	ft_strdel(&str);
}
