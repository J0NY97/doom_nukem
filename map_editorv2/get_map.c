/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/11 13:40:11 by nneronin          #+#    #+#             */
/*   Updated: 2020/10/11 15:27:17 by nneronin         ###   ########.fr       */
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

void	read_vertex(t_grid *grid, int fd)
{
	char *line;
	char **arr;
	t_point	*new_p;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		new_p = (t_point *)malloc(sizeof(t_point));
		new_p->pos = gfx_new_vector(ft_atoi(arr[1]),
									ft_atoi(arr[2]),
									ft_atoi(arr[3]));
		new_p->id = ft_atoi(arr[0]);
		add_to_list(&grid->points, new_p, sizeof(t_point));
		free_array(arr);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

void	read_map(t_map *map, int fd)
{
	char *line;
	char **arr;
	int	w;
	int h;
	char *name;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		w = ft_atoi(arr[0]);
		h = ft_atoi(arr[1]);
		name = ft_strdup(arr[2]);
		printf("%d, %d, %s\n", w, h, name);
		free(name);
		free_array(arr);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

// TODO: get the texture_scale from the file, to accomplish this you have to first write in it
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
		new_w = new_wall(get_point_with_id(grid->points, ft_atoi(arr[1])),
				get_point_with_id(grid->points, ft_atoi(arr[2])));
		new_w->id = ft_atoi(arr[0]);
		//new_w->orig = get_point_with_id(grid->points, ft_atoi(arr[1]));
		//new_w->dest = get_point_with_id(grid->points, ft_atoi(arr[2]));
		new_w->texture_id = ft_atoi(arr[3]);
		add_to_list(&grid->walls, new_w, sizeof(t_wall));
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
		spawn->pos = gfx_new_vector(ft_atoi(arr[0]), ft_atoi(arr[1]), ft_atoi(arr[2]));
		free_array(arr);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

void	read_sectors(t_editor *doom, int fd)
{
	t_sector *sect;
	char *line;
	char **arr;
	char **walls;
	char **neighbor;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		sect = new_sector(doom->grid.sector_amount++);
		sect->id = ft_atoi(arr[0]);
		sect->floor_height = ft_atoi(arr[1]);
		sect->ceiling_height = ft_atoi(arr[2]);
		walls = ft_strsplit(arr[3], ' ');
		neighbor = ft_strsplit(arr[4], ' ');
		int i = 0;
		while (walls[i] != 0)
		{
			add_to_list(&sect->walls, get_wall_with_id(doom->grid.walls, ft_atoi(walls[i])), sizeof(t_wall));
			((t_wall *)sect->walls->content)->neighbor = ft_atoi(neighbor[i]);
			i++;
		}
		sect->floor_texture = ft_atoi(arr[5]);
		sect->ceiling_texture = ft_atoi(arr[6]);
		sect->gravity = ft_atoi(arr[7]);
		sect->light_level = ft_atoi(arr[8]);
		add_to_list(&doom->grid.sectors, sect, sizeof(t_sector));
		free_array(arr);
		free_array(walls);
		free_array(neighbor);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

void		read_entities(t_editor *doom, int fd)
{
	t_entity *ent;
	char *line;
	char **arr;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		ent = new_entity(ft_atoi(arr[0]), (t_vector){
				atof(arr[1]),
				atof(arr[2]),
				atof(arr[3])
			});
		ent->type = ft_atoi(arr[4]);
		ent->sprite_id = ft_atoi(arr[5]);
		ent->max_health = ft_atoi(arr[6]);
		ent->speed = ft_atoi(arr[7]);
		add_to_list(&doom->grid.entities, ent, sizeof(t_entity));
		free_array(arr);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

void		read_map_file(t_editor *doom)
{
	int fd;
	char *line;

	if ((fd = open(doom->filename, O_RDONLY)) < 0)
		return ;
	while (get_next_line(fd, &line))
	{
		ft_putstr(line);
		ft_putchar('\n');
		if (!(ft_strncmp(line, "type:vertex", 11)))
			read_vertex(&doom->grid, fd);
		else if (!(ft_strncmp(line, "type:wall", 9)))
			read_wall(&doom->grid, fd);
		else if (!(ft_strncmp(line, "type:spawn", 10)))
			read_spawn(&doom->spawn, fd);
		else if (!(ft_strncmp(line, "type:sectors", 11)))
			read_sectors(doom, fd);
		else if (!(ft_strncmp(line, "type:entity", 10)))
			read_entities(doom, fd);
		ft_strdel(&line);
	}
	close(fd);
	free(line);
}
