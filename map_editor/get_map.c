/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/11 13:40:11 by nneronin          #+#    #+#             */
/*   Updated: 2021/07/19 14:41:40 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_point	*get_point_with_id(t_list *points, unsigned int id)
{
	t_list *curr;

	curr = points;
	while (curr)
	{
		if (((t_point *)curr->content)->id == id)
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

t_wall	*get_wall_with_id(t_list *walls, unsigned int id)
{
	t_list *curr;

	curr = walls;
	while (curr)
	{
		if (((t_wall *)curr->content)->id == id)
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

t_sector *get_sector_with_id(t_list *sectors, unsigned int id)
{
	t_list *curr;

	curr = sectors;
	while (curr)
	{
		if (((t_sector *)curr->content)->id == id)
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

void	read_vertex(t_grid *grid, int fd)
{
	char	*line;
	char	**arr;
	t_point	*new_p;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		ft_putstr(line);
		ft_putstr("\n");
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
	char *line;
	char **arr;
	t_wall *new_w;

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
	char *line;
	char **arr;

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

void	read_entities(t_editor *editor, int fd)
{
	int		id;
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
		ent->preset =
			get_entity_preset_with_name(editor->entity_presets,
			arr[1]);
		ent->direction = ft_atoi(arr[5]);
		add_to_list(&editor->grid.entities, ent, sizeof(t_entity));
		free_array(arr);
		ft_strdel(&line);
		id++;
	}
	ft_strdel(&line);
}

void	read_mapinfo(t_editor *editor, int fd)
{
	char *line;
	char **arr;

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
		read_sectors(editor, fd);
	else if (!(ft_strncmp(line, "type:f&c", 8)))
		read_fandc(editor, fd);
	else if (!(ft_strncmp(line, "type:entity", 10)))
		read_entities(editor, fd);
}

void		read_map_file(t_editor *editor)
{
	int	fd;
	char	*line;

	if ((fd = open(editor->fullpath, O_RDONLY)) < 0)
	{
		ft_putstr("Couldnt open map.\n");
		return ;
	}
	while (get_next_line(fd, &line))
	{
		ft_putstr(line);
		ft_putchar('\n');
		choose_correct_reader(editor, line, fd);
		ft_strdel(&line);
		ft_putstr("read");
	}
	close(fd);
	free(line);
}
