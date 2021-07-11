/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/05/17 12:43:53 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	render_sprites_on_the_wall(t_editor *editor, SDL_Surface *scaled_wall, int scale)
{
	t_list		*curr;
	t_sprite	*sprite;
	SDL_Surface	*temp_sprite;

	curr = editor->grid.modify_wall->sprites;
	while (curr)
	{
		sprite = curr->content;
		temp_sprite =
			editor->texture_textures[sprite->sprite_id];
		sprite->coord = ui_init_coords(
			scale * sprite->real_x, scale * sprite->real_y,
			scale * sprite->scale, scale * sprite->scale);
		SDL_BlitScaled(temp_sprite, NULL, scaled_wall, &(SDL_Rect){
			sprite->coord.x, sprite->coord.y,
			sprite->coord.w, sprite->coord.h});
		curr = curr->next;
	}
	if (editor->grid.modify_sprite != NULL)
		draw_rect_border(scaled_wall,
			(t_xywh){editor->grid.modify_sprite->coord.x,
			editor->grid.modify_sprite->coord.y,
			editor->grid.modify_sprite->coord.w,
			editor->grid.modify_sprite->coord.h}, 0xff0000ff, 2);
}

void	render_scaled_wall(t_editor *editor, SDL_Surface *scaled_wall)
{
	SDL_Surface	*ent_tex;
	SDL_Surface	*dst_surf;
	SDL_Rect	rect;
	float		aspect;

	ent_tex = scaled_wall;
	dst_surf = editor->edit_view_wall->active_surface;
	aspect = ft_fmin((float)dst_surf->w / (float)ent_tex->w,
		(float)dst_surf->h / (float)ent_tex->h);
	rect.w = ent_tex->w * aspect;
	rect.h = ent_tex->h * aspect;
	rect.x = (dst_surf->w - rect.w) / 2;
	rect.y = (dst_surf->h - rect.h) / 2;
	SDL_BlitScaled(scaled_wall, NULL,
		editor->edit_view_wall->active_surface, &rect);
	gfx_draw_rect(editor->edit_view_wall->active_surface, 0xff00ff00,
		(t_xywh){rect.x, rect.y, rect.w, rect.h});
	SDL_FreeSurface(scaled_wall);
}

void	wall_render(t_editor *editor)
{
	t_wall	*wall;
	t_sector	*sec;
	t_xywh	dim;
	SDL_Surface	*temp_texture;
	SDL_Surface	*scaled_wall;
	SDL_Rect	temp;
	float	scale;
	float	x_axis;
	float	y_axis;
	int	y;
	int	x;

	SDL_FillRect(editor->edit_view_wall->active_surface, NULL, 0xff000000);
	wall = editor->grid.modify_wall;
	dim.w = gfx_distance(wall->orig->pos, wall->dest->pos) * editor->scale;
	sec = get_sector_with_wall(&editor->grid.sectors, wall);
	dim.h = sec->ceiling_height - sec->floor_height;
	if (wall->texture_id < 0 || wall->texture_id >= editor->texture_amount)
		temp_texture = editor->texture_textures[0];
	else
		temp_texture = editor->texture_textures[wall->texture_id];
	x_axis = ((float)dim.w / (float)editor->grid.modify_wall->texture_scale);
	y_axis = ((float)dim.h / (float)editor->grid.modify_wall->texture_scale);
	scale = editor->edit_view_wall->position.w / dim.w;
	scaled_wall = create_surface(dim.w * scale, dim.h * scale);
	y = 0;
	while (y < y_axis)
	{
		x = 0;
		while (x < x_axis)
		{
			temp.w = scale * editor->grid.modify_wall->texture_scale;
			temp.h = scale * editor->grid.modify_wall->texture_scale;
			temp.x = x * temp.w;
			temp.y = y * temp.h;
			SDL_BlitScaled(temp_texture, NULL, scaled_wall, &temp);
			x++;
		}
		y++;
	}
	render_sprites_on_the_wall(editor, scaled_wall, scale);
	render_scaled_wall(editor, scaled_wall);
}

void	wall_texture_button_events(t_editor *editor)
{
	char	*temp;

	temp = ft_itoa(editor->grid.modify_wall->texture_id);
	if (only_one_button_toggled_at_a_time(editor->wall_texture_buttons,
		&editor->active_wall_texture))
	{}
	else
		toggle_on_element_with_text(editor->wall_texture_buttons,
			&editor->active_wall_texture, temp);
	ft_strdel(&temp);
	if (editor->active_wall_texture != NULL)
		editor->grid.modify_wall->texture_id =
			ft_atoi(editor->active_wall_texture->text);
}

void	wall_texture_view_events(t_editor *editor)
{
	changer_prefab_events_float(editor->texture_scale_changer,
		&editor->grid.modify_wall->texture_scale, 0.1f);
	editor->grid.modify_wall->texture_scale =
		ft_fclamp(editor->grid.modify_wall->texture_scale, 0.1f, 64.0f);
	editor->wall_solid_tick->toggle = editor->grid.modify_wall->solid;
	if (bui_button(editor->wall_solid_tick))
	{
		if (editor->wall_solid_tick->toggle == 1)
			editor->grid.modify_wall->solid = 0;
		else
			editor->grid.modify_wall->solid = 1;
	}
	if (editor->grid.modify_wall->neighbor != -1)
		editor->wall_portal_tick->toggle = 1;
	else
		editor->wall_portal_tick->toggle = 0;
	if (bui_button(editor->wall_portal_tick))
	{
		if (editor->wall_portal_tick->toggle == 1)
			remove_portal(&editor->grid);
		else
			add_portal(&editor->grid);
	}
	wall_texture_button_events(editor);
}

void	portal_texture_view_events(t_editor *editor)
{
	char	*temp;

	temp = ft_itoa(editor->grid.modify_wall->portal_texture_id);
	if (only_one_button_toggled_at_a_time(editor->portal_texture_buttons,
		&editor->active_portal_texture))
	{}
	else
		toggle_on_element_with_text(editor->portal_texture_buttons,
			&editor->active_portal_texture, temp);
	ft_strdel(&temp);
	if (editor->active_portal_texture != NULL)
		editor->grid.modify_wall->portal_texture_id =
			ft_atoi(editor->active_portal_texture->text);
}

void	move_selected_sprite(t_editor *editor)
{
	float	move_speed;

	move_speed = 0.1f;
	if (editor->grid.modify_sprite != NULL)
	{
		if (key_pressed(editor->libui, KEY_LEFT))
			editor->grid.modify_sprite->real_x -= move_speed;
		else if (key_pressed(editor->libui, KEY_RIGHT))
			editor->grid.modify_sprite->real_x += move_speed;
		if (key_pressed(editor->libui, KEY_UP))
			editor->grid.modify_sprite->real_y -= move_speed;
		else if (key_pressed(editor->libui, KEY_DOWN))
			editor->grid.modify_sprite->real_y += move_speed;
		changer_prefab_events_float(editor->sprite_scale_changer,
			&editor->grid.modify_sprite->scale, 0.1f);
		if (bui_button(editor->remove_wall_sprite_button))
		{
			remove_from_sprites(&editor->grid.modify_wall->sprites,
				editor->grid.modify_sprite);
			editor->grid.modify_sprite = NULL;
		}
	}
}

void	change_selected_sprite(t_editor *editor)
{
	if (editor->grid.modify_wall->sprites != NULL)
	{
		changer_prefab_events(editor->sprite_changer,
			&editor->selected_sprite, 1);
		if (editor->selected_sprite
			>= get_list_len(&editor->grid.modify_wall->sprites))
			editor->selected_sprite = 0;
		else if (editor->selected_sprite < 0)
			editor->selected_sprite = get_list_len(
				&editor->grid.modify_wall->sprites) - 1;
		editor->grid.modify_sprite = get_nth_from_list(
			&editor->grid.modify_wall->sprites,
			editor->selected_sprite)->content;
	}
}

void	change_selected_sprite_texture(t_editor *editor)
{
	char	*temp;

	if (editor->grid.modify_sprite != NULL)
	{
		temp = ft_itoa(editor->grid.modify_sprite->sprite_id);
		if (only_one_button_toggled_at_a_time(
			editor->wall_sprite_buttons,
			&editor->active_wall_sprite))
		{}
		else
			toggle_on_element_with_text(
				editor->wall_sprite_buttons,
				&editor->active_wall_sprite, temp);
		ft_strdel(&temp);
		if (editor->active_wall_sprite != NULL)
			editor->grid.modify_sprite->sprite_id =
				ft_atoi(editor->active_wall_sprite->text);
	}
}

void	wall_sprite_view_events(t_editor *editor)
{
	t_sprite	*sprite;

	if (bui_button(editor->add_wall_sprite_button))
	{

		sprite = new_sprite();
		sprite->sprite_id = 0;
		add_to_list(&editor->grid.modify_wall->sprites, sprite,
			sizeof(t_sprite));
		editor->grid.modify_sprite = sprite;
	}

	change_selected_sprite_texture(editor);
	change_selected_sprite(editor);
	move_selected_sprite(editor);
}

void	wall_option(t_editor *editor)
{
	editor->edit_view_wall->show = 1;
	editor->edit_toolbox_wall->show = 1;
	preset_tab_events(editor->wall_tab);
	if (editor->wall_texture_view->show == 1)
		wall_texture_view_events(editor);
	else if (editor->portal_texture_view->show == 1)
		portal_texture_view_events(editor);
	else if (editor->wall_sprite_view->show == 1)
		wall_sprite_view_events(editor);
	wall_render(editor);
}

t_wall	*get_longest_wall_from_list(t_list *list)
{
	t_list	*curr;
	t_wall	*wall;
	t_wall	*long_wall;
	int	temp_dist;
	int	curr_longest;

	curr = list;
	curr_longest = -2147483648;
	long_wall = NULL;
	while (curr)
	{
		wall = curr->content;
		temp_dist = gfx_distance(wall->orig->pos, wall->dest->pos);
		if (temp_dist > curr_longest)
		{
			curr_longest = temp_dist;
			long_wall = wall;
		}
		curr = curr->next;
	}
	return (long_wall);
}

void	sector_f_and_c_button_events(t_editor *editor)
{
	t_list	*curr;
	char	*ceil_tex;
	char	*floor_tex;

	ceil_tex = ft_itoa(editor->grid.modify_sector->ceiling_texture);
	floor_tex = ft_itoa(editor->grid.modify_sector->floor_texture);
	editor->active_ceiling_texture =
		bui_get_element_with_text_from_list(
		editor->sector_texture_buttons, ceil_tex);
	editor->active_floor_texture =
		bui_get_element_with_text_from_list(
		editor->sector_texture_buttons, floor_tex);
	ft_strdel(&ceil_tex);
	ft_strdel(&floor_tex);
	curr = editor->sector_texture_buttons;
	while (curr)
	{
		if (((t_bui_element *)curr->content)->was_clicked_last_frame)
		{
			if (mouse_pressed(editor->libui, MKEY_LEFT))
				editor->active_floor_texture = curr->content;
			else if (mouse_pressed(editor->libui, MKEY_RIGHT))
				editor->active_ceiling_texture = curr->content;
		}
		curr = curr->next;
	}
	editor->grid.modify_sector->floor_texture =
		ft_atoi(editor->active_floor_texture->text);
	editor->grid.modify_sector->ceiling_texture =
		ft_atoi(editor->active_ceiling_texture->text);
}

void	draw_selected_f_and_c_button(t_editor *editor)
{
	t_xywh	c;

	if (editor->active_floor_texture)
	{
		c = ui_init_coords(0, 0,
			editor->active_floor_texture->active_surface->w,
			editor->active_floor_texture->active_surface->h);
		draw_rect_border(editor->active_floor_texture->active_surface,
			c, 0xff0000ff, 2);
	}
	if (editor->active_ceiling_texture)
	{
		c = ui_init_coords(2, 2,
			editor->active_ceiling_texture->active_surface->w - 4,
			editor->active_ceiling_texture->active_surface->h - 4);
		draw_rect_border(
			editor->active_ceiling_texture->active_surface,
			c, 0xff00ff00, 2);
	}
}

void	sector_changer_prefab_events(t_editor *editor)
{
	changer_prefab_events(editor->floor_height,
		&editor->grid.modify_sector->floor_height, 1);
	changer_prefab_events(editor->ceiling_height,
		&editor->grid.modify_sector->ceiling_height, 1);
	changer_prefab_events(editor->gravity,
		&editor->grid.modify_sector->gravity, 1);
	changer_prefab_events(editor->lighting,
		&editor->grid.modify_sector->light_level, 1);
	changer_prefab_events_float(editor->floor_scale,
		&editor->grid.modify_sector->floor_texture_scale, 0.1f);
	changer_prefab_events_float(editor->ceiling_scale,
		&editor->grid.modify_sector->ceiling_texture_scale, 0.1f);
}

void	floor_ceiling_slope_changer_prefab_events(t_editor *editor)
{
	changer_prefab_events(editor->slope_floor_angle_changer,
		&editor->grid.modify_sector->floor_slope, 1);
	if (editor->grid.modify_sector->floor_slope > 45)
		editor->grid.modify_sector->floor_slope = -45;
	else if (editor->grid.modify_sector->floor_slope < -45)
		editor->grid.modify_sector->floor_slope = 45;
	changer_prefab_events(editor->slope_ceiling_angle_changer,
		&editor->grid.modify_sector->ceiling_slope, 1);
	if (editor->grid.modify_sector->ceiling_slope > 45)
		editor->grid.modify_sector->ceiling_slope = -45;
	else if (editor->grid.modify_sector->ceiling_slope < -45)
		editor->grid.modify_sector->ceiling_slope = 45;
}

t_vector	*get_scaled_line(SDL_Surface *surface, t_wall *wall, t_vector center, float scale)
{
	t_vector	new_orig;
	t_vector	new_dest;
	float		dist_to_middle_x;
	float		dist_to_middle_y;

	dist_to_middle_x = (surface->w / 2) - center.x;
	dist_to_middle_y = (surface->h / 2) - center.y;
	new_orig = wall->orig->pos;
	new_dest = wall->dest->pos;
	new_orig = (t_vector){new_orig.x - center.x, new_orig.y - center.y, 0};
	new_orig = gfx_vector_multiply(new_orig, scale);
	new_orig = (t_vector){new_orig.x + center.x, new_orig.y + center.y, 0};
	new_dest = (t_vector){new_dest.x - center.x, new_dest.y - center.y, 0};
	new_dest = gfx_vector_multiply(new_dest, scale);
	new_dest = (t_vector){new_dest.x + center.x, new_dest.y + center.y, 0};
	new_orig.x += dist_to_middle_x;
	new_orig.y += dist_to_middle_y;
	new_dest.x += dist_to_middle_x;
	new_dest.y += dist_to_middle_y;
	return ((t_vector []){new_orig, new_dest});
}

void	sector_option(t_editor *editor, t_grid *grid)
{
	editor->sector_texture_menu->show = 1;
	editor->edit_toolbox_sector->show = 1;
	editor->slope_edit_menu->show = 1;

	sector_f_and_c_button_events(editor);
	draw_selected_f_and_c_button(editor);
	sector_changer_prefab_events(editor);

	// draw the sector and event handle the wall choosing.
	t_list	*curr;
	t_wall	*long_wall;
	t_wall	*wall;
	t_vector *v;
	float	long_wall_dist;
	float	scale_ratio;

	curr = editor->grid.modify_sector->walls;
	long_wall = get_longest_wall_from_list(curr);
	long_wall_dist = gfx_distance(long_wall->orig->pos, long_wall->dest->pos);
	scale_ratio = editor->slope_sector->position.h / long_wall_dist;

	t_vector a = gfx_vector_divide(editor->grid.modify_sector->center, editor->grid.gap);
	while (curr)
	{
		wall = curr->content;
		v = get_scaled_line(editor->slope_sector->active_surface, wall, a, scale_ratio - 1.0f);
		gfx_draw_line(editor->slope_sector->active_surface, editor->grid.modify_sector->color, v[0], v[1]);
		curr = curr->next;
	}
	// the changers.
	t_wall *temp_wall; 
	int	wall_amount;
	// floor wall id changer
	changer_prefab_events(editor->slope_floor_wall_changer, &editor->grid.modify_sector->floor_slope_wall_id, 1);
	wall_amount = get_list_len(&editor->grid.modify_sector->walls);
	if (editor->grid.modify_sector->floor_slope_wall_id >= wall_amount)
		editor->grid.modify_sector->floor_slope_wall_id = 0;
	else if (editor->grid.modify_sector->floor_slope_wall_id < 0)
		editor->grid.modify_sector->floor_slope_wall_id = wall_amount - 1;
	temp_wall = get_nth_from_list(&editor->grid.modify_sector->walls, editor->grid.modify_sector->floor_slope_wall_id)->content;
	v = get_scaled_line(editor->slope_sector->active_surface, temp_wall, a, scale_ratio - 1.5f);
	gfx_draw_line(editor->slope_sector->active_surface, 0xff0000ff, v[0], v[1]);

	// ceiling wall id changer
	changer_prefab_events(editor->slope_ceiling_wall_changer, &editor->grid.modify_sector->ceiling_slope_wall_id, 1);
	if (editor->grid.modify_sector->ceiling_slope_wall_id >= wall_amount)
		editor->grid.modify_sector->ceiling_slope_wall_id = 0;
	else if (editor->grid.modify_sector->ceiling_slope_wall_id < 0)
		editor->grid.modify_sector->ceiling_slope_wall_id = wall_amount - 1;
	temp_wall = get_nth_from_list(&editor->grid.modify_sector->walls, editor->grid.modify_sector->ceiling_slope_wall_id)->content;
	v = get_scaled_line(editor->slope_sector->active_surface, temp_wall, a, scale_ratio - 0.5f);
	gfx_draw_line(editor->slope_sector->active_surface, 0xff00ff00, v[0], v[1]);

	floor_ceiling_slope_changer_prefab_events(editor);
}

void	draw_selected_entity_texture(t_editor *editor)
{
	SDL_Surface	*ent_tex;
	SDL_Surface	*dst_surf;
	SDL_Rect	rect;
	float		aspect;

	if (!editor->grid.modify_entity->preset->texture)
		return ;
	ent_tex = editor->grid.modify_entity->preset->texture;
	dst_surf = editor->edit_view_entity->active_surface;
	aspect = ft_fmin((float)dst_surf->w / (float)ent_tex->w,
		(float)dst_surf->h / (float)ent_tex->h);
	rect.w = ent_tex->w * aspect;
	rect.h = ent_tex->h * aspect;
	rect.x = (dst_surf->w - rect.w) / 2;
	rect.y = (dst_surf->h - rect.h) / 2;
	SDL_BlitScaled(editor->grid.modify_entity->preset->texture, NULL,
		editor->edit_view_entity->active_surface, &rect);
}

void	entity_option(t_editor *editor)
{
	t_list	*curr;

	if (editor->grid.modify_entity == NULL)
		return ;
	editor->edit_toolbox_entity->show = 1;
	editor->edit_view_entity->show = 1;
	// dorp
	editor->entity_type_drop->active = bui_get_element_with_text_from_list(editor->entity_type_drop->elements,
		editor->grid.modify_entity->preset->name);
	preset_dropdown_events(editor->entity_type_drop);
	if (editor->entity_type_drop->active != NULL)
	{
		editor->grid.modify_entity->preset = get_entity_preset_with_name(editor->entity_presets,
			editor->entity_type_drop->active->text);
	}
	// direction radio buttons
	curr = editor->entity_direction_radio_buttons;
	while (curr)
	{
		if (ft_atoi(((t_bui_element *)curr->content)->text) == editor->grid.modify_entity->direction)
			editor->active_direction_button = curr->content;
		curr = curr->next;
	}
	only_one_button_toggled_at_a_time(editor->entity_direction_radio_buttons, &editor->active_direction_button);
	editor->grid.modify_entity->direction = ft_atoi(editor->active_direction_button->text);
	// draw selected entity
	draw_selected_entity_texture(editor);
}

void	selected_option_menu(t_editor *doom, t_grid *grid, t_bui_libui *libui)
{
	doom->sector_texture_menu->show = 0;
	doom->edit_toolbox_sector->show = 0;
	doom->slope_edit_menu->show = 0;

	doom->edit_view_wall->show = 0;
	doom->edit_toolbox_wall->show = 0;

	doom->edit_toolbox_entity->show = 0;
	doom->edit_view_entity->show = 0;

	if (grid->modify_wall != NULL)
		wall_option(doom);
	else if (grid->modify_sector != NULL)
		sector_option(doom, grid);
	else if (grid->modify_entity != NULL)
		entity_option(doom);
}
