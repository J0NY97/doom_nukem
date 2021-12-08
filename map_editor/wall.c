#include "editor.h"

t_wall	*wall_new(void)
{
	t_wall	*wall;

	wall = ft_memalloc(sizeof(t_wall));
	wall->solid = 1;
	wall->texture_scale = 10.0f;
	return (wall);
}

/*
 * NOTE:
 * 	this function's job is not to free the points,
 * 		they might be used in some other wall;
*/
void	wall_free(void *w, size_t size)
{
	t_wall	*wall;

	wall = w;
	if (!wall)
		return ;
	ft_lstdel(&wall->sprites, &sprite_free);
	free(wall);
}

void	wall_render(t_editor *editor, t_wall *wall, Uint32 color)
{
	t_vec2i	p1;
	t_vec2i	p2;

	p1 = conversion(editor, wall->p1->pos);
	p2 = conversion(editor, wall->p2->pos);
	ui_surface_line_draw(editor->drawing_surface, p1, p2, color);
}

void	remove_all_wall_sprites(t_editor *editor, t_wall *wall)
{
	t_list	*curr;
	t_list	*next;

	curr = wall->sprites;
	while (curr)
	{
		next = curr->next;
		remove_sprite(editor, curr->content);
		curr = next;
	}
}

/*
 * Removes wall from editor walls;
*/
int	remove_wall(t_editor *editor, t_wall *wall)
{
	if (!wall)
		return (0);
	remove_from_list(&editor->walls, wall);
	remove_all_wall_sprites(editor, wall);
	free(wall);
	wall = NULL;
	--editor->wall_amount;
	return (1);
}

void	move_wall(t_wall *wall, t_vec2i move_amount)
{
	if (!wall)
		return ;
	wall->p1->pos = vec2i_add(wall->p1->pos, move_amount);
	wall->p2->pos = vec2i_add(wall->p2->pos, move_amount);
}

/*
 * Add wall to editor->walls and returns itself;
*/
t_wall	*add_wall(t_editor *editor)
{
	t_wall	*wall;

	wall = wall_new();
	wall->wall_texture = 7;
	add_to_list(&editor->walls, wall, sizeof(t_wall));
	++editor->wall_amount;
	return (wall);
}

t_list	*get_next_wall_node(t_list *list, t_wall *wall)
{
	t_wall	*dest_wall;

	while (list)
	{
		dest_wall = list->content;
		if (dest_wall->p1 == wall->p2)
			return (list);
		list = list->next;
	}
	return (NULL);
}

/*
 * if the 2nd point in wall isnt the 1st point in 2nd wall, they in wrong order;
 * 		if the they are just flipped, just flip them back;
 * 		else the wall is in the wrong place, so lets move stuff around;
*/
void	sort_walls(t_list *list)
{
	t_list	*curr;
	t_wall	*w1;
	t_wall	*w2;
	t_list	*correct;

	curr = list;
	while (curr && curr->next)
	{
		w1 = curr->content;
		w2 = curr->next->content;
		if (w1->p1 != w2->p2)
		{
			if (w1->p1 == w2->p1)
				pointer_swap(w2->p1, w2->p2);
			else
			{
				correct = get_next_wall_node(list, w1);
				curr->next->content = correct->content;
				correct->content = w2;
			}
		}
		curr = curr->next;
	}
}

float	distance_from_vector_to_wall(t_vec2i p0, t_wall *wall)
{
	t_vec2i		p1;
	t_vec2i		p2;
	float		dist;
	float		up;
	float		down;

	p1 = wall->p1->pos;
	p2 = wall->p2->pos;
	up = (p2.x - p1.x) * (p1.y - p0.y)
		- (p1.x - p0.x) * (p2.y - p1.y);
	down = sqrt(ft_pow(p2.x - p1.x, 2) + ft_pow(p2.y - p1.y, 2));
	dist = up / down;
	return (dist);
}

int	vector_in_rect_of_radius(t_vec2i p, t_vec2i v1, t_vec2i v2, float radius)
{
	float	min;
	float	max;

	min = fmin(v1.x, v2.x);
	max = fmax(v1.x, v2.x);
	if (p.x >= min - radius
		&& p.x <= max + radius)
	{
		min = fmin(v1.y, v2.y);
		max = fmax(v1.y, v2.y);
		if (p.y >= min - radius
			&& p.y <= max + radius)
			return (1);
	}
	return (0);
}

t_wall	*get_wall_with_id(t_list *list, int id)
{
	while (list)
	{
		if (((t_wall *)list->content)->id == id)
			return (list->content);
		list = list->next;
	}
	return (NULL);
}

t_wall	*get_wall_from_list_around_radius(
		t_list *list, t_vec2i pos, float allowed_radius)
{
	t_wall		*wall;
	float		dist;

	while (list)
	{
		wall = list->content;
		dist = fabs(distance_from_vector_to_wall(pos, wall));
		if (dist <= allowed_radius)
			if (vector_in_rect_of_radius(pos, wall->p1->pos, wall->p2->pos,
					allowed_radius))
				return (list->content);
		list = list->next;
	}
	return (NULL);
}

t_vec2i	get_wall_middle(t_wall *wall)
{
	t_vec2i	middle;

	middle.x = (wall->p1->pos.x + wall->p2->pos.x) / 2;
	middle.y = (wall->p1->pos.y + wall->p2->pos.y) / 2;
	return (middle);
}

void	remove_sprite_from_wall(t_sprite *sprite, t_wall *wall)
{
	t_list	*curr;

	curr = wall->sprites;
	while (curr)
	{
		if (curr->content == sprite)
		{
			free(curr->content);
			ft_lstdelone_nonfree(&wall->sprites, curr);
		}
		curr = curr->next;
	}
}

/*
 * which == 0 floor;
 * 		== 1 ceiling;
*/
void	remove_wall_list_angles(t_list *list, int which)
{
	t_wall	*wall;

	while (list)
	{
		wall = list->content;
		if (which == 0)
			wall->floor_angle = 0;
		else if (which == 1)
			wall->ceiling_angle = 0;
		list = list->next;
	}
}

t_wall	*get_connected_wall(t_list *list, t_wall *wall)
{
	t_wall	*dest_wall;

	while (list)
	{
		dest_wall = list->content;
		if (dest_wall->p1 == wall->p2)
			return (dest_wall);
		list = list->next;
	}
	return (NULL);
}

///////////////////
// Cleanup
//////////////////

int	wall_in_sector(t_sector *sector, t_wall *wall)
{
	t_list	*curr;

	if (!wall)
	{
		ft_printf("[%s] Wall == NULL\n", __FUNCTION__);
		return (-1);
	}
	curr = sector->walls;
	while (curr)
	{
		if (curr->content == wall)
			return (1);
		curr = curr->next;
	}
	return (0);
}

int	wall_in_any_sector(t_list *sectors, t_wall *wall)
{
	if (!wall)
	{
		ft_printf("[%s] Wall == NULL\n", __FUNCTION__);
		return (-1);
	}
	while (sectors)
	{
		if (wall_in_sector(sectors->content, wall))
			return (1);
		sectors = sectors->next;
	}
	return (0);
}

/*
 * Removes all walls not a part of a sector;
 * Remove all incomplete walls;
*/
void	remove_all_lonely_walls(t_editor *editor)
{
	t_wall	*wall;
	t_list	*curr;
	t_list	*next;

	curr = editor->walls;
	while (curr)
	{
		wall = curr->content;
		next = curr->next;
		if (!wall_in_any_sector(editor->sectors, wall)
			|| wall->p1 == NULL || wall->p2 == NULL)
			remove_wall(editor, curr->content);
		curr = next;
	}
}

void	activate_correct_wall_skybox_button(t_editor *editor, t_wall *wall)
{
	if (wall->skybox == -1)
		ui_dropdown_activate(editor->wall_skybox_dropdown,
			editor->wall_skybox_one);
	else if (wall->skybox == -2)
		ui_dropdown_activate(editor->wall_skybox_dropdown,
			editor->wall_skybox_two);
	else if (wall->skybox == -3)
		ui_dropdown_activate(editor->wall_skybox_dropdown,
			editor->wall_skybox_three);
	else
		ui_dropdown_activate(editor->wall_skybox_dropdown,
			editor->wall_skybox_none);
}

/*
 * When you select new wall update ui;
*/
void	set_wall_ui(t_editor *editor, t_wall *wall)
{
	char	temp_str[20];

	activate_correct_wall_skybox_button(editor, wall);
	editor->wall_texture_something.id = wall->wall_texture;
	editor->portal_texture_something.id = wall->portal_texture;
	ui_element_image_set(editor->wall_texture_image, UI_STATE_AMOUNT,
		editor->wall_textures[wall->wall_texture]);
	ui_element_image_set(editor->portal_texture_image, UI_STATE_AMOUNT,
		editor->wall_textures[wall->portal_texture]);
	ui_checkbox_toggle_accordingly(editor->solid_checkbox, wall->solid);
	ui_checkbox_toggle_accordingly(editor->portal_checkbox, wall->neighbor);
	ui_input_set_text(editor->floor_wall_angle_input,
		ft_b_itoa(wall->floor_angle, temp_str));
	ui_input_set_text(editor->ceiling_wall_angle_input,
		ft_b_itoa(wall->ceiling_angle, temp_str));
	ui_input_set_text(editor->wall_texture_scale_input,
		ft_b_ftoa(wall->texture_scale, 2, temp_str));
}

/*
 * Update ui when you edit wall;
 *
 * TODO : split this function into multiple functions ( adds convolution imo)
*/
void	get_wall_ui2(t_editor *editor, t_wall *wall)
{
	int				angle;
	char			temp_str[20];

	if (ui_input_exit(editor->floor_wall_angle_input))
	{
		angle = ft_clamp(ft_atoi(ui_input_get_text(
						editor->floor_wall_angle_input)), -45, 45);
		if (angle != 0)
			remove_wall_list_angles(wall->parent_sector->walls, 0);
		wall->floor_angle = angle;
		ft_b_itoa(wall->floor_angle, temp_str);
		ui_input_set_text(editor->floor_wall_angle_input, temp_str);
	}
	if (ui_input_exit(editor->ceiling_wall_angle_input))
	{
		angle = ft_clamp(ft_atoi(ui_input_get_text(
						editor->ceiling_wall_angle_input)), -45, 45);
		if (angle != 0)
			remove_wall_list_angles(wall->parent_sector->walls, 1);
		wall->ceiling_angle = angle;
		ft_b_itoa(wall->ceiling_angle, temp_str);
		ui_input_set_text(editor->ceiling_wall_angle_input, temp_str);
	}
}

void	get_wall_ui3(t_editor *editor, t_wall *wall)
{
	char			temp_str[20];

	if (ui_input_exit(editor->wall_texture_scale_input))
	{
		wall->texture_scale = ft_fclamp(ft_atof(ui_input_get_text(
						editor->wall_texture_scale_input)), 0.1f, 100.0f);
		ft_b_ftoa(wall->texture_scale, 2, temp_str);
		ui_input_set_text(editor->wall_texture_scale_input, temp_str);
	}
}

void	get_wall_ui(t_editor *editor, t_wall *wall)
{
	t_ui_element	*skybox_active;

	if (ui_dropdown_exit(editor->wall_skybox_dropdown))
	{
		skybox_active = ui_dropdown_active(editor->wall_skybox_dropdown);
		if (skybox_active == editor->wall_skybox_none)
			wall->skybox = 0;
		if (skybox_active == editor->wall_skybox_one)
			wall->skybox = -1;
		if (skybox_active == editor->wall_skybox_two)
			wall->skybox = -2;
		if (skybox_active == editor->wall_skybox_three)
			wall->skybox = -3;
	}
	wall->wall_texture = editor->wall_texture_something.id;
	wall->portal_texture = editor->portal_texture_something.id;
	if (!editor->portal_checkbox->is_toggle)
		wall->neighbor = NULL;
	if (editor->portal_checkbox->is_toggle)
		if (!can_you_make_portal_of_this_wall(editor->sectors,
				wall->parent_sector, wall))
			ui_checkbox_toggle_off(editor->portal_checkbox);
	if (!wall->neighbor)
		ui_checkbox_toggle_on(editor->solid_checkbox);
	wall->solid = editor->solid_checkbox->is_toggle;
	get_wall_ui2(editor, wall);
	get_wall_ui3(editor, wall);
}

void	split_wall(t_editor *editor, t_sector *sector, t_wall *wall)
{
	t_point	*p1;
	t_point	*p3;

	p1 = wall->p1;
	p3 = add_point(editor);
	p3->pos = get_wall_middle(wall);
	wall->p1 = p3;
	add_wall_to_sector_at_pos(editor, sector, p1->pos, p3->pos);
}

void	draw_walls(t_editor *editor, t_list *walls, Uint32 color)
{
	t_wall	*wall;

	while (walls)
	{
		wall = walls->content;
		wall->width = distance(wall->p1->pos.v, wall->p2->pos.v, 2);
		if (!wall->parent_sector)
			ft_printf("[%s] Error: No parent_sector.\n", __FUNCTION__);
		else
			wall->height = wall->parent_sector->ceiling_height
				- wall->parent_sector->floor_height;
		if (wall->neighbor)
		{
			if (get_sector_wall_at_pos(wall->neighbor,
					wall->p1->pos, wall->p2->pos))
				wall_render(editor, wall, 0xffff0000);
			else
				wall->neighbor = NULL;
		}
		else
			wall_render(editor, wall, color);
		point_render(editor, wall->p1, color);
		walls = walls->next;
	}
}

bool	can_you_make_portal_of_this_wall(
		t_list *sector_list, t_sector *part_of_sector, t_wall *wall)
{
	t_sector	*sector;
	t_wall		*temp_wall;

	if (!part_of_sector || !wall)
		return (0);
	while (sector_list)
	{
		sector = sector_list->content;
		if (sector != part_of_sector)
		{
			temp_wall = get_sector_wall_at_pos(sector,
					wall->p1->pos, wall->p2->pos);
			if (temp_wall)
			{
				wall->neighbor = sector;
				temp_wall->neighbor = part_of_sector;
				return (1);
			}
		}
		sector_list = sector_list->next;
	}
	return (0);
}

t_sprite	*add_sprite_to_wall(t_editor *editor, t_wall *wall)
{
	t_sprite	*sprite;

	sprite = add_sprite(editor);
	if (editor->active_texture_button_id > -1)
		sprite->texture_id = editor->active_texture_button_id;
	sprite->parent = wall;
	add_to_list(&wall->sprites, sprite, sizeof(t_sprite));
	++wall->sprite_amount;
	return (sprite);
}
