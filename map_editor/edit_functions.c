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

// @Improvement: blit everything on the normal sized wall and then finally scale it up -.-
void	wall_render(t_editor *doom)
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
	if (curr)
		temp_sprite =
			doom->sprite_textures[grid->modify_sprite->sprite_id];
	while (curr)
	{
		sprite = curr->content;
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
	
		// TODO: this doesnt take into account the scale of the sprite, it assumes that the scale is default, which means
		// 	if you mkae the scale bigger you can only choose the sprite from the top left corner of the sprite, and not
		// 	the actual whole sprite render. (change the mouse_hover to multiply the position with the scale)

		// Choose the sprite
		if (mouse_hover(editor->libui, editor->edit_view_wall->position) &&
		mouse_pressed(editor->libui, MKEY_LEFT))
		{
			int view_x = editor->libui->mouse_x - editor->edit_view_wall->position.x - 50;
			int view_y = editor->libui->mouse_y - editor->edit_view_wall->position.y - 50;
			t_sprite *temp;

			// Note: this function returns the sprite at that exact location, so you have to give
			// 	the exact location of where you want it to look if there is a sprite. thus the way you have
			// 	removed from the mouse_x and y the view location.
			temp = get_sprite_from_list(&editor->grid.modify_wall->sprites, view_x, view_y);
			if (temp != NULL)
			{
				ft_putstr("Sprite was successfully selected.\n");
				editor->grid.modify_sprite = temp;
			}
		}
		// Move the sprite
		if (editor->grid.modify_sprite != NULL)
		{
			float move_speed = 0.1;
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

void	sector_option(t_editor *editor, t_grid *grid)
{
	char *temp;

	editor->sector_ceiling_menu->show = 1;
	editor->sector_floor_menu->show = 1;
	editor->edit_toolbox_sector->show = 1;
	if (!only_one_button_toggled_at_a_time(editor->floor_texture_buttons, &editor->active_floor_texture))
	{
		temp = ft_itoa(editor->grid.modify_sector->floor_texture);
		toggle_on_element_with_text(editor->floor_texture_buttons, &editor->active_floor_texture, temp);
		ft_strdel(&temp);
	}
	if (!only_one_button_toggled_at_a_time(editor->ceiling_texture_buttons, &editor->active_ceiling_texture))
	{
		temp = ft_itoa(editor->grid.modify_sector->ceiling_texture);
		toggle_on_element_with_text(editor->ceiling_texture_buttons, &editor->active_ceiling_texture, temp);
		ft_strdel(&temp);
	}
	editor->grid.modify_sector->floor_texture = ft_atoi(editor->active_floor_texture->text);
	editor->grid.modify_sector->ceiling_texture = ft_atoi(editor->active_ceiling_texture->text);
	changer_prefab_events(editor->floor_height, &grid->modify_sector->floor_height, 1);
	changer_prefab_events(editor->ceiling_height, &grid->modify_sector->ceiling_height, 1);
	changer_prefab_events(editor->gravity, &grid->modify_sector->gravity, 1);
	changer_prefab_events(editor->lighting, &grid->modify_sector->light_level, 1);
	changer_prefab_events_float(editor->floor_scale, &grid->modify_sector->floor_texture_scale, 0.1f);
	changer_prefab_events_float(editor->ceiling_scale, &grid->modify_sector->ceiling_texture_scale, 0.1f);
}

// NOTE:
// fix when youre keeping entities.
void	entity_option(t_editor *editor)
{
	if (editor->grid.modify_entity == NULL)
		return ;

	editor->edit_toolbox_entity->show = 1;
	editor->edit_view_entity->show = 1;

	// NOTE: when you unselect the entity you should reset the active drop element
	// 	unfortunately this does so, the element isnt always clicked, only once.
	// NOTE: if the dropdown menu is open when you select another entity, it will automatically make that
	// 	the last clicked entity type... bug or feature? ANOTHER NOTE: it doesnt matter if the bass is dropped.
	if (preset_dropdown_events(editor->entity_type_drop))
	{}
	else
		toggle_on_element_with_text(editor->entity_type_drop->elements, &editor->entity_type_drop->active, editor->grid.modify_entity->preset->name); 
	if (editor->entity_type_drop->active != NULL)
	{
		t_entity_preset *preset = get_entity_preset_with_name(editor->entity_presets, editor->entity_type_drop->active->text);
		if (preset != NULL)
			editor->grid.modify_entity->preset = preset;
	}

	// direction radio buttons
	t_list *curr;

	curr = editor->entity_direction_radio_buttons;
	while (curr)
	{
		if (ft_atoi(((t_bui_element *)curr->content)->text) == editor->grid.modify_entity->direction)
			editor->active_direction_button = curr->content;
		curr = curr->next;
	}

	only_one_button_toggled_at_a_time(editor->entity_direction_radio_buttons, &editor->active_direction_button);
	editor->grid.modify_entity->direction = ft_atoi(editor->active_direction_button->text);
}

void	selected_option_menu(t_editor *doom, t_grid *grid, t_bui_libui *libui)
{
	doom->sector_ceiling_menu->show = 0;
	doom->sector_floor_menu->show = 0;
	doom->edit_toolbox_sector->show = 0;

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
