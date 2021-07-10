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

void	wall_render(t_editor *editor)
{
	t_grid *grid;
	t_wall *wall;
	t_sector *sec;
	t_xywh dim;
	SDL_Surface	*temp_texture;
	SDL_Rect	texture;
	SDL_Rect	temp;
	SDL_Surface	*scaled_wall;
	float	scale;
	float	x_axis;
	float	y_axis;

	SDL_FillRect(editor->edit_view_wall->active_surface, NULL, 0xffdee2d9);
	grid = &editor->grid;
	wall = grid->modify_wall;

	// make the wall surface the size of the actual wall.
	sec = get_sector_with_wall(&editor->grid.sectors, wall);
	dim.h = (sec->ceiling_height - sec->floor_height) * 10;
	dim.w = (gfx_distance(wall->orig->pos, wall->dest->pos) * editor->scale) * 10;
	SDL_Surface *the_wall = create_surface(dim.w, dim.h);
	ft_printf("%d %d\n", dim.w, dim.h);

	if (wall->texture_id < 0 || wall->texture_id >= editor->texture_amount) 
		temp_texture = editor->texture_textures[0];
	else
		temp_texture = editor->texture_textures[wall->texture_id];

	x_axis = ((float)dim.w / (temp_texture->w * (float)wall->texture_scale));
	y_axis = ((float)dim.h / (temp_texture->h * (float)wall->texture_scale));

	ft_printf("temp_texutre-> ; %d %d\n",temp_texture->w, temp_texture->h);
	ft_printf("_axis ; %.1f %.1f\n", x_axis, y_axis);
	for (int y = 0; y < y_axis; y++)
	{
		for (int x = 0; x < x_axis; x++)
		{
			temp.w = temp_texture->w * editor->grid.modify_wall->texture_scale;
			temp.h = temp_texture->h * editor->grid.modify_wall->texture_scale;
			temp.x = x * temp.w;
			temp.y = y * temp.h;
			SDL_BlitScaled(temp_texture, NULL, the_wall, &temp);
		}
	}

// render the objects placed on the wall
	t_list		*curr;
	t_sprite	*sprite;
	SDL_Surface	*temp_sprite;

	curr = wall->sprites;
	while (curr)
	{
		sprite = curr->content;
		temp_sprite =
			editor->texture_textures[sprite->sprite_id];
		sprite->coord = ui_init_coords(
			sprite->real_x, sprite->real_y,
			sprite->scale * 10, sprite->scale * 10);
		SDL_BlitScaled(temp_sprite, NULL, the_wall, &(SDL_Rect){
			sprite->coord.x, sprite->coord.y,
			sprite->coord.w, sprite->coord.h});
		curr = curr->next;
	}
	if (editor->grid.modify_sprite != NULL)
		draw_rect_border(the_wall,
			(t_xywh){editor->grid.modify_sprite->coord.x,
			editor->grid.modify_sprite->coord.y,
			editor->grid.modify_sprite->coord.w,
			editor->grid.modify_sprite->coord.h}, 0xff0000ff, 1);

// finally blit the wall to the surface of the window
	SDL_Surface	*ent_tex;
	SDL_Surface	*dst_surf;
	SDL_Rect	rect;
	float		aspect;

	ent_tex = the_wall;
	dst_surf = editor->edit_view_wall->active_surface;
	aspect = ft_fmin((float)dst_surf->w / (float)ent_tex->w,
		(float)dst_surf->h / (float)ent_tex->h);
	rect.w = ent_tex->w * aspect;
	rect.h = ent_tex->h * aspect;
	rect.x = (dst_surf->w - rect.w) / 2;
	rect.y = (dst_surf->h - rect.h) / 2;
	SDL_BlitScaled(the_wall, NULL,
		editor->edit_view_wall->active_surface, &rect);
	gfx_draw_rect(editor->edit_view_wall->active_surface, 0xff00ff00,
		(t_xywh){rect.x, rect.y, rect.w, rect.h});
	SDL_FreeSurface(the_wall);
	ft_printf("%d %d\n", dst_surf->w, dst_surf->h);
	ft_printf("%d %d %d %d\n", rect.x, rect.y, rect.w, rect.h);
}


// @Improvement: blit everything on the normal sized wall and then finally scale it up -.-
void	old_wall_render(t_editor *doom)
{
	t_grid *grid;
	t_wall *wall;
	t_sector *sec;
	t_xywh dim;
	SDL_Surface	*temp_texture;
	SDL_Rect	texture;
	SDL_Rect	temp;
	SDL_Surface	*scaled_wall;
	float	scale;
	float	x_axis;
	float	y_axis;

	SDL_FillRect(doom->edit_view_wall->active_surface,
		&(SDL_Rect){0, 0,
		doom->edit_view_wall->position.w,
		doom->edit_view_wall->position.h}, 0xff000000);
	grid = &doom->grid;
	wall = grid->modify_wall;
	// get w from the difference from the orig -> dest x
	dim.w = gfx_distance(wall->orig->pos, wall->dest->pos) * doom->scale; // NOTE: this '* doom->scale', means what one value on the grid is in the game
	// get h from the sector it is a part of
	// get the sector that the wall is a part of... and then take the height from it
	sec = get_sector_with_wall(&doom->grid.sectors, wall);
	dim.h = sec->ceiling_height - sec->floor_height;
	// all of these values needs to  have a zoom value depending on which is...
	//	more the x or the y so that we can see the whole wall...
	//	MAYBE: but if you after that want to zoom you can??
	dim.x = 50; // offsets, because of the position of the wall on the surface of the view element.
	dim.y = 50;

	// render wall with the texture
	if (wall->texture_id < 0 || wall->texture_id >= doom->texture_amount) // this is just to check that the wall has a texture id between 0 and the maximum loaded textures, otherwise sgflt
	{
		// TODO: instaed of doing this every time should we do this in the getter, or if it once comes here you just put the texture id to default value and not just select the default id texture every loop.
		ft_putstr("[wall_render]\n");
		ft_printf("[Error] Wall Texture ID : %d is not valid!\n", wall->texture_id);
		ft_putstr("Defaulting to texture id : 0\n");
		temp_texture = doom->texture_textures[0];
	}
	else
		temp_texture = doom->texture_textures[wall->texture_id];
	texture.x = 0;
	texture.y = 0;
	texture.w = temp_texture->w; 
	texture.h = temp_texture->h;

	// how many of the textures can fit on each axis
	x_axis = ceil((float)dim.w / (float)doom->grid.modify_wall->texture_scale);
	y_axis = ceil((float)dim.h / (float)doom->grid.modify_wall->texture_scale);

	scale = (doom->edit_view_wall->position.w - (dim.x * 2)) / dim.w;
	scaled_wall = create_surface(dim.w * scale, dim.h * scale);
	for (int y = 0; y < y_axis; y++)
	{
		for (int x = 0; x < x_axis; x++)
		{
			temp.w = scale * doom->grid.modify_wall->texture_scale;
			temp.h = scale * doom->grid.modify_wall->texture_scale;
			temp.x = x * temp.w;
			temp.y = y * temp.h;
			SDL_BlitScaled(temp_texture, &texture, scaled_wall, &temp);
		}
	}

// render the objects placed on the wall
	t_list		*curr;
	t_sprite	*sprite;
	SDL_Surface	*temp_sprite;

	curr = wall->sprites;
	while (curr)
	{
		sprite = curr->content;
		temp_sprite =
			doom->texture_textures[sprite->sprite_id];
		sprite->coord = ui_init_coords(
			scale * sprite->real_x, scale * sprite->real_y,
			scale * sprite->scale, scale * sprite->scale);
		SDL_BlitScaled(temp_sprite, NULL, scaled_wall, &(SDL_Rect){
			sprite->coord.x, sprite->coord.y,
			sprite->coord.w, sprite->coord.h});
		curr = curr->next;
	}
	if (doom->grid.modify_sprite != NULL)
		draw_rect_border(scaled_wall,
			(t_xywh){doom->grid.modify_sprite->coord.x,
			doom->grid.modify_sprite->coord.y,
			doom->grid.modify_sprite->coord.w,
			doom->grid.modify_sprite->coord.h}, 0xff0000ff, 2);

// finally blit the wall to the surface of the window
	SDL_BlitSurface(scaled_wall, NULL, doom->edit_view_wall->active_surface,
		&(SDL_Rect){dim.x, dim.y, dim.w * scale, dim.h * scale});
	gfx_draw_rect(doom->edit_view_wall->active_surface, 0xff00ff00,
		(t_xywh){dim.x, dim.y, dim.w * scale, dim.h * scale});
	SDL_FreeSurface(scaled_wall);
}

void	wall_option(t_editor *editor, t_bui_libui *libui)
{
	char *temp;

	editor->edit_view_wall->show = 1;
	editor->edit_toolbox_wall->show = 1;

	preset_tab_events(editor->wall_tab);

	// NOTE: the newer way libui functions, is that all the elements will be event handled no matter what,
	// 		so we have to actually check here if the texture view is open before we use the events to our nytta.
	// check all the wall texture view elements
	if (editor->wall_texture_view->show == 1)
	{
			// texture scale
		changer_prefab_events_float(editor->texture_scale_changer, &editor->grid.modify_wall->texture_scale, 0.1f);
		editor->grid.modify_wall->texture_scale = ft_fclamp(editor->grid.modify_wall->texture_scale, 0.1f, 64.0f);
			// wall solidity tick box
		editor->wall_solid_tick->toggle = editor->grid.modify_wall->solid;
		if (bui_button(editor->wall_solid_tick))
		{
			if (editor->wall_solid_tick->toggle == 1)
				editor->grid.modify_wall->solid = 0;
			else
				editor->grid.modify_wall->solid = 1;
		}
			// wall portal tick box
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
		// TODO: first step is to take from the "modify_wall" the "texture_id" and toggle that texture button
		// TODO: second step is to check for the mouse event on the buttons
		// TODO: third step is to set the modify_wall texture id with the correct one.
		// NOTE: i think this is pretty spaghett, not pretty spaghett but pretty spaghett if you know what i mean.
			// texture buttons
		temp = ft_itoa(editor->grid.modify_wall->texture_id);
		if (only_one_button_toggled_at_a_time(editor->wall_texture_buttons, &editor->active_wall_texture))
		{}
		else
			toggle_on_element_with_text(editor->wall_texture_buttons, &editor->active_wall_texture, temp);
		ft_strdel(&temp);
		if (editor->active_wall_texture != NULL)
			editor->grid.modify_wall->texture_id = ft_atoi(editor->active_wall_texture->text);
	} ///// !! END OF TEXTURE_VIEW  !! ////
	else if (editor->portal_texture_view->show == 1)
	{
			// portal textures
		temp = ft_itoa(editor->grid.modify_wall->portal_texture_id);
		if (only_one_button_toggled_at_a_time(editor->portal_texture_buttons, &editor->active_portal_texture))
		{}
		else
			toggle_on_element_with_text(editor->portal_texture_buttons, &editor->active_portal_texture, temp);
		ft_strdel(&temp);
		if (editor->active_portal_texture != NULL)
			editor->grid.modify_wall->portal_texture_id = ft_atoi(editor->active_portal_texture->text);
	} // !! END OF PORTAL_TEXUTRE_VIEW !! //
	else if (editor->wall_sprite_view->show == 1)
	{
			// wall sprites
		/// adding
		if (bui_button(editor->add_wall_sprite_button))
		{
			t_sprite *sprite;

			sprite = new_sprite();
			sprite->sprite_id = 0; // default 0 texture
			add_to_list(&editor->grid.modify_wall->sprites, sprite, sizeof(t_sprite));
			editor->grid.modify_sprite = sprite;
		}

		// the button change thingy
		if (editor->grid.modify_sprite != NULL)
		{
			temp = ft_itoa(editor->grid.modify_sprite->sprite_id);
			if (only_one_button_toggled_at_a_time(editor->wall_sprite_buttons, &editor->active_wall_sprite))
			{}
			else
				toggle_on_element_with_text(editor->wall_sprite_buttons, &editor->active_wall_sprite, temp);
			ft_strdel(&temp);
			if (editor->active_wall_sprite != NULL)
				editor->grid.modify_sprite->sprite_id = ft_atoi(editor->active_wall_sprite->text);
		}
		// Choose sprite
		if (editor->grid.modify_wall->sprites != NULL)
		{
			changer_prefab_events(editor->sprite_changer, &editor->selected_sprite, 1);
			if (editor->selected_sprite >= get_list_len(&editor->grid.modify_wall->sprites))
				editor->selected_sprite = 0;
			else if (editor->selected_sprite < 0)
				editor->selected_sprite = get_list_len(&editor->grid.modify_wall->sprites) - 1;
			editor->grid.modify_sprite = get_nth_from_list(
				&editor->grid.modify_wall->sprites,
				editor->selected_sprite)->content;
		}
		// Move the sprite
		if (editor->grid.modify_sprite != NULL)
		{
			float move_speed = 1;
			if (key_pressed(libui, KEY_LEFT))
				editor->grid.modify_sprite->real_x -= move_speed;
			else if (key_pressed(libui, KEY_RIGHT))
				editor->grid.modify_sprite->real_x += move_speed;
			if (key_pressed(libui, KEY_UP))
				editor->grid.modify_sprite->real_y -= move_speed;
			else if (key_pressed(libui, KEY_DOWN))
				editor->grid.modify_sprite->real_y += move_speed;

			// the sprite scale changer
			changer_prefab_events_float(editor->sprite_scale_changer, &editor->grid.modify_sprite->scale, 0.1f);
			
			// remove sprite button
			if (bui_button(editor->remove_wall_sprite_button))
			{
				ft_putstr("Removing sprite from wall.\n");
				// TODO: make function for removing and freeing wall sprite.
				remove_from_sprites(&editor->grid.modify_wall->sprites, editor->grid.modify_sprite);
				editor->grid.modify_sprite = NULL;
			}
		}
	} // !! END OF WALL_SPRITE_VIEW !! //
	// NOTE: in this function you also render the wall sprites.
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

void	sector_option(t_editor *editor, t_grid *grid)
{
	char *ceil_tex;
	char *floor_tex;

	editor->sector_texture_menu->show = 1;
	editor->edit_toolbox_sector->show = 1;
	editor->slope_edit_menu->show = 1;

	// manually event handle these buttons.
	t_list	*curr;

	ceil_tex = ft_itoa(editor->grid.modify_sector->ceiling_texture);
	floor_tex = ft_itoa(editor->grid.modify_sector->floor_texture);
	editor->active_ceiling_texture =
		bui_get_element_with_text_from_list(editor->sector_texture_buttons, ceil_tex);
	editor->active_floor_texture =
		bui_get_element_with_text_from_list(editor->sector_texture_buttons, floor_tex);
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
	t_xywh	c;

	if (editor->active_floor_texture)
	{
		c = ui_init_coords(0, 0,
			editor->active_floor_texture->active_surface->w,
			editor->active_floor_texture->active_surface->h);
		draw_rect_border(editor->active_floor_texture->active_surface, c, 0xff0000ff, 2);
	}
	if (editor->active_ceiling_texture)
	{
		c = ui_init_coords(2, 2,
			editor->active_ceiling_texture->active_surface->w - 4,
			editor->active_ceiling_texture->active_surface->h - 4);
		draw_rect_border(editor->active_ceiling_texture->active_surface, c, 0xff00ff00, 2);
	}
	editor->grid.modify_sector->floor_texture = ft_atoi(editor->active_floor_texture->text);
	editor->grid.modify_sector->ceiling_texture = ft_atoi(editor->active_ceiling_texture->text);
	changer_prefab_events(editor->floor_height, &grid->modify_sector->floor_height, 1);
	changer_prefab_events(editor->ceiling_height, &grid->modify_sector->ceiling_height, 1);
	changer_prefab_events(editor->gravity, &grid->modify_sector->gravity, 1);
	changer_prefab_events(editor->lighting, &grid->modify_sector->light_level, 1);
	changer_prefab_events_float(editor->floor_scale, &grid->modify_sector->floor_texture_scale, 0.1f);
	changer_prefab_events_float(editor->ceiling_scale, &grid->modify_sector->ceiling_texture_scale, 0.1f);

	// draw the sector and event handle the wall choosing.
	// TODO: figureo ut how to fit any size of sector on the small element.
	t_wall	*long_wall;
	t_wall	*wall;
	float	long_wall_dist;
	float	scale_ratio;

	curr = editor->grid.modify_sector->walls;
	long_wall = get_longest_wall_from_list(curr);
	long_wall_dist = gfx_distance(long_wall->orig->pos, long_wall->dest->pos);
	scale_ratio = editor->slope_sector->position.h / long_wall_dist;

	t_vector a = gfx_vector_divide(editor->grid.modify_sector->center, editor->grid.gap);
	float dist_to_middle_x = (editor->slope_sector->position.w / 2) - a.x;
	float dist_to_middle_y = (editor->slope_sector->position.h / 2) - a.y;
	t_vector new_orig; 
	t_vector new_dest;
	while (curr)
	{
		wall = curr->content;

		new_orig = wall->orig->pos;
		new_dest = wall->dest->pos;

		new_orig = (t_vector){new_orig.x - a.x, new_orig.y - a.y, 0};
		new_orig = gfx_vector_multiply(new_orig, scale_ratio - 1);
		new_orig = (t_vector){new_orig.x + a.x, new_orig.y + a.y, 0};

		new_dest = (t_vector){new_dest.x - a.x, new_dest.y - a.y, 0};
		new_dest = gfx_vector_multiply(new_dest, scale_ratio - 1);
		new_dest = (t_vector){new_dest.x + a.x, new_dest.y + a.y, 0};

		new_orig.x += dist_to_middle_x;
		new_orig.y += dist_to_middle_y;
		new_dest.x += dist_to_middle_x;
		new_dest.y += dist_to_middle_y;
		
		gfx_draw_line(editor->slope_sector->active_surface,
		editor->grid.modify_sector->color,
		new_orig, new_dest);
		curr = curr->next;
	}
	// the changers.
	t_wall *temp_wall; 
	// floor wall id changer
	changer_prefab_events(editor->slope_floor_wall_changer, &editor->grid.modify_sector->floor_slope_wall_id, 1);
	// NOTE: you cant use clamp, because you want it to wrap around.
	int wall_amount = get_list_len(&editor->grid.modify_sector->walls);
	if (editor->grid.modify_sector->floor_slope_wall_id >= wall_amount)
		editor->grid.modify_sector->floor_slope_wall_id = 0;
	else if (editor->grid.modify_sector->floor_slope_wall_id < 0)
		editor->grid.modify_sector->floor_slope_wall_id = wall_amount - 1;
	temp_wall = get_nth_from_list(&editor->grid.modify_sector->walls, editor->grid.modify_sector->floor_slope_wall_id)->content;

	new_orig = temp_wall->orig->pos;
	new_dest = temp_wall->dest->pos;

	new_orig = (t_vector){new_orig.x - a.x, new_orig.y - a.y, 0};
	new_orig = gfx_vector_multiply(new_orig, scale_ratio - 1.5f);
	new_orig = (t_vector){new_orig.x + a.x, new_orig.y + a.y, 0};

	new_dest = (t_vector){new_dest.x - a.x, new_dest.y - a.y, 0};
	new_dest = gfx_vector_multiply(new_dest, scale_ratio - 1.5f);
	new_dest = (t_vector){new_dest.x + a.x, new_dest.y + a.y, 0};

	new_orig.x += dist_to_middle_x;
	new_orig.y += dist_to_middle_y;
	new_dest.x += dist_to_middle_x;
	new_dest.y += dist_to_middle_y;

	gfx_draw_line(editor->slope_sector->active_surface,
	0xff0000ff,
	new_orig, new_dest);

	// ceiling wall id changer
	changer_prefab_events(editor->slope_ceiling_wall_changer, &editor->grid.modify_sector->ceiling_slope_wall_id, 1);
	if (editor->grid.modify_sector->ceiling_slope_wall_id >= wall_amount)
		editor->grid.modify_sector->ceiling_slope_wall_id = 0;
	else if (editor->grid.modify_sector->ceiling_slope_wall_id < 0)
		editor->grid.modify_sector->ceiling_slope_wall_id = wall_amount - 1;
	temp_wall = get_nth_from_list(&editor->grid.modify_sector->walls, editor->grid.modify_sector->ceiling_slope_wall_id)->content;

	new_orig = temp_wall->orig->pos;
	new_dest = temp_wall->dest->pos;

	new_orig = (t_vector){new_orig.x - a.x, new_orig.y - a.y, 0};
	new_orig = gfx_vector_multiply(new_orig, scale_ratio - 0.5f);
	new_orig = (t_vector){new_orig.x + a.x, new_orig.y + a.y, 0};

	new_dest = (t_vector){new_dest.x - a.x, new_dest.y - a.y, 0};
	new_dest = gfx_vector_multiply(new_dest, scale_ratio - 0.5f);
	new_dest = (t_vector){new_dest.x + a.x, new_dest.y + a.y, 0};

	new_orig.x += dist_to_middle_x;
	new_orig.y += dist_to_middle_y;
	new_dest.x += dist_to_middle_x;
	new_dest.y += dist_to_middle_y;

	gfx_draw_line(editor->slope_sector->active_surface,
	0xff00ff00,
	new_orig, new_dest);

	// floor slope angle changer
	changer_prefab_events(editor->slope_floor_angle_changer, &editor->grid.modify_sector->floor_slope, 1);
	if (editor->grid.modify_sector->floor_slope > 45)
		editor->grid.modify_sector->floor_slope = -45;
	else if (editor->grid.modify_sector->floor_slope < -45)
		editor->grid.modify_sector->floor_slope = 45;
	// ceiling slope angle changer
	changer_prefab_events(editor->slope_ceiling_angle_changer, &editor->grid.modify_sector->ceiling_slope, 1);
	if (editor->grid.modify_sector->ceiling_slope > 45)
		editor->grid.modify_sector->ceiling_slope = -45;
	else if (editor->grid.modify_sector->ceiling_slope < -45)
		editor->grid.modify_sector->ceiling_slope = 45;
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
		wall_option(doom, libui);
	else if (grid->modify_sector != NULL)
		sector_option(doom, grid);
	else if (grid->modify_entity != NULL)
		entity_option(doom);
}
