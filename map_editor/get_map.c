#include "editor.h"

/*
 * Only the map type is usefull here, for the correct tickbox to be ticked;
*/
void	get_map_info(t_editor *editor, char **args)
{
	ft_printf("[%s]\n", __FUNCTION__);
	editor->map_type = MAP_TYPE_ENDLESS;
	if (ft_strnequ(args[1], "story", 5))
		editor->map_type = MAP_TYPE_STORY;
}

void	get_spawn(t_editor *editor, char **lines, int *i)
{
	char	**args;

	ft_printf("[%s]\n", __FUNCTION__);
	args = ft_strsplit(lines[*i + 1], '\t');
	editor->spawn.pos.x = ft_atoi(args[1]);
	editor->spawn.pos.y = ft_atoi(args[2]);
	editor->spawn.z = ft_atoi(args[3]);
	editor->spawn.yaw = ft_atoi(args[4]);
	*i += 2;
	ft_arraydel(args);
}

void	get_points(t_editor *editor, char **lines, int *i)
{
	char	**args;
	t_point	*point;

	ft_printf("[%s]\n", __FUNCTION__);
	while (lines[*i])
	{
		*i += 1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		point = add_point(editor);
		point->id = ft_atoi(args[0]);
		point->pos.x = ft_atoi(args[1]);
		point->pos.y = ft_atoi(args[2]);
		ft_arraydel(args);
	}
}

void	get_walls(t_editor *editor, char **lines, int *i)
{
	char	**args;
	t_wall	*wall;

	ft_printf("[%s]\n", __FUNCTION__);
	while (lines[*i])
	{
		*i += 1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		wall = add_wall(editor);
		wall->id = ft_atoi(args[0]);
		wall->p1 = get_point_with_id(editor->points, ft_atoi(args[1]));
		wall->p2 = get_point_with_id(editor->points, ft_atoi(args[2]));
		wall->wall_texture = ft_atoi(args[3]);
		if (wall->wall_texture < 0)
		{
			wall->skybox = wall->wall_texture;
			wall->wall_texture = 0;
		}
		wall->portal_texture = ft_atoi(args[4]);
		wall->texture_scale = ft_atof(args[5]);
		wall->solid = ft_atoi(args[6]);
		ft_arraydel(args);
	}
}

/*
 * If no wall with correct id found, we will ignore this sprite;
*/
void	get_sprites(t_editor *editor, char **lines)
{
	t_sprite	*sprite;
	int			wall_id;
	t_wall		*parent_wall;
	int			ggg;

	ft_printf("[%s]\n", __FUNCTION__);
	ggg = -1;
	wall_id = ft_atoi(lines[1]);
	parent_wall = get_wall_with_id(editor->walls, wall_id);
	if (parent_wall)
	{
		sprite = add_sprite(editor);
		sprite->id = ft_atoi(lines[0]);
		sprite->parent = parent_wall;
		sprite->pos.x = ft_atoi(lines[2]);
		sprite->pos.x = ft_atoi(lines[3]);
		sprite->texture_id = ft_atoi(lines[4]);
		if (ft_atoi(lines[5]) != 0)
			sprite->scale = 100 / ft_atoi(lines[5]);
		while (++ggg < SPRITE_TYPE_AMOUNT)
			if (ft_strequ(g_sprite_type[ggg], lines[6]))
				sprite->type = ggg;
		add_to_list(&parent_wall->sprites, sprite, sizeof(t_sprite));
		++parent_wall->sprite_amount;
	}
}

/*
 * search from 'list' walls with 'id' in 'id_str' (delim with ' '(space))
 * 	and add to 'sector'->walls;
 * and also settings neighbor ids at the same time, becuase why not;
*/
void	get_sector_walls(
		t_list *list, char *id_str, char *neighbor_str, t_sector *sector)
{
	char	**wall_ids;
	char	**neigh_ids;
	int		i;
	t_wall	*wall;
	int		id;

	i = -1;
	wall_ids = ft_strsplit(id_str, ' ');
	neigh_ids = ft_strsplit(neighbor_str, ' ');
	while (wall_ids[++i])
	{
		id = ft_atoi(wall_ids[i]);
		wall = get_wall_with_id(list, id);
		if (wall)
		{
			wall->parent_sector = sector;
			wall->neighbor_id = ft_atoi(neigh_ids[i]);
			++sector->wall_amount;
			add_to_list(&sector->walls, wall, sizeof(t_wall));
		}
		else
			ft_printf("[%s] No wall with id : %d.\n", __FUNCTION__, id);
	}
	ft_arraydel(wall_ids);
}

/*
 * From the sector wall goes through all
 * 		and gets the actual sector from the neighbor id in the walls;
*/
void	get_all_actual_sectors(t_editor *editor)
{
	t_list	*curr_sec;
	t_list	*curr_wall;

	curr_sec = editor->sectors;
	while (curr_sec)
	{
		curr_wall = ((t_sector *)curr_sec->content)->walls;
		while (curr_wall)
		{
			((t_wall *)curr_wall->content)->neighbor
				= get_sector_by_id_from_list(editor->sectors,
					((t_wall *)curr_wall->content)->neighbor_id);
			curr_wall = curr_wall->next;
		}
		curr_sec = curr_sec->next;
	}
}

void	get_sectors(t_editor *editor, char **lines, int *i)
{
	char		**args;
	t_sector	*sector;

	ft_printf("[%s]\n", __FUNCTION__);
	while (lines[*i])
	{
		*i += 1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		sector = add_sector(editor);
		sector->id = ft_atoi(args[0]);
		get_sector_walls(editor->walls, args[1], args[2], sector);
		sector->gravity = ft_atoi(args[3]);
		sector->lighting = ft_atoi(args[4]);
		ft_arraydel(args);
	}
	get_all_actual_sectors(editor);
}

void	set_slopes(t_list *walls, char **slopes)
{
	t_wall	*wall;

	if (ft_atoi(slopes[0]) != -1)
	{
		wall = get_wall_with_id(walls, ft_atoi(slopes[0]));
		if (wall)
			wall->floor_angle = ft_atoi(slopes[1]);
	}
	if (ft_atoi(slopes[2]) != -1)
	{
		wall = get_wall_with_id(walls, ft_atoi(slopes[2]));
		if (wall)
			wall->ceiling_angle = ft_atoi(slopes[3]);
	}
}

/*
 * NOTE:
 *		Returns if we cant find sector with correct id;
*/
void	get_fc(t_editor *editor, char **args)
{
	char		**slopes;
	t_sector	*sector;
	int			id;

	ft_printf("[%s]\n", __FUNCTION__);
	id = ft_atoi(args[0]);
	sector = get_sector_by_id_from_list(editor->sectors, id);
	if (!sector)
		return ;
	sector->floor_height = ft_atoi(args[1]);
	sector->ceiling_height = ft_atoi(args[2]);
	sector->floor_texture = ft_atoi(args[3]);
	sector->ceiling_texture = ft_atoi(args[4]);
	if (sector->floor_texture < 0)
		sector->skybox = sector->floor_texture;
	sector->floor_scale = ft_atof(args[5]);
	sector->ceiling_scale = ft_atof(args[6]);
	slopes = ft_strsplit(args[7], ' ');
	set_slopes(sector->walls, slopes);
}

void	get_entity(t_editor *editor, char **lines, int *i)
{
	char		**args;
	t_entity	*entity;

	ft_printf("[%s]\n", __FUNCTION__);
	while (lines[*i])
	{
		*i += 1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		entity = add_entity(editor);
		entity->type = get_entity_type(args[1]);
		entity->pos.x = ft_atoi(args[2]);
		entity->pos.y = ft_atoi(args[3]);
		entity->z = ft_atoi(args[4]);
		entity->yaw = ft_atoi(args[5]);
		ft_arraydel(args);
	}
}

void	get_events(t_editor *editor, char **args)
{
	t_event		*event;

	ft_printf("[%s]\n", __FUNCTION__);
	event = add_event(editor);
	event->id = ft_atoi(args[0]);
	event->type = get_event_type(args[1]);
	event->action = get_event_action(args[2]);
	event->pointer_type = TYPE_SPRITE;
	if (event->action == SECTOR || event->action == TYPE_SECTOR)
	{
		event->pointer_type = TYPE_SECTOR;
		event->pointer
			= get_sector_by_id_from_list(editor->sectors, ft_atoi(args[3]));
	}
	else
		event->pointer
			= get_sprite_by_id_from_list(editor->sprites, ft_atoi(args[3]));
	event->sector = ft_strdup(args[4]);
	event->min = ft_atoi(args[5]);
	event->max = ft_atoi(args[6]);
	event->speed = ft_atoi(args[7]);
	update_event_elem(event->elem);
}

void	helper_pelper(
		t_editor *editor, char **lines, int *i, void (*f)(t_editor *, char **))
{
	char		**args;

	while (lines[*i])
	{
		*i += 1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		f(editor, args);
		ft_arraydel(args);
	}
}

void	decision_maker(t_editor *editor, char *type, char **lines, int *i)
{
	if (ft_strnequ(type, "type:map", 8))
		helper_pelper(editor, lines, i, &get_map_info);
	else if (ft_strnequ(type, "type:spawn", 10))
		get_spawn(editor, lines, i);
	else if (ft_strnequ(type, "type:point", 10))
		get_points(editor, lines, i);
	else if (ft_strnequ(type, "type:wall", 9))
		get_walls(editor, lines, i);
	else if (ft_strnequ(type, "type:wsprite", 12))
		helper_pelper(editor, lines, i, &get_sprites);
	else if (ft_strnequ(type, "type:sector", 11))
		get_sectors(editor, lines, i);
	else if (ft_strnequ(type, "type:f&c", 8))
		helper_pelper(editor, lines, i, &get_fc);
	else if (ft_strnequ(type, "type:entity", 11))
		get_entity(editor, lines, i);
	else if (ft_strnequ(type, "type:event", 10))
		helper_pelper(editor, lines, i, &get_events);
}

void	get_map(t_editor *editor, char *map)
{
	char	*file_content;
	char	**lines;
	int		i;

	ft_printf("[%s] Getting map <%s>.\n", __FUNCTION__, map);
	file_content = ft_get_file_content(map);
	lines = ft_strsplit(file_content, '\n');
	i = -1;
	while (lines && lines[++i])
		decision_maker(editor, lines[i], lines, &i);
	ft_arraydel(lines);
	ft_strdel(&file_content);
	ft_printf("[%s] Success getting map.\n", __FUNCTION__);
}
