/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:44:41 by jsalmi           ###   ########.fr       */
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

/*
 * Returns a string made of all the char **.
*/
char	*compile_str_from_array(char **arr)
{
	char	*str;
	int		i;

	i = 0;
	str = NULL;
	while (arr[i])
	{
		ft_stradd(&str, arr[i]);
		if (arr[i + 1] != NULL)
			ft_straddchar(&str, ' ');
		i++;
	}
	return (str);
}

char	*ttt(t_list *wall_list, char **wall_ids, char **neighbor_ids)
{
	t_list	*curr;
	char	*str;
	char	*walls;
	char	*neighbors;
	int		i;

	i = 0;
	curr = wall_list;
	while (curr)
	{
		wall_ids[i] = ft_itoa(((t_wall *)curr->content)->id);
		if (((t_wall *)curr->content)->neighbor_sector)
			neighbor_ids[i]
				= ft_itoa(((t_wall *)curr->content)->neighbor_sector->id);
		else
			neighbor_ids[i] = ft_itoa(-1);
		curr = curr->next;
		i++;
	}
	walls = compile_str_from_array(wall_ids);
	neighbors = compile_str_from_array(neighbor_ids);
	str = ft_sprintf("%s\t%s", walls, neighbors);
	ft_strdel(&walls);
	ft_strdel(&neighbors);
	return (str);
}

char	*set_walls_and_neighbors_for_sector(t_list *wall_list)
{
	char	*str;
	char	**wall_ids;
	char	**neighbor_ids;
	int		wall_amount;

	wall_amount = ft_lstlen(wall_list);
	wall_ids = ft_memalloc(sizeof(char *) * wall_amount + 1);
	wall_ids[wall_amount] = NULL;
	neighbor_ids = ft_memalloc(sizeof(char *) * wall_amount + 1);
	neighbor_ids[wall_amount] = NULL;
	str = ttt(wall_list, wall_ids, neighbor_ids);
	free_array(wall_ids);
	free_array(neighbor_ids);
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

char	*actual_set_map(t_editor *editor)
{
	char	*map;
	char	*type;

	if (editor->active_map_type == editor->story_tickbox)
		type = ft_strdup("story");
	else
		type = ft_strdup("endless");
	map = ft_sprintf("type:map\ttype\tscale\tvert\twall\tsec\tent\n"\
			"0\t%s\t%d\t%d\t%d\t%d\t%d\n",
			type, editor->scale, editor->grid.point_amount,
			editor->grid.wall_amount, editor->grid.sector_amount,
			editor->grid.entity_amount);
	ft_strdel(&type);
	return (map);
}

enum	e_map_order
{
	M_MAP,
	M_SPAWN,
	M_POINT,
	M_WALL,
	M_SPRITE,
	M_SECTOR,
	M_FANDC,
	M_ENTITY,
	M_DIVIDER,
	M_ORDER_AMOUNT
};

char	*make_whole_map_string(t_editor *editor)
{
	char	**order;
	char	*str;

	order = ft_memalloc(sizeof(char *) * M_ORDER_AMOUNT + 1);
	order[M_ORDER_AMOUNT] = NULL;
	order[M_DIVIDER] = ft_strdup("-----------------------------------\n");
	order[M_MAP] = actual_set_map(editor);
	order[M_SPAWN] = set_spawn(editor);
	order[M_POINT] = set_point(editor);
	order[M_WALL] = set_wall(editor);
	order[M_SPRITE] = set_sprite(editor);
	order[M_SECTOR] = set_sector(editor);
	order[M_FANDC] = set_fandc(editor);
	order[M_ENTITY] = set_entities(editor);
	str = ft_strjoiner(order[M_MAP], order[M_DIVIDER],
			order[M_SPAWN], order[M_DIVIDER],
			order[M_POINT], order[M_DIVIDER],
			order[M_WALL], order[M_DIVIDER],
			order[M_SPRITE], order[M_DIVIDER],
			order[M_SECTOR], order[M_DIVIDER],
			order[M_FANDC], order[M_DIVIDER],
			order[M_ENTITY], order[M_DIVIDER], NULL);
	free_array(order);
	return (str);
}

void	set_map(t_editor *editor)
{
	char	*str;
	int		fd;

	ft_putstr("Saving file.\n");
	recount_everything(editor);
	str = make_whole_map_string(editor);
	fd = creat(editor->fullpath, S_IRUSR | S_IWUSR);
	if (fd > -1)
		ft_dprintf(fd, "%s", str);
	else
		add_text_to_info_box(editor, "[ERROR] Couldnt save map!");
	close(fd);
	ft_printf("%s", str);
	ft_strdel(&str);
	ft_putstr("Map saved Successfully.\n");
	ft_printf("to : %s\n", editor->fullpath);
}
