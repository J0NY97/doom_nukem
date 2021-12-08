#include "editor.h"

t_sector	*sector_new(void)
{
	t_sector	*sector;

	sector = ft_memalloc(sizeof(t_sector));
	sector->color = random_blue_color();
	sector->floor_height = 0;
	sector->ceiling_height = 20;
	sector->floor_texture = 0;
	sector->ceiling_texture = 0;
	sector->gravity = 20;
	sector->lighting = 0;
	sector->floor_scale = 10.0;
	sector->ceiling_scale = 10.0;
	return (sector);
}

void	sector_free(void *sec, size_t size)
{
	t_sector	*sector;

	sector = sec;
	if (!sector)
		return ;
	ft_lstdel(&sector->walls, &wall_free);
	free(sector);
}

t_sector	*add_sector(t_editor *editor)
{
	t_sector	*sector;

	sector = sector_new();
	sector->id = get_next_sector_id(editor->sectors);
	++editor->sector_amount;
	add_to_list(&editor->sectors, sector, sizeof(t_sector));
	return (sector);
}

void	sector_render(t_editor *editor, t_sector *sector, Uint32 color)
{
	draw_walls(editor, sector->walls, color);
}

/*
 * Checking if we have anything in p1 and p2 should be
 * 	checked before this function is called;
*/
void	add_wall_to_sector_at_pos(
		t_editor *editor, t_sector *sector, t_vec2i p1, t_vec2i p2)
{
	t_wall	*wall;

	wall = add_wall(editor);
	wall->parent_sector = sector;
	wall->p1 = get_point_from_sector_around_radius(sector, p1, 0.0f);
	if (!wall->p1)
	{
		wall->p1 = add_point(editor);
		wall->p1->pos = p1;
	}
	wall->p2 = get_point_from_sector_around_radius(sector, p2, 0.0f);
	if (!wall->p2)
	{
		wall->p2 = add_point(editor);
		wall->p2->pos = p2;
	}
	++sector->wall_amount;
	add_to_list(&sector->walls, wall, sizeof(t_wall));
}

/*
 * NOTE: we only need to move the p1 because
 * 	we sort the list of walls in the sector rendering;
 * 	and if we move both of them, all the walls move double the amount
 * 	they should;
*/
void	move_sector(t_sector *sector, t_vec2i move_amount)
{
	t_list	*wall_list;
	t_wall	*wall;

	wall_list = sector->walls;
	while (wall_list)
	{
		wall = wall_list->content;
		wall->p1->pos = vec2i_add(wall->p1->pos, move_amount);
		wall_list = wall_list->next;
	}
}

void	activate_correct_sector_skybox_button(
		t_editor *editor, t_sector *sector)
{
	if (sector->skybox == -1)
		ui_dropdown_activate(editor->sector_skybox_dropdown,
			editor->sector_skybox_one);
	else if (sector->skybox == -2)
		ui_dropdown_activate(editor->sector_skybox_dropdown,
			editor->sector_skybox_two);
	else if (sector->skybox == -3)
		ui_dropdown_activate(editor->sector_skybox_dropdown,
			editor->sector_skybox_three);
	else
		ui_dropdown_activate(editor->sector_skybox_dropdown,
			editor->sector_skybox_none);
}

/*
 * From t_sector update the ui values;
 * happens when you select new sector;
*/
void	set_sector_ui(t_editor *editor, t_sector *sector)
{
	char	temp_str[20];

	activate_correct_sector_skybox_button(editor, sector);
	sector->floor_texture
		= ft_clamp(sector->floor_texture, 0, MAP_TEXTURE_AMOUNT);
	sector->ceiling_texture
		= ft_clamp(sector->ceiling_texture, 0, MAP_TEXTURE_AMOUNT);
	editor->floor_texture_something.id = sector->floor_texture;
	editor->ceiling_texture_something.id = sector->ceiling_texture;
	ui_input_set_text(editor->floor_height_input,
		ft_b_itoa(sector->floor_height, temp_str));
	ui_input_set_text(editor->ceiling_height_input,
		ft_b_itoa(sector->ceiling_height, temp_str));
	ui_input_set_text(editor->gravity_input,
		ft_b_itoa(sector->gravity, temp_str));
	ui_input_set_text(editor->lighting_input,
		ft_b_itoa(sector->lighting, temp_str));
	ui_input_set_text(editor->floor_texture_scale_input,
		ft_b_ftoa(sector->floor_scale, 2, temp_str));
	ui_input_set_text(editor->ceiling_texture_scale_input,
		ft_b_ftoa(sector->ceiling_scale, 2, temp_str));
	ui_element_image_set(editor->ceiling_texture_image, UI_STATE_AMOUNT,
		editor->wall_textures[sector->ceiling_texture]);
	ui_element_image_set(editor->floor_texture_image, UI_STATE_AMOUNT,
		editor->wall_textures[sector->floor_texture]);
}

void	get_height_inputs(t_editor *editor, t_sector *sector)
{
	char	temp_str[20];

	if (ui_input_exit(editor->floor_height_input))
	{
		sector->floor_height = ft_min(ft_atoi(ui_input_get_text(
						editor->floor_height_input)), sector->ceiling_height);
		ft_b_itoa(sector->floor_height, temp_str);
		ui_input_set_text(editor->floor_height_input, temp_str);
	}
	if (ui_input_exit(editor->ceiling_height_input))
	{
		sector->ceiling_height = ft_max(ft_atoi(ui_input_get_text(
						editor->ceiling_height_input)), sector->floor_height);
		ft_b_itoa(sector->ceiling_height, temp_str);
		ui_input_set_text(editor->ceiling_height_input, temp_str);
	}
}

/*
 * Not grenade launcher,
 * 	but gravity and lighting;
*/
void	get_gl_inputs(t_editor *editor, t_sector *sector)
{
	char	temp_str[20];

	if (ui_input_exit(editor->gravity_input))
	{
		sector->gravity = ft_clamp(ft_atoi(ui_input_get_text(
						editor->gravity_input)), 0, 100);
		ft_b_itoa(sector->gravity, temp_str);
		ui_input_set_text(editor->gravity_input, temp_str);
	}
	if (ui_input_exit(editor->lighting_input))
	{
		sector->lighting = ft_clamp(ft_atoi(ui_input_get_text(
						editor->lighting_input)), 0, 100);
		ft_b_itoa(sector->lighting, temp_str);
		ui_input_set_text(editor->lighting_input, temp_str);
	}
}

void	get_texture_scale_inputs(t_editor *editor, t_sector *sector)
{
	char	temp_str[20];

	if (ui_input_exit(editor->floor_texture_scale_input))
	{
		sector->floor_scale = ft_fclamp(ft_atof(ui_input_get_text(
						editor->floor_texture_scale_input)), 0.1f, 100.0f);
		ft_b_ftoa(sector->floor_scale, 2, temp_str);
		ui_input_set_text(editor->floor_texture_scale_input, temp_str);
	}
	if (ui_input_exit(editor->ceiling_texture_scale_input))
	{
		sector->ceiling_scale = ft_fclamp(ft_atof(ui_input_get_text(
						editor->ceiling_texture_scale_input)), 0.1f, 100.0f);
		ft_b_ftoa(sector->ceiling_scale, 2, temp_str);
		ui_input_set_text(editor->ceiling_texture_scale_input, temp_str);
	}
}

/*
 * From ui update the t_sector values;
 * when you edit sector;
 *
 * TODO: As short as it can be, at some point split this into multiple
 * 		functions ( not yet, convolution otherwise );
*/
void	get_sector_ui(t_editor *editor, t_sector *sector)
{
	t_ui_element	*skybox_active;

	sector->floor_texture = editor->floor_texture_something.id;
	sector->ceiling_texture = editor->ceiling_texture_something.id;
	if (ui_dropdown_exit(editor->sector_skybox_dropdown))
	{
		skybox_active = ui_dropdown_active(editor->sector_skybox_dropdown);
		if (skybox_active == editor->sector_skybox_one)
			sector->skybox = -1;
		else if (skybox_active == editor->sector_skybox_two)
			sector->skybox = -2;
		else if (skybox_active == editor->sector_skybox_three)
			sector->skybox = -3;
		else
			sector->skybox = 0;
	}
	get_height_inputs(editor, sector);
	get_gl_inputs(editor, sector);
	get_texture_scale_inputs(editor, sector);
}

t_sector	*get_sector_with_id(t_list *sectors, int id)
{
	while (sectors)
	{
		if (((t_sector *)sectors->content)->id == id)
			return (sectors->content);
		sectors = sectors->next;
	}
	return (NULL);
}

t_vec2i	get_sector_center(t_sector *sector)
{
	int		i;
	float	x;
	float	y;
	t_list	*wall;
	t_wall	*w;

	x = 0;
	y = 0;
	wall = sector->walls;
	while (wall)
	{
		w = wall->content;
		x += (w->p1->pos.x + w->p2->pos.x);
		y += (w->p1->pos.y + w->p2->pos.y);
		wall = wall->next;
	}
	i = ft_lstlen(sector->walls) * 2;
	if (i < 2)
		return (vec2i(-1, -1));
	return (vec2i(x / i, y / i));
}

t_sector	*get_sector_from_list_around_radius(
		t_list *list, t_vec2i pos, int allowed_radius)
{
	t_sector	*sec;

	while (list)
	{
		sec = list->content;
		if (vec2_in_vec4(pos,
				vec4i(sec->center.x - allowed_radius,
					sec->center.y - allowed_radius,
					allowed_radius * 2, allowed_radius * 2)))
			return (sec);
		list = list->next;
	}
	return (NULL);
}

t_sector	*get_sector_by_id_from_list(t_list *list, int id)
{
	while (list)
	{
		if (((t_sector *)list->content)->id == id)
			return (list->content);
		list = list->next;
	}
	return (NULL);
}

t_wall	*get_sector_wall_at_pos(t_sector *sector, t_vec2i p1, t_vec2i p2)
{
	t_list	*curr;
	t_wall	*curr_wall;

	curr = sector->walls;
	while (curr)
	{
		curr_wall = curr->content;
		if ((compare_veci(curr_wall->p1->pos.v, p1.v, 2)
				&& compare_veci(curr_wall->p2->pos.v, p2.v, 2))
			|| (compare_veci(curr_wall->p1->pos.v, p2.v, 2)
				&& compare_veci(curr_wall->p2->pos.v, p1.v, 2)))
			return (curr_wall);
		curr = curr->next;
	}
	return (NULL);
}

/*
 * Removes all traces of this sector;
*/
int	remove_sector(t_editor *editor, t_sector *sector)
{
	if (!sector)
		return (0);
	remove_from_list(&editor->sectors, sector);
	ft_lstdel(&sector->walls, &dummy_free_er);
	free(sector);
	sector = NULL;
	ft_printf("[%s] Sector removed from editor->sectors\n", __FUNCTION__);
	--editor->sector_amount;
	return (1);
}

int	get_next_sector_id(t_list *list)
{
	t_sector	*event;
	int			i;
	char		ids[1024];

	i = -1;
	memset(ids, 0, sizeof(char) * 1024);
	while (list)
	{
		event = list->content;
		ids[event->id] = 1;
		list = list->next;
	}
	while (ids[++i])
		;
	return (i);
}

/*
 * 'pc' is the one we are comparing;
*/
int	get_point_side(t_vec2i p1, t_vec2i p2, t_vec2i pc)
{
	return ((p2.x - p1.x) * (pc.y - p1.y) - (p2.y - p1.y) * (pc.x - p1.x));
}

/*
 * Yoinked from here:
 * http://www.sunshine2k.de/coding/java/Polygon/Convex/polygon.htm
 * https://github.com/Epicurius
*/
int	check_sector_convexity(t_sector *sector)
{
	int		i;
	int		res;
	int		newres;
	t_wall	*w1;
	t_wall	*w2;
	t_list	*curr;

	i = 0;
	curr = sector->walls;
	while (curr)
	{
		w1 = curr->content;
		w2 = get_connected_wall(sector->walls, w1);
		if (!w2)
			return (-1);
		if (i == 0)
			res = get_point_side(w1->p2->pos, w2->p2->pos, w1->p1->pos);
		else
		{
			newres = get_point_side(w1->p2->pos, w2->p2->pos, w1->p1->pos);
			if ((newres > 0 && res < 0) || (newres < 0 && res > 0))
				return (0);
			res = newres;
		}
		i++;
		curr = curr->next;
	}
	return (1);
}

/*
 * The wall should be sorted, winding doesnt matter;
*/
int	check_point_in_sector(t_sector *sector, t_vec2i p)
{
	int		i;
	int		res;
	int		newres;
	t_wall	*wall;
	t_list	*curr;

	i = 0;
	curr = sector->walls;
	while (curr)
	{
		wall = curr->content;
		if (i == 0)
			res = get_point_side(wall->p1->pos, wall->p2->pos, p);
		else
		{
			newres = get_point_side(wall->p1->pos, wall->p2->pos, p);
			if (newres == 0 || res == 0
				|| (newres > 0 && res < 0) || (newres < 0 && res > 0))
				return (0);
			res = newres;
		}
		i++;
		curr = curr->next;
	}
	return (1);
}

void	entity_inside_which_sector(t_list *sectors, t_entity *entity)
{
	t_list	*curr;

	curr = sectors;
	entity->inside_sector = NULL;
	while (curr)
	{
		if (check_point_in_sector(curr->content, entity->pos) == 1)
		{
			entity->inside_sector = curr->content;
			break ;
		}
		curr = curr->next;
	}
}

void	sector_check_errors(t_editor *editor, t_sector *sector)
{
	if (!check_sector_convexity(sector))
	{
		draw_text(editor->drawing_surface, "Not Convex!",
			editor->font, sector->screen_center, 0xffff0000);
		editor->errors += 1;
	}
	if (sector->ceiling_height - sector->floor_height < 0)
	{
		draw_text(editor->drawing_surface,
			"Floor & Ceiling Height Doesn\'t Make Sense!",
			editor->font, sector->screen_center, 0xffffff00);
		editor->errors += 1;
	}
}

int	get_sector_id(t_sector *sector)
{
	if (!sector)
		return (-1);
	return (sector->id);
}
