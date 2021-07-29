/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:44:41 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 13:44:42 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

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
