/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/05/10 15:38:38 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

// @Improvement: blit everything on the normal sized wall and then finally scale it up -.-
void	wall_render(t_editor *doom)
{
	t_grid *grid;
	t_wall *wall;
	t_xywh dim;
	SDL_Rect	texture;
	SDL_Rect	temp;
	SDL_Surface	*scaled_wall;
	float	scale;
	float	x_axis;
	float	y_axis;

	grid = &doom->grid;
	wall = grid->modify_wall;
	// get w from the difference from the orig -> dest x
	// TODO: figure out what 1 value on the grid is, so that we know how much we have to multiply the dimensions with,
	// 	pretty sure these are niklas things.
	
	// TODO: the "NOTE:" under here, i think that '* x' is the scale of the map...???
	dim.w = gfx_distance(wall->orig->pos, wall->dest->pos); //* 10; // NOTE: this '* x', means what one value on the grid is in the game
	// get h from the sector it is a part of
	dim.h = 96; // get the sector that the wall is a part of... and then take the height from it
	// all of these values needs to  have a zoom value depending on which is...
	//	more the x or the y so that we can see the whole wall...
	//	but if you after that want to zoom you can??
	dim.x = 50; // offsets
	dim.y = 50;
	// get texture from the wall selected
	// (wall->texture_id)
	// render wall with the texture

	// TODO: this needs to be taken from somewhere else, maybe take in a t_texture that has all these informations
	// 	(get texture with id function that takes from t_texture list and returns that surface)
	// 	get_texture_from_list_with_id();
	// 	get_texture_with_id_from_list();
	// 	already in it
	SDL_Surface *tex = load_image(ROOT_PATH"ui/ui_images/wallimage.png");
	texture.x = 0;
	texture.y = 0;
	texture.w = tex->w; 
	texture.h = tex->h;

	// how many of the textures can fit on each axis
	x_axis = (float)dim.w / ((float)texture.w * doom->grid.modify_wall->texture_scale);
	y_axis = (float)dim.h / ((float)texture.h * doom->grid.modify_wall->texture_scale);

//ft_printf("x_axis amount: %.3f, y amount: %.3f\n", x_axis, y_axis);

	scale = (doom->edit_view_wall->position.w - (dim.x * 2)) / dim.w;
	scaled_wall = create_surface(dim.w * scale, dim.h * scale);
	for (int y = 0; y < ceil(y_axis); y++)
	{
		for (int x = 0; x < ceil(x_axis); x++)
		{
			temp.w = texture.h * scale * doom->grid.modify_wall->texture_scale;
			temp.h = texture.w * scale * doom->grid.modify_wall->texture_scale;
			temp.x = x * temp.w;
			temp.y = y * temp.h;
			SDL_BlitScaled(tex, &texture, scaled_wall, &temp);
		}
	}
	SDL_FreeSurface(tex);

// render the objects placed on the wall
	t_list *curr;
	t_sprite *sprite;

	curr = wall->sprites;
	while (curr)
	{
		sprite = curr->content;

		// should take this from somewhere else
		SDL_Surface *temp_sprite = load_image(ROOT_PATH"ui/ui_images/sprite.jpg");

		temp.x = sprite->coord.x;
		temp.y = sprite->coord.y;
		temp.w = temp_sprite->w * sprite->scale;
		temp.h = temp_sprite->h * sprite->scale;

		sprite->coord.w = temp_sprite->w;
		sprite->coord.h = temp_sprite->h;
		SDL_BlitScaled(temp_sprite, NULL, scaled_wall, &temp);

	// TODO: this needs to be niklas blitscaled
		/* // This might be the way to do this, but before this work i will only use one sprite
		SDL_BlitScaled(doom->sprites[0].surface,
			&(SDL_Rect){
				doom->sprites[0].position[sprite->sprite_id][0],
				doom->sprites[0].position[sprite->sprite_id][1],
				doom->sprites[0].x_size,
				doom->sprites[0].y_size},
			scaled_wall, &temp);
			*/

		curr = curr->next;
	}
	if (doom->grid.modify_sprite != NULL)
	{
		draw_rect_border(scaled_wall, 
				doom->grid.modify_sprite->coord.x,
				doom->grid.modify_sprite->coord.y,
				doom->grid.modify_sprite->coord.w * doom->grid.modify_sprite->scale,
				doom->grid.modify_sprite->coord.h * doom->grid.modify_sprite->scale,
				0xff0000ff, 3);
	}
// finally blit the wall to the surface of the window
	SDL_BlitSurface(scaled_wall, NULL, doom->edit_view_wall->active_surface, &(SDL_Rect){dim.x, dim.y, dim.w * scale, dim.h * scale});
	gfx_draw_rect(doom->edit_view_wall->active_surface, 0xff00ff00, (t_xywh){dim.x, dim.y, dim.w * scale, dim.h * scale});
	SDL_FreeSurface(scaled_wall);
}

void	wall_option(t_editor *editor, t_bui_libui *libui)
{
	editor->edit_view_wall->show = 1;
	editor->edit_toolbox_wall->show = 1;

	preset_tab_events(editor->wall_tab);

	// check all the wall texture view elements
	
		// texture scale
	// Note: add and subtract from "t_wall" "texture_scale"
	if (bui_button(editor->wall_scale_add))
		editor->grid.modify_wall->texture_scale += 0.1f;
	else if (bui_button(editor->wall_scale_sub))
		editor->grid.modify_wall->texture_scale -= 0.1f;

	// TODO:NOTE:IDKK: not sure where i should do this.
	if (editor->grid.modify_wall->texture_scale < 0.1)
		editor->grid.modify_wall->texture_scale = 0.1f; 
	
	char *scale_value_str = ft_ftoa(editor->grid.modify_wall->texture_scale, 1);
	bui_change_element_text(editor->wall_scale_value, scale_value_str);
	ft_strdel(&scale_value_str);

		// wall solidity tick box
	editor->wall_solid_tick->toggle = editor->grid.modify_wall->solid;
	if (bui_button(editor->wall_solid_tick))
	{
		if (editor->wall_solid_tick->toggle == 1)
			editor->grid.modify_wall->solid = 0;
		else
			editor->grid.modify_wall->solid = 1;
	}

   //	wall portal tick box
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
	char *temp;

	temp = ft_itoa(editor->grid.modify_wall->texture_id);
	if (only_one_button_toggled_at_a_time(editor->wall_texture_buttons, &editor->active_wall_texture))
	{}
	else
		toggle_on_element_with_text(editor->wall_texture_buttons, &editor->active_wall_texture, temp);
	ft_strdel(&temp);
	if (editor->active_wall_texture != NULL)
		editor->grid.modify_wall->texture_id = ft_atoi(editor->active_wall_texture->text);
		// portal textures
	temp = ft_itoa(editor->grid.modify_wall->portal_texture_id);
	if (only_one_button_toggled_at_a_time(editor->portal_texture_buttons, &editor->active_portal_texture))
	{}
	else
		toggle_on_element_with_text(editor->portal_texture_buttons, &editor->active_portal_texture, temp);
	ft_strdel(&temp);
	if (editor->active_portal_texture != NULL)
		editor->grid.modify_wall->portal_texture_id = ft_atoi(editor->active_portal_texture->text);
		// wall sprites
	// TODO: figure out where this should get its texture id
	temp = ft_itoa(2);
	if (only_one_button_toggled_at_a_time(editor->wall_sprite_buttons, &editor->active_wall_sprite))
	{}
	else
		toggle_on_element_with_text(editor->wall_sprite_buttons, &editor->active_wall_sprite, temp);
	ft_strdel(&temp);
	if (editor->active_wall_sprite != NULL)
	{
		// editor->grid.modify_wall->portal_texture_id = ft_atoi(editor->active_portal_texture->text);
	}

	// the sprite add button
	short int chosen_texture = 0;
	if (editor->active_wall_sprite != NULL)
		chosen_texture = ft_atoi(editor->active_wall_sprite->text);
	if (bui_button(editor->add_wall_sprite_button))
	{
		// add the sprite to the wall
		ft_putstr("Adding sprite to wall\n");
		// 1. get the texture you have chosen from list below;
		// 	has been done already, aka chosen_texture??
		// 2. make new sprite,
		 	t_sprite *sprite = new_sprite();
		 	sprite->sprite_id = chosen_texture;
		// 3. add to wall->sprites
		 	add_to_list(&editor->grid.modify_wall->sprites, sprite, sizeof(t_sprite));
	}
	
	// NOTE: in this function you also render the wall sprites.
	wall_render(editor);

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
		int move_speed = 5;
		if (key_pressed(libui, KEY_LEFT))
			editor->grid.modify_sprite->coord.x -= move_speed;
		else if (key_pressed(libui, KEY_RIGHT))
			editor->grid.modify_sprite->coord.x += move_speed;
		if (key_pressed(libui, KEY_UP))
			editor->grid.modify_sprite->coord.y -= move_speed;
		else if (key_pressed(libui, KEY_DOWN))
			editor->grid.modify_sprite->coord.y += move_speed;

		// the sprite scale buttons
		// NOTE: might aswell do this here where we are already checking if there is a modify sprite.
		if (bui_button(editor->sprite_scale_add))
			editor->grid.modify_sprite->scale += 0.1f;
		else if (bui_button(editor->sprite_scale_sub))
			editor->grid.modify_sprite->scale -= 0.1f;

		// TODO:NOTE:IDKK: not sure where i should do this.
		if (editor->grid.modify_sprite->scale < 0.1)
			editor->grid.modify_sprite->scale = 0.1f; 
		
		char *sprite_scale_value_str = ft_ftoa(editor->grid.modify_sprite->scale, 1);
		bui_change_element_text(editor->sprite_scale_value, sprite_scale_value_str);
		ft_strdel(&sprite_scale_value_str);
		
		// remove sprite button
		if (bui_button(editor->remove_wall_sprite_button))
		{
			ft_putstr("Removing sprite from wall.\n");
			// TODO: make function for removing and freeing wall sprite.
			remove_from_sprites(&editor->grid.modify_wall->sprites, editor->grid.modify_sprite);
			editor->grid.modify_sprite = NULL;
		}
	}
}

void	sector_option(t_editor *editor, t_grid *grid)
{
	editor->edit_view_sector->show = 1;
	editor->edit_toolbox_sector->show = 1;

	// Floor and ceiling texture element events
	char *temp;
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
	// sector editing buttons.
	changer_prefab_events(editor->floor_height, &grid->modify_sector->floor_height, 1);
	changer_prefab_events(editor->ceiling_height, &grid->modify_sector->ceiling_height, 1);
	changer_prefab_events(editor->gravity, &grid->modify_sector->gravity, 1);
	changer_prefab_events(editor->lighting, &grid->modify_sector->light_level, 1);

	// floor and ceiling
	changer_prefab_events_float(editor->floor_scale, &grid->modify_sector->floor_texture_scale, 0.1f);
	changer_prefab_events_float(editor->ceiling_scale, &grid->modify_sector->ceiling_texture_scale, 0.1f);
}

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
		t_entity_preset *preset = get_entity_preset_from_list_with_name(editor->entity_presets, editor->entity_type_drop->active->text);
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
	doom->edit_view_sector->show = 0;
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
