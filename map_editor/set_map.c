/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/02 12:10:33 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

char	*set_spawn(t_editor *doom)
{
	char		*str;
	char		*temp;
	t_vector	pos;

	pos = doom->spawn.pos;
	str = ft_sprintf("type:spawn\tx\ty\tz\tdir\n");
	temp = ft_sprintf("0\t%.1f\t%.1f\t%.1f\t%d\n",
		pos.x, pos.y, pos.z, doom->spawn.direction);
	ft_stradd(&str, temp);
	ft_strdel(&temp);
	return (str);
}

char	*set_point(t_editor *doom)
{
	int		id;
	char		*str;
	char		*temp;
	t_list		*curr;
	t_point		*point;

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

char	*set_all_sprites_for_wall(t_wall *wall, int wall_id)
{
	t_list		*curr_sprite;
	t_sprite	*sprite;
	char		*temp;
	char		*str;
	int		id;

	id = 0;
	curr_sprite = wall->sprites;
	str = ft_strnew(0);
	while (curr_sprite)
	{
		sprite = curr_sprite->content;
		temp = ft_sprintf("%d\t%d\t%.3f\t%.3f\t%d\t%.3f\n",
			id, wall_id, sprite->real_x, sprite->real_y,
			sprite->sprite_id, sprite->scale);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		curr_sprite = curr_sprite->next;
		id++;
	}
	return (str);
}

char	*set_sprite(t_editor *editor)
{
	t_list		*curr_wall;
	char		*str;
	char		*temp;
	int		wall_id;

	wall_id = 0;
	str = ft_sprintf("type:wsprite\tid\twall_id\tx\ty\ttex\tscale\n");
	curr_wall = editor->grid.walls;
	while (curr_wall)
	{
		temp = set_all_sprites_for_wall(curr_wall->content, wall_id);
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
	int	id;

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

char	*set_fandc(t_editor *editor)
{
	char		*str;
	char		*temp;
	t_list		*curr;
	t_sector	*sec;

	str = ft_sprintf("type:f&c\tid\tf_height\tc_height\tf_tex\tc_tex\t"\
		"f_scale\tc_scale\tslope\n");
	curr = editor->grid.sectors;
	while (curr)
	{
		sec = curr->content;
		temp = ft_sprintf("%d\t%d\t%d\t%d\t%d\t%.1f\t%.1f\t%d %d %d"\
			" %d\n", sec->id, sec->floor_height,
			sec->ceiling_height, sec->floor_texture,
			sec->ceiling_texture, sec->floor_texture_scale,
			sec->ceiling_texture_scale, sec->floor_slope_wall_id,
			sec->floor_slope, sec->ceiling_slope_wall_id,
			sec->ceiling_slope);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (str);
}

char	*set_walls_and_neighbors_for_sector(t_list *wall_list)
{
	char	*temp_wall;
	char	*temp_neighbor;
	char	*temp_w;
	char	*temp_n;
	char	*temp;
	char	*str;
	t_wall	*w;

	temp_w = NULL;
	temp_n = NULL;
	str = ft_strnew(0);
	while (wall_list)
	{
		w = wall_list->content;
		if (temp_w == NULL)
			temp_wall = ft_sprintf("%d", w->id);
		else
			temp_wall = ft_sprintf("%d %s", w->id, temp_w);
		if (temp_n == NULL)
		{
			if (w->neighbor_sector)
				temp_neighbor = ft_sprintf("%d", w->neighbor_sector->id);
			else
				temp_neighbor = ft_sprintf("%d", -1);
			//temp_neighbor = ft_sprintf("%d", w->neighbor);
		}
		else
		{
			if (w->neighbor_sector)
				temp_neighbor = ft_sprintf("%d %s", w->neighbor_sector->id, temp_n);
			else
				temp_neighbor = ft_sprintf("%d %s", -1, temp_n);
		//	temp_neighbor = ft_sprintf("%d %s", w->neighbor, temp_n);
		}
		ft_strdel(&temp_w);
		ft_strdel(&temp_n);
		temp_w = ft_strdup(temp_wall);
		temp_n = ft_strdup(temp_neighbor);
		ft_strdel(&temp_wall);
		ft_strdel(&temp_neighbor);
		wall_list = wall_list->next;
	}
	ft_stradd(&str, temp_w);
	temp = ft_sprintf("\t%s", temp_n);
	ft_stradd(&str, temp);
	ft_strdel(&temp_w);
	ft_strdel(&temp_n);
	ft_strdel(&temp);
	return (str);
}


char	*set_sector(t_editor *doom)
{
	t_list		*curr;
	char		*str;
	char		*temp;
	t_sector	*sec;

	curr = doom->grid.sectors;
	str = ft_sprintf("type:sector\tid\twall_id\tneighbors\tgravity\t"\
		"light\n");
	while (curr)
	{
		sec = curr->content;
		sort_sector_wall_list(sec);
		temp = ft_sprintf("%d\t", sec->id);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		temp = set_walls_and_neighbors_for_sector(sec->walls);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		temp = ft_sprintf("\t%d\t%d\n", sec->gravity, sec->light_level);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (str);
}

char	*set_entities(t_editor *doom)
{
	char *temp;
	char *str;
	t_list	*curr;
	t_entity *ent;

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
	char	*fandc;
	char	*sector;
	char	*entity;

	ft_putstr("Saving file.\n");

	recount_everything(doom);

	divider = ft_strdup("-----------------------------------\n");
	map = ft_sprintf("type:map\tname\tscale\tvert\twall\tsec\tent\n0\t%s\t%d\t%d\t%d\t%d\t%d\n",
		doom->fullpath, doom->scale, doom->grid.point_amount,
		doom->grid.wall_amount, doom->grid.sector_amount,
		doom->grid.entity_amount);
	spawn = set_spawn(doom);
	point = set_point(doom);
	wall = set_wall(doom);
	sprite = set_sprite(doom);
	sector = set_sector(doom);
	fandc = set_fandc(doom);
	entity = set_entities(doom);
	str = ft_strjoiner(map, divider, spawn, divider, point, divider, wall, divider, sprite, divider, sector, divider, fandc, divider, entity, divider, NULL);

	fd = creat(doom->fullpath, S_IRUSR | S_IWUSR);
	if (fd > -1)
		ft_fprintf(fd, "%s", str);
	else
		add_text_to_info_box(doom, "[ERROR] Couldnt save map!");
	close(fd);

	ft_printf("%s", str);

	ft_strdel(&divider);
	ft_strdel(&map);
	ft_strdel(&spawn);
	ft_strdel(&point);
	ft_strdel(&wall);
	ft_strdel(&sprite);
	ft_strdel(&fandc);
	ft_strdel(&sector);
	ft_strdel(&entity);
	ft_strdel(&str);

	ft_putstr("Map saved Successfully.\n");
	ft_printf("to : %s\n", doom->fullpath);
}
