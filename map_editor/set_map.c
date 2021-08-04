/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/29 13:03:41 by jsalmi           ###   ########.fr       */
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
	M_EVENT,
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
	order[M_EVENT] = set_event(editor);
	str = ft_strjoiner(order[M_MAP], order[M_DIVIDER],
			order[M_SPAWN], order[M_DIVIDER], order[M_POINT], order[M_DIVIDER],
			order[M_WALL], order[M_DIVIDER], order[M_SPRITE], order[M_DIVIDER],
			order[M_SECTOR], order[M_DIVIDER], order[M_FANDC], order[M_DIVIDER],
			order[M_ENTITY], order[M_DIVIDER],
			order[M_EVENT], order[M_DIVIDER], NULL);
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
	ft_strdel(&str);
	ft_putstr("Map saved Successfully.\n");
	ft_printf("to : %s\n", editor->fullpath);
}
