/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2020/09/27 16:30:53 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

// @Improvement: blit everything on the normal sized wall and then finally scale it up -.-
void	show_wall_render(t_editor *doom, t_grid *grid)
{
	t_wall *wall;
	t_xywh dim;
	SDL_Rect	texture;
	SDL_Rect	temp;
	SDL_Surface	*scaled_wall;
	double	scale;
	double	x_axis;
	double	y_axis;

	wall = grid->modify_wall;
	// get w from the difference from the orig -> dest x
	dim.w = gfx_distance(wall->orig->pos, wall->dest->pos);
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

	texture.x = doom->textures[0].position[wall->texture_id][0];
	texture.y = doom->textures[0].position[wall->texture_id][1];
	texture.w = doom->textures[0].x_size;
	texture.h = doom->textures[0].y_size;

	// how many of the textures can fit on each axis
	x_axis = (float)dim.w / (float)doom->textures[0].x_size;
	y_axis = (float)dim.h / (float)doom->textures[0].y_size;

	scale = (doom->option.show_render->position.w - (dim.x * 2)) / dim.w;
	scaled_wall = create_surface(dim.w * scale, dim.h * scale);
	for (int y = 0; y < y_axis; y++)
	{
		for (int x = 0; x < x_axis; x++)
		{
			temp.w = doom->textures[0].x_size * scale;
			temp.h = doom->textures[0].y_size * scale;
			temp.x = x * temp.w;
			temp.y = y * temp.h;
			// TODO: this needs to be niklas made blitscaled
			SDL_BlitScaled(doom->textures[0].surface, &texture, scaled_wall, &temp);
		}
	}
// render the objects placed on the wall
	t_list *curr;
	t_sprite *sprite;

	curr = wall->sprites;
	while (curr)
	{
		sprite = curr->content;
		temp.x = sprite->pos.x;
		temp.y = sprite->pos.y;
		temp.w = sprite->w;
		temp.h = sprite->h;
		// TODO: this needs to be niklas blitscaled
		SDL_BlitScaled(doom->sprites[0].surface,
					&(SDL_Rect){doom->sprites[0].position[sprite->sprite_id][0],
								doom->sprites[0].position[sprite->sprite_id][1],
								doom->sprites[0].x_size,
								doom->sprites[0].y_size},
					scaled_wall, &temp);
		curr = curr->next;
	}
	if (doom->option.modify_sprite != NULL)
	{
		gfx_draw_rect(doom->option.show_render->active_surface,
					0xffff0000,
					(t_xywh){doom->option.modify_sprite->pos.x,
							doom->option.modify_sprite->pos.y,
							doom->option.modify_sprite->w,
							doom->option.modify_sprite->h});
	}
// finally blit the wall to the surface of the window
	SDL_BlitSurface(scaled_wall, NULL, doom->option.show_render->active_surface, &(SDL_Rect){dim.x, dim.y, dim.w * scale, dim.h * scale});
	gfx_draw_rect(doom->option.show_render->active_surface, 0xff00ff00, (t_xywh){dim.x, dim.y, dim.w * scale, dim.h * scale});
	SDL_FreeSurface(scaled_wall);
}

void	new_wall_render(t_editor *doom)
{
	t_grid *grid;
	t_wall *wall;
	t_xywh dim;
	SDL_Rect	texture;
	SDL_Rect	temp;
	SDL_Surface	*scaled_wall;
	double	scale;
	double	x_axis;
	double	y_axis;

	grid = &doom->grid;
	wall = grid->modify_wall;
	// get w from the difference from the orig -> dest x
	// TODO: figure out what 1 value on the grid is, so that we know how much we have to multiply the dimensions with,
	// 	pretty sure these are niklas things.
	dim.w = gfx_distance(wall->orig->pos, wall->dest->pos) * 10; // NOTE: this '* x', means what one value on the grid is in the game
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
	// 	already in it
	SDL_Surface *tex = load_image("../engine/ui/ui_images/wallimage.png");
	texture.x = 0;
	texture.y = 0;
	texture.w = tex->w; 
	texture.h = tex->h;

	// how many of the textures can fit on each axis
	x_axis = (float)dim.w / (float)texture.w;
	y_axis = (float)dim.h / (float)texture.h;

	scale = (doom->edit_view_wall->position.w - (dim.x * 2)) / dim.w;
	scaled_wall = create_surface(dim.w * scale, dim.h * scale);
	// TODO: ceil() function call needs to happen once.
	for (int y = 0; y < ceil(y_axis); y++)
	{
		for (int x = 0; x < ceil(x_axis); x++)
		{
			// TODO: figure out, if you want to be able to make the texture smaller, probably
			// 	discuss with niknokkare
			temp.w = texture.h * scale * doom->grid.modify_wall->texture_scale;
			temp.h = texture.w * scale * doom->grid.modify_wall->texture_scale;
			temp.x = x * temp.w;
			temp.y = y * temp.h;
			// TODO: this needs to be niklas made blitscaled
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

		SDL_Surface *temp_sprite = load_image("../engine/ui/ui_images/sprite.jpg");

		temp.x = sprite->coord.x;
		temp.y = sprite->coord.y;
		// TODO: this 2 needs to be multiplied by the scale when working.
		temp.w = temp_sprite->w; // * sprite->scale;
		temp.h = temp_sprite->h; // * sprite->scale;

				// Remove this when you have something else... duh?!
		sprite->coord.w = temp_sprite->w;
		sprite->coord.h = temp_sprite->h;
		SDL_BlitScaled(temp_sprite,
			&(SDL_Rect) {
				0, 0, temp_sprite->w, temp_sprite->h
			}, scaled_wall, &temp);

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
	if (doom->option.modify_sprite != NULL)
	{
		draw_rect_border(scaled_wall, 
				doom->option.modify_sprite->coord.x,
				doom->option.modify_sprite->coord.y,
				doom->option.modify_sprite->coord.w,
				doom->option.modify_sprite->coord.h,
				0xff0000ff, 3);
	}
// finally blit the wall to the surface of the window
	SDL_BlitSurface(scaled_wall, NULL, doom->edit_view_wall->active_surface, &(SDL_Rect){dim.x, dim.y, dim.w * scale, dim.h * scale});
	gfx_draw_rect(doom->edit_view_wall->active_surface, 0xff00ff00, (t_xywh){dim.x, dim.y, dim.w * scale, dim.h * scale});
	SDL_FreeSurface(scaled_wall);
}

void	wall_option(t_editor *doom, t_grid *grid, t_bui_libui *libui)
{
	doom->edit_view_wall->show = 1;
	doom->edit_toolbox_wall->show = 1;

	preset_tab_events(doom->wall_tab);

	// check all the wall texture view elements
	
		// scale
	// Note: add and subtract from "t_wall" "texture_scale"
	if (bui_button(libui, doom->wall_scale_add))
		doom->grid.modify_wall->texture_scale += 1;
	else if (bui_button(libui, doom->wall_scale_sub))
		doom->grid.modify_wall->texture_scale -= 1;

	// TODO:NOTE:IDKK: not sure where i should do this.
	if (doom->grid.modify_wall->texture_scale < 1)
		doom->grid.modify_wall->texture_scale = 1; 
	
	char *scale_value_str = ft_itoa(doom->grid.modify_wall->texture_scale);
	bui_change_element_text(doom->wall_scale_value, scale_value_str);
	ft_strdel(&scale_value_str);
	
		// texture buttons
	// Note: first step is to take from the "modify_wall" the "texture_id" and toggle that texture button
	t_list *curr;
	int current_id = 0; // this is the texture id for the current button in the loop
	curr = doom->wall_texture_buttons;
	while (curr)
	{
		bui_button_toggle(libui, curr->content);
		if (!((t_bui_element *)curr->content)->was_clicked_last_frame &&
		doom->grid.modify_wall->texture_id != current_id)
			((t_bui_element *)curr->content)->toggle = 0;
		else
		{
			doom->grid.modify_wall->texture_id = current_id;
			// TODO: when coming in to this function toggle on the already selected texture.
			((t_bui_element *)curr->content)->toggle = 1;
		}
		current_id++;
		curr = curr->next;	
	}

	// portal textures
	current_id = 0; // this is the texture id for the current button in the loop
	curr = doom->portal_texture_buttons;
	while (curr)
	{
		bui_button_toggle(libui, curr->content);
		if (!((t_bui_element *)curr->content)->was_clicked_last_frame &&
		doom->grid.modify_wall->portal_texture_id != current_id)
			((t_bui_element *)curr->content)->toggle = 0;
		else
		{
			doom->grid.modify_wall->portal_texture_id = current_id;
			// TODO: when coming in to this function toggle on the already selected texture.
			((t_bui_element *)curr->content)->toggle = 1;
		}
		current_id++;
		curr = curr->next;	
	}

		// wall sprites
	// TODO: update this everytime you click on the wall render thing.
	// TODO: maybe save this somewhere else, t.ex in the t_wall you have chosen or in the editor main struct;
	t_sprite *current_wall_sprite = NULL;
	short int chosen_texture = 0;

	current_id = 0; // this is the texture id for the current button in the loop
	curr = doom->wall_sprite_buttons;
	while (curr)
	{
		bui_button_toggle(libui, curr->content);
		if (!((t_bui_element *)curr->content)->was_clicked_last_frame &&
		current_wall_sprite != NULL &&
		current_wall_sprite->sprite_id != current_id)
			((t_bui_element *)curr->content)->toggle = 0;
		else
		{
			if (current_wall_sprite != NULL)
				current_wall_sprite->sprite_id = current_id;
			// TODO: when coming in to this function toggle on the already selected texture.
			((t_bui_element *)curr->content)->toggle = 1;
			chosen_texture = current_id;
		}
		current_id++;
		curr = curr->next;	
	}
	// the sprite add button
	if (bui_button(libui, doom->add_wall_sprite_button))
	{
		// add the sprite to the wall
		ft_putstr("Adding sprite to wall\n");
		// 1. get the texture you have chosen from list below;
		// 	has been done already, aka chosen_texture??
		// 2. make new sprite,
		 	t_sprite *sprite = new_sprite();
		 	sprite->sprite_id = chosen_texture;
		// 3. add to wall->sprites
		 	add_to_list(&doom->grid.modify_wall->sprites, sprite, sizeof(t_sprite));
	}
	// TODO: fix this, its slower than jesus on the cross
	// NOTE: in this function you also render the wall sprites.
	new_wall_render(doom);

	// Choose the sprite
	if (mouse_hover(doom->libui, doom->edit_view_wall->position) &&
	mouse_pressed(doom->libui, MKEY_LEFT))
	{
		int view_x = doom->libui->mouse_x - doom->edit_view_wall->position.x - 50;
		int view_y = doom->libui->mouse_y - doom->edit_view_wall->position.y - 50;
		t_sprite *temp;

		// Note: this function returns the sprite at that exact location, so you have to give
		// 	the exact location of where you want it to look if there is a sprite. thus the way you have
		// 	removed from the mouse_x and y the view location.
		temp = get_sprite_from_list(&doom->grid.modify_wall->sprites, view_x, view_y);
		if (temp != NULL)
		{
			// TODO: remove this from doom->option. (because we want to remove that struct helt och and the whole)
			ft_putstr("Sprite was successfully selected.\n");
			doom->option.modify_sprite = temp;
		}
	}
	// Move the sprite
	if (doom->option.modify_sprite != NULL)
	{
		int move_speed = 5;
		if (key_pressed(libui, KEY_LEFT))
			doom->option.modify_sprite->coord.x -= move_speed;
		else if (key_pressed(libui, KEY_RIGHT))
			doom->option.modify_sprite->coord.x += move_speed;
		if (key_pressed(libui, KEY_UP))
			doom->option.modify_sprite->coord.y -= move_speed;
		else if (key_pressed(libui, KEY_DOWN))
			doom->option.modify_sprite->coord.y += move_speed;
	}
}

void	sector_edit_button_events(t_bui_libui *libui, t_sector_edit *collection, short int *current_value)
{
	char *str = NULL;
	
	if (bui_button(libui, collection->add_button))
		*current_value += 1;
	if (bui_button(libui, collection->sub_button))
		*current_value -= 1;
	str = ft_sprintf("%d", *current_value);
	bui_change_element_text(collection->amount, str);
	ft_strdel(&str);
}

void	sector_option(t_editor *doom, t_grid *grid, t_bui_libui *libui)
{
	t_list *curr;
	t_sector_edit *temp;

	bui_change_element_text(doom->option.title, "Sector edit");

	doom->edit_view_sector->show = 1;
	doom->edit_toolbox_sector->show = 1;

	curr = doom->option.sector_edit_buttons;
	if (grid->modify_sector == NULL)
		return ;
	while (curr)
	{
		temp = curr->content;
		if (ft_strstr(temp->text->text, "floor height"))
			temp->f_amount = &grid->modify_sector->floor_height;
		else if (ft_strstr(temp->text->text, "ceiling height"))
			temp->f_amount = &grid->modify_sector->ceiling_height;
		else if (ft_strstr(temp->text->text, "gravity"))
			temp->f_amount = &grid->modify_sector->gravity;
		else if (ft_strstr(temp->text->text, "lighting"))
			temp->f_amount = &grid->modify_sector->light_level;
		temp->text->show = 1;
		temp->sub_button->show = 1;
		temp->amount->show = 1;
		temp->add_button->show = 1;
		char *str;
		if (temp->f_amount == NULL)
			str = ft_strdup("not set");
		else
		{
			str = ft_itoa(*temp->f_amount);
			if (bui_button(doom->libui, temp->sub_button))
				*temp->f_amount -= 1;
			if (bui_button(doom->libui, temp->add_button))
				*temp->f_amount += 1;
		}
		bui_change_element_text(temp->amount, str);
		ft_strdel(&str);
		curr = curr->next;
	}

	// Floor and ceiling texture element events
	curr = doom->floor_texture_buttons;
	while (curr)
	{
		bui_button_toggle(libui, curr->content);
		if (!((t_bui_element *)curr->content)->was_clicked_last_frame &&
		doom->active_floor_texture != NULL &&
		doom->active_floor_texture != ((t_bui_element *)curr->content))
			((t_bui_element *)curr->content)->toggle = 0;
		else
			doom->active_floor_texture = curr->content;
		curr = curr->next;	
	}

	curr = doom->ceiling_texture_buttons;
	while (curr)
	{
		bui_button_toggle(libui, curr->content);
		if (!((t_bui_element *)curr->content)->was_clicked_last_frame &&
		doom->active_ceiling_texture != NULL &&
		doom->active_ceiling_texture != ((t_bui_element *)curr->content))
			((t_bui_element *)curr->content)->toggle = 0;
		else
			doom->active_ceiling_texture = curr->content;
		curr = curr->next;	
	}
	// new sector editing buttons.
	sector_edit_button_events(libui, doom->floor_height, &grid->modify_sector->floor_height);
	sector_edit_button_events(libui, doom->ceiling_height, &grid->modify_sector->ceiling_height);
	sector_edit_button_events(libui, doom->gravity, &grid->modify_sector->gravity);
	sector_edit_button_events(libui, doom->lighting, &grid->modify_sector->light_level);
}

void	entity_option(t_editor *doom, t_grid *grid, t_bui_libui *libui)
{
	t_wall_edit *option;

	bui_change_element_text(doom->option.title, "Entity edit");
	option = &doom->option;
	option->ent_sprite_button->show = 1;
	option->ent_info_button->show = 1;
	option->ent_render_sprite->show = 1;
	if (bui_button_toggle(doom->libui, option->ent_info_button))
	{
		// Untoggle the other buttons/tabs
		option->ent_sprite_button->toggle = 0;
		option->ent_sprites->show = 0;

		option->ent_info_menu->show = 1;
		// TODO: strdel here if they havent already
		// TODO: better libui elements dont have .text
		/*
		grid->modify_entity->max_health = ft_atoi(option->ent_info_health_text_area->text.text);
		grid->modify_entity->speed = ft_atoi(option->ent_info_speed_text_area->text.text);
		grid->modify_entity->armor = ft_atoi(option->ent_info_armor_text_area->text.text);
		*/

		t_list *item;
		int i = 0;
		// TODO: drop down is not supported yet in better libui
		//item = ((t_drop_down *)option->type_dropdown->info)->items;
		item = NULL; // dont forget to remove this
		while (item)
		{
			if (bui_button(doom->libui, item->content))
				grid->modify_entity->type = i;
			item = item->next;
			i++;
		}
//ft_printf("Entity type: %d\n", grid->modify_entity->type);
	}
	if (bui_button_toggle(doom->libui, option->ent_sprite_button))
	{
		// Untoggle the other tabs
		option->ent_info_button->toggle = 0;
		option->ent_info_menu->show = 0;

		option->ent_sprites->show = 1;
		entity_sprite_buttons(doom, grid);
	}
	// render the sprite on the menu view
	if (grid->modify_entity->sprite_id < 0 || grid->modify_entity->sprite_id >= doom->entity_sprites[0].max_textures)
		return ;
	SDL_BlitSurface(doom->entity_sprites[0].surface,
					&(SDL_Rect){doom->entity_sprites[0].position[grid->modify_entity->sprite_id][0],
								doom->entity_sprites[0].position[grid->modify_entity->sprite_id][1],
								doom->entity_sprites[0].x_size,
								doom->entity_sprites[0].y_size},
					doom->option.ent_render_sprite->active_surface,
					&(SDL_Rect){0, 0,
								doom->entity_sprites[0].x_size,
								doom->entity_sprites[0].y_size});
}

void	selected_option_menu(t_editor *doom, t_grid *grid, t_bui_libui *libui)
{
	doom->option.texture_button->show = 0;
	doom->option.textures->show = 0;
	doom->option.add_button->show = 0;
	doom->option.sprites->show = 0;
	doom->option.show_render->show = 0;
	
	// NEW STUFF
	doom->edit_view_sector->show = 0;
	doom->edit_toolbox_sector->show = 0;

	doom->edit_view_wall->show = 0;
	doom->edit_toolbox_wall->show = 0;
	// END NEW STUFF

// sector stuff
	t_list *curr;
	t_sector_edit *temp;

	curr = doom->option.sector_edit_buttons;
	while (curr)
	{
		temp = curr->content;
		temp->text->show = 0;
		temp->sub_button->show = 0;
		temp->amount->show = 0;
		temp->add_button->show = 0;
		curr = curr->next;
	}
// entity stuff
	doom->option.ent_sprite_button->show = 0;
	doom->option.ent_sprites->show = 0;
	doom->option.ent_info_button->show = 0;
	doom->option.ent_info_menu->show = 0;
	doom->option.ent_render_sprite->show = 0;
	if (grid->modify_wall != NULL)
		wall_option(doom, grid, libui);
	else if (grid->modify_sector != NULL)
		sector_option(doom, grid, libui);
	else if (grid->modify_entity != NULL)
		entity_option(doom, grid, libui);
	else
	{
		bui_change_element_text(doom->option.title, " "); // reset the text if nothing is selected
		return ;
	}
	doom->option.menu->show = 1;
}
