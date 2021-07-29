/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:39:00 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 13:39:02 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	add_wall_ids_to_sector(
	t_editor *editor, t_sector *sector, char **walls, char **neighbors)
{
	int	i;

	i = 0;
	while (walls[i] != 0)
	{
		add_to_list(&sector->walls, get_wall_with_id(
				editor->grid.walls,
				ft_atoi(walls[i])), sizeof(t_wall));
		((t_wall *)sector->walls->content)->neighbor
			= ft_atoi(neighbors[i]);
		i++;
	}
}

void	read_sectors(t_editor *editor, int fd)
{
	char		*line;
	char		**arr;
	char		**walls;
	char		**neighbors;
	t_sector	*sect;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		sect = new_sector(editor->grid.sector_amount++);
		sect->id = ft_atoi(arr[0]);
		walls = ft_strsplit(arr[1], ' ');
		neighbors = ft_strsplit(arr[2], ' ');
		add_wall_ids_to_sector(editor, sect, walls, neighbors);
		sect->gravity = ft_atoi(arr[3]);
		sect->light_level = ft_atoi(arr[4]);
		add_to_list(&editor->grid.sectors, sect, sizeof(t_sector));
		free_array(arr);
		free_array(walls);
		free_array(neighbors);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

void	set_sector_slope(t_sector *sec, char **slope_arr)
{
	sec->floor_slope_wall_id = ft_atoi(slope_arr[0]);
	sec->floor_slope = ft_atoi(slope_arr[1]);
	sec->ceiling_slope_wall_id = ft_atoi(slope_arr[2]);
	sec->ceiling_slope = ft_atoi(slope_arr[3]);
}

void	read_fandc(t_editor *editor, int fd)
{
	char		*line;
	char		**arr;
	char		**slope_arr;
	t_sector	*sec;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		sec = get_sector_with_id(editor->grid.sectors,
				ft_atoi(arr[0]));
		sec->floor_height = ft_atoi(arr[1]);
		sec->ceiling_height = ft_atoi(arr[2]);
		sec->floor_texture = ft_atoi(arr[3]);
		sec->ceiling_texture = ft_atoi(arr[4]);
		sec->floor_texture_scale = ft_atof(arr[5]);
		sec->ceiling_texture_scale = ft_atof(arr[6]);
		slope_arr = ft_strsplit(arr[7], ' ');
		set_sector_slope(sec, slope_arr);
		free_array(arr);
		free_array(slope_arr);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}
