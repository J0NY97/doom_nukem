/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:39:03 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 16:42:11 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	read_vertex(t_grid *grid, int fd)
{
	char	*line;
	char	**arr;
	t_point	*new_p;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		new_p = (t_point *)malloc(sizeof(t_point));
		new_p->pos = gfx_new_vector(ft_atoi(arr[1]),
				ft_atoi(arr[2]),
				0);
		new_p->id = ft_atoi(arr[0]);
		add_to_list(&grid->points, new_p, sizeof(t_point));
		free_array(arr);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

void	read_wall(t_grid *grid, int fd)
{
	char	*line;
	char	**arr;
	t_wall	*new_w;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		new_w = new_wall(get_point_with_id(grid->points,
					ft_atoi(arr[1])),
				get_point_with_id(grid->points,
					ft_atoi(arr[2])));
		new_w->id = ft_atoi(arr[0]);
		new_w->texture_id = ft_atoi(arr[3]);
		new_w->portal_texture_id = ft_atoi(arr[4]);
		new_w->texture_scale = ft_atof(arr[5]);
		new_w->solid = ft_atoi(arr[6]);
		add_to_list(&grid->walls, new_w, sizeof(t_wall));
		free_array(arr);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

void	read_sprite(t_grid *grid, int fd)
{
	char		*line;
	char		**arr;
	t_wall		*wall;
	t_sprite	*sprite;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		sprite = new_sprite();
		sprite->real_x = ft_atof(arr[2]);
		sprite->real_y = ft_atof(arr[3]);
		sprite->sprite_id = ft_atoi(arr[4]);
		sprite->scale = ft_atof(arr[5]);
		wall = get_wall_with_id(grid->walls, ft_atoi(arr[1]));
		add_to_list(&wall->sprites, sprite, sizeof(t_sprite));
		free_array(arr);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

void	read_spawn(t_spawn *spawn, int fd)
{
	char	*line;
	char	**arr;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		spawn->pos = gfx_new_vector(
				ft_atoi(arr[1]),
				ft_atoi(arr[2]),
				ft_atoi(arr[3]));
		spawn->direction = ft_atoi(arr[4]);
		free_array(arr);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

void	read_entities(t_editor *editor, int fd)
{
	int			id;
	char		*line;
	char		**arr;
	t_entity	*ent;

	id = 0;
	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		ent = new_entity(ft_atoi(arr[0]), (t_vector){
				atof(arr[2]),
				atof(arr[3]),
				atof(arr[4])});
		ent->preset
			= get_entity_preset_with_name(editor->entity_presets, arr[1]);
		ent->direction = ft_atoi(arr[5]);
		add_to_list(&editor->grid.entities, ent, sizeof(t_entity));
		free_array(arr);
		ft_strdel(&line);
		id++;
	}
	ft_strdel(&line);
}
