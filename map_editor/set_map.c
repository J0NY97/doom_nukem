#include "editor.h"

char	*set_map_info(t_editor *editor)
{
	char	*final;
	char	*map_type;

	if (editor->map_type == MAP_TYPE_STORY)
		map_type = ft_strdup("story");
	else
		map_type = ft_strdup("endless");
	final = ft_sprintf("type:map\ttype\tscale\tvert\twall\tsect\n"
			"%d\t%s\t%.2f\t%d\t%d\t%d\n",
			0,
			map_type,
			editor->map_scale,
			ft_lstlen(editor->points),
			ft_lstlen(editor->walls),
			ft_lstlen(editor->sectors));
	ft_strdel(&map_type);
	return (final);
}

char	*set_spawn(t_editor *editor)
{
	char	*final;
	int		inside_sector_id;

	inside_sector_id = -1;
	if (editor->spawn.inside_sector)
		inside_sector_id = editor->spawn.inside_sector->id;
	final = ft_sprintf("type:spawn\tx\ty\tz\tyaw\tsector\n"
			"%d\t%d\t%d\t%d\t%d\t%d\n",
			0,
			editor->spawn.pos.x,
			editor->spawn.pos.y,
			editor->spawn.z,
			editor->spawn.yaw,
			inside_sector_id);
	return (final);
}

char	*set_points(t_editor *editor)
{
	char	*final;
	char	*temp;
	t_list	*curr;
	t_point	*point;
	int		id;

	id = -1;
	final = ft_sprintf("type:point\n");
	curr = editor->points;
	while (curr)
	{
		point = curr->content;
		point->id = ++id;
		temp = ft_sprintf("%d\t%d\t%d\n",
				point->id, point->pos.x, point->pos.y);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*create_wall_str(t_wall *wall)
{
	char	*final;
	int		texture_id;

	texture_id = wall->wall_texture;
	if (wall->parent_sector->skybox != 0)
		texture_id = wall->parent_sector->skybox;
	if (wall->skybox != 0)
		texture_id = wall->skybox;
	final = ft_sprintf("%d\t%d\t%d\t%d\t%d\t%.2f\t%d\n",
			wall->id, wall->p1->id, wall->p2->id,
			texture_id, wall->portal_texture,
			wall->texture_scale, wall->solid);
	return (final);
}

char	*set_walls(t_editor *editor)
{
	char	*final;
	char	*temp;
	t_list	*curr;
	t_wall	*wall;
	int		id;

	id = -1;
	final = ft_sprintf("type:wall\tp1\tp2\twtx\tptx\tscale\tsolid\n");
	curr = editor->walls;
	while (curr)
	{
		wall = curr->content;
		wall->id = ++id;
		temp = create_wall_str(wall);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*get_wall_sprites(t_wall *wall, int *id)
{
	char		*final;
	char		*temp;
	t_list		*curr;
	t_sprite	*sprite;

	final = NULL;
	curr = wall->sprites;
	while (curr)
	{
		sprite = curr->content;
		*id += 1;
		temp = ft_sprintf("%d\t%d\t%d\t%d\t%d\t%d\t%s\n",
				*id, wall->id, sprite->pos.x, sprite->pos.y,
				sprite->texture_id, (int)(sprite->scale * 100),
				g_sprite_type[sprite->type]);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*set_sprites(t_editor *editor)
{
	char	*final;
	char	*temp;
	t_list	*curr;
	int		total_sprites;

	total_sprites = -1;
	final = ft_sprintf("type:wsprite\twall\tx\ty\ttexture\tscale\ttype\n");
	curr = editor->walls;
	while (curr)
	{
		temp = get_wall_sprites(curr->content, &total_sprites);
		if (temp)
		{
			ft_stradd(&final, temp);
			ft_strdel(&temp);
		}
		curr = curr->next;
	}
	return (final);
}

char	*create_wall_ids(t_list *walls)
{
	char	*final;
	char	temp_str[20];
	t_wall	*wall;

	final = NULL;
	while (walls)
	{
		wall = walls->content;
		ft_stradd(&final, ft_b_itoa(wall->id, temp_str));
		ft_straddchar(&final, ' ');
		walls = walls->next;
	}
	return (final);
}

char	*create_neighbor_ids(t_list *walls)
{
	char	*final;
	char	temp_str[20];
	t_wall	*wall;

	final = NULL;
	while (walls)
	{
		wall = walls->content;
		if (wall->neighbor)
			ft_stradd(&final, ft_b_itoa(wall->neighbor->id, temp_str));
		else
			ft_stradd(&final, "-1");
		ft_straddchar(&final, ' ');
		walls = walls->next;
	}
	return (final);
}

char	*get_sector_wall_ids(t_sector *sector)
{
	char	*final;
	char	*walls;
	char	*neighbors;

	walls = create_wall_ids(sector->walls);
	neighbors = create_neighbor_ids(sector->walls);
	final = ft_strdup(walls);
	ft_straddchar(&final, '\t');
	ft_stradd(&final, neighbors);
	ft_strdel(&walls);
	ft_strdel(&neighbors);
	return (final);
}

char	*create_sector_string(t_sector *sector)
{
	char	*final;
	char	*walls;

	walls = get_sector_wall_ids(sector);
	final = ft_sprintf("%d\t%s\t%d\t%d\n",
			sector->id, walls, sector->gravity, sector->lighting);
	ft_strdel(&walls);
	return (final);
}

char	*set_sectors(t_editor *editor)
{
	char		*final;
	char		*temp;
	t_list		*curr;
	t_sector	*sector;
	int			id;

	id = -1;
	final = ft_sprintf("type:sector\twalls\tneighbors\tg\tl\n");
	curr = editor->sectors;
	while (curr)
	{
		sector = curr->content;
		sector->id = ++id;
		temp = create_sector_string(sector);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*get_wall_slopes(t_list *walls)
{
	t_wall	*wall;
	char	*floor;
	char	*ceil;
	char	*final;

	floor = NULL;
	ceil = NULL;
	while (walls)
	{
		wall = walls->content;
		if (wall->floor_angle != 0)
			floor = ft_sprintf("%d %d ", wall->id, wall->floor_angle);
		if (wall->ceiling_angle != 0)
			ceil = ft_sprintf("%d %d", wall->id, wall->ceiling_angle);
		walls = walls->next;
	}
	if (!floor)
		floor = ft_sprintf("-1 0 ");
	if (!ceil)
		ceil = ft_sprintf("-1 0");
	final = ft_strjoin(floor, ceil);
	ft_strdel(&floor);
	ft_strdel(&ceil);
	return (final);
}

int	get_floor_texture(t_sector *sector)
{
	if (sector->skybox != 0)
		return (sector->skybox);
	return (sector->floor_texture);
}

int	get_ceiling_texture(t_sector *sector)
{
	if (sector->skybox != 0)
		return (sector->skybox);
	return (sector->ceiling_texture);
}

char	*set_fandc(t_editor *editor)
{
	char		*final;
	char		*temp;
	char		*slopes;
	t_list		*curr;
	t_sector	*sector;

	final = ft_sprintf("type:f&c\tfh\tch\tftx\tctx\tfs\tcs\tsl\n");
	curr = editor->sectors;
	while (curr)
	{
		sector = curr->content;
		slopes = get_wall_slopes(sector->walls);
		temp = ft_sprintf("%d\t%d\t%d\t%d\t%d\t%.2f\t%.2f\t%s\n",
				sector->id,
				sector->floor_height, sector->ceiling_height,
				get_floor_texture(sector), get_ceiling_texture(sector),
				sector->floor_scale, sector->ceiling_scale,
				slopes);
		ft_strdel(&slopes);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*set_entity(t_editor *editor)
{
	char		*final;
	char		*temp;
	t_list		*curr;
	t_entity	*entity;
	int			id;

	id = -1;
	final = ft_sprintf("type:entity\tname\tx\ty\tz\td\tsector\n");
	curr = editor->entities;
	while (curr)
	{
		entity = curr->content;
		temp = ft_sprintf("%d\t%s\t%d\t%d\t%d\t%d\t%d\n",
				++id, g_entity_data[entity->type - 1].name,
				entity->pos.x, entity->pos.y, entity->z,
				entity->yaw, get_sector_id(entity->inside_sector));
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*create_event_string(t_event *event)
{
	char	*final;
	char	*sector_text;
	int		target_id;

	final = NULL;
	if (event->pointer_type == TYPE_SECTOR)
		target_id = ((t_sector *)event->pointer)->id;
	else
		target_id = ((t_sprite *)event->pointer)->id;
	if (event->sector)
		sector_text = ft_strdup(event->sector);
	else
		sector_text = ft_strdup("-");
	final = ft_sprintf("%d\t%s\t%s\t%d\t%s\t%d\t%d\t%d\n",
			event->id, g_event_type[event->type],
			g_event_action[event->action].name, target_id,
			sector_text, event->min, event->max, event->speed);
	ft_strdel(&sector_text);
	return (final);
}

char	*set_event(t_editor *editor)
{
	char	*final;
	char	*temp;
	t_list	*curr;
	t_event	*event;
	int		id;

	id = -1;
	final = ft_sprintf("type:event\tTYPE\tACTION\tID\tSECTOR\tMIN\tMAX\tSPEED\n");
	curr = editor->events;
	while (curr)
	{
		event = curr->content;
		event->id = ++id;
		temp = create_event_string(event);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

void	first_half(t_editor *editor, int fd, char *delim)
{
	char	*temp;

	temp = set_map_info(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_spawn(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_points(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_walls(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_sprites(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_sectors(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_fandc(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
}

void	reid(t_editor *editor)
{
	t_list	*curr;
	int		id;

	id = -1;
	curr = editor->sectors;
	while (curr)
	{
		((t_sector *)curr->content)->id = ++id;
		curr = curr->next;
	}
}

void	set_map(t_editor *editor, char *name)
{
	int		fd;
	char	*temp;
	char	delim[3];

	fd = creat(name, S_IRUSR | S_IWUSR | O_CREAT);
	if (!fd)
	{
		ft_printf("[%s] Couldn\'t open map file : %s\n", __FUNCTION__, name);
		return ;
	}
	reid(editor);
	ft_strcpy(delim, "-\n");
	first_half(editor, fd, delim);
	temp = set_entity(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_event(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	close(fd);
	ft_printf("[%s] Map saved succesfully : [%s]\n", __FUNCTION__, name);
}
