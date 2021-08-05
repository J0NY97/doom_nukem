/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/11 13:40:11 by nneronin          #+#    #+#             */
/*   Updated: 2021/07/28 16:41:15 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	rearrange_wall_neighbors(t_editor *editor)
{
	t_list	*curr_sec;
	t_list	*curr;

	curr_sec = editor->grid.sectors;
	while (curr_sec)
	{
		curr = ((t_sector *)curr_sec->content)->walls;
		while (curr)
		{
			((t_wall *)curr->content)->neighbor_sector
				= get_sector_with_id(editor->grid.sectors,
					((t_wall *)curr->content)->neighbor);
			curr = curr->next;
		}
		curr_sec = curr_sec->next;
	}
}

void	read_mapinfo(t_editor *editor, int fd)
{
	char	*line;
	char	**arr;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		editor->scale = ft_atoi(arr[2]);
		free_array(arr);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

/*
 * NOTE: read_events needs to be called after we have gotten all the sectors.
*/
void	read_events(t_editor *editor, int fd)
{
	char	*line;
	char	**arr;
	t_event	*event;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		event = new_event();
		event->type = ft_strdup(arr[1]);
		event->action = ft_strdup(arr[2]);
		event->id = ft_atoi(arr[3]); // is this wall sprite id?
		if (!ft_strequ(arr[4], "-"))
			event->sector = get_sector_with_id(editor->grid.sectors,
					atoi(arr[4]));
		if (!ft_strequ(arr[5], "-"))
			event->min = ft_atoi(arr[5]);
		if (!ft_strequ(arr[6], "-"))
			event->max = ft_atoi(arr[6]);
		if (!ft_strequ(arr[7], "-"))
			event->speed = ft_atoi(arr[7]);
		add_to_list(&editor->grid.events, event, sizeof(t_event));
		free_array(arr);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

void	choose_correct_reader(t_editor *editor, char *line, int fd)
{
	if (!(ft_strncmp(line, "type:map", 8)))
		read_mapinfo(editor, fd);
	else if (!(ft_strncmp(line, "type:vertex", 11)))
		read_vertex(&editor->grid, fd);
	else if (!(ft_strncmp(line, "type:wsprite", 12)))
		read_sprite(&editor->grid, fd);
	else if (!(ft_strncmp(line, "type:wall", 9)))
		read_wall(&editor->grid, fd);
	else if (!(ft_strncmp(line, "type:spawn", 10)))
		read_spawn(&editor->spawn, fd);
	else if (!(ft_strncmp(line, "type:sector", 11)))
	{
		read_sectors(editor, fd);
		rearrange_wall_neighbors(editor);
	}
	else if (!(ft_strncmp(line, "type:f&c", 8)))
		read_fandc(editor, fd);
	else if (!(ft_strncmp(line, "type:entity", 10)))
		read_entities(editor, fd);
	else if (!(ft_strncmp(line, "type:event", 10)))
		read_events(editor, fd);
}

void	read_map_file(t_editor *editor)
{
	int		fd;
	char	*line;

	fd = open(editor->fullpath, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr("[read_map_file] Couldnt open map.\n");
		return ;
	}
	while (get_next_line(fd, &line))
	{
		choose_correct_reader(editor, line, fd);
		ft_strdel(&line);
	}
	close(fd);
	ft_strdel(&line);
}
