/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 12:19:16 by jsalmi            #+#    #+#             */
/*   Updated: 2020/09/27 15:42:22 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	window_init(t_editor *doom, t_libui *libui)
{
	t_window_info info;

	info.coord = ui_init_coords(0, 0, 1920, 1080);
	info.title = ft_strjoiner("Doom Nukem Map Editor : ", doom->filename, NULL);
	info.bg_color = doom->palette.win;
	doom->window = ft_create_window(libui, info);

	info.coord = ui_init_coords(500, 500, 1500, 750);
	info.title = ft_strdup("Editor");
	info.bg_color = doom->palette.win;
	doom->edit_window = ft_create_window(libui, info);
	SDL_MinimizeWindow(doom->edit_window->win);
}

void	grid_init(t_editor *doom)
{
	t_xywh coord;

	coord = ui_init_coords(doom->window->surface->w / 4 + 20, 10,
			(doom->window->surface->w / 4) * 3 - 30, doom->window->surface->h - 20);
	doom->grid.elem = ui_create_surface(doom->window, coord, NULL);
	doom->grid.elem->shadow = 0;
	ft_update_elem_background(doom->grid.elem, doom->palette.win_elem);

	doom->grid.hover = EMPTY_VEC;
	doom->spawn.pos = EMPTY_VEC;
	doom->grid.selected1 = EMPTY_VEC;
	doom->grid.selected2 = EMPTY_VEC;
	doom->grid.modify_wall = NULL;
	doom->grid.modify_point = NULL;
	doom->grid.modify_sector = NULL;
	doom->grid.modify_entity = NULL;
	doom->grid.points = NULL;
	doom->grid.walls = NULL;
	doom->grid.sectors = NULL;
	doom->grid.entities = NULL;
	doom->grid.gap = 10;
	doom->grid.point_amount = 0;
	doom->grid.wall_amount = 0;
	doom->grid.sector_amount = 0;
	doom->grid.entity_amount = 0;
	doom->grid.x = doom->grid.elem->coord.x;
	doom->grid.y = doom->grid.elem->coord.y;
}

void	toolbox_init(t_editor *doom)
{
	t_xywh coord;

	coord = ui_init_coords(10, 10, doom->window->surface->w / 4, doom->window->surface->h - 20);
	doom->toolbox = ui_create_surface(doom->window, coord, NULL);
	doom->toolbox->shadow = 0;
	ft_update_elem_background(doom->toolbox, doom->palette.win_elem);

	coord = ui_init_coords(10, (doom->toolbox->surface->h / 4 + 10), doom->toolbox->surface->w - 20, (doom->toolbox->surface->h / 4) * 3 - 20);
	doom->info_area = ui_create_surface(doom->window, coord, doom->toolbox);
	doom->info_area->shadow = 0;
	ft_update_elem_background(doom->info_area, doom->palette.elem_elem);
}

void	button_init(t_editor *doom)
{
	t_xywh coord;

	coord = ui_init_coords(25, 25, 100, 50);
	doom->button_draw = ui_create_button(doom->window, coord, doom->toolbox);
	doom->button_draw->toggleable = 1;
	ft_set_text(&doom->button_draw->text, "draw");
	ft_update_elem_background(doom->button_draw, doom->palette.elem_elem);
	doom->button_draw->default_state = 1;

	coord = ui_init_coords(150, 25, 100, 50);
	doom->button_select = ui_create_button(doom->window, coord, doom->toolbox);
	doom->button_select->toggleable = 1;
	ft_set_text(&doom->button_select->text, "select");
	ft_update_elem_background(doom->button_select, doom->palette.elem_elem);

	coord = ui_init_coords(275, 25, 100, 50);
	doom->button_save = ui_create_button(doom->window, coord, doom->toolbox);
	doom->button_save->f = &single_click;
	ft_set_text(&doom->button_save->text, "save");
	ft_update_elem_background(doom->button_save, doom->palette.elem_elem);
// deletion button
	coord = ui_init_coords(doom->info_area->surface->w - 110, 25, 100, 50);
	doom->button_remove = ui_create_button(doom->window, coord, doom->info_area);
	doom->button_remove->f = &single_click;
	doom->button_remove->shadow = 0;
	ft_set_text(&doom->button_remove->text, "remove");
	ft_update_elem_background(doom->button_remove, doom->palette.elem_elem_elem);
// edit button
	coord = ui_init_coords(doom->info_area->surface->w - 110, 100, 100, 50);
	doom->button_edit = ui_create_button(doom->window, coord, doom->info_area);
	doom->button_edit->f = &single_click;
	doom->button_edit->shadow = 0;
	ft_set_text(&doom->button_edit->text, "edit");
	ft_update_elem_background(doom->button_edit, doom->palette.elem_elem_elem);
// add button
	coord = ui_init_coords(doom->info_area->surface->w - 110, 175, 100, 50);
	doom->button_add = ui_create_button(doom->window, coord, doom->info_area);
	doom->button_add->f = &single_click;
	doom->button_add->shadow = 0;
	ft_set_text(&doom->button_add->text, "add portal");
	ft_update_elem_background(doom->button_add, doom->palette.elem_elem_elem);
// remove button
	coord = ui_init_coords(doom->info_area->surface->w - 110, 250, 100, 50);
	doom->button_remove_portal = ui_create_button(doom->window, coord, doom->info_area);
	doom->button_remove_portal->f = &single_click;
	doom->button_remove_portal->shadow = 0;
	ft_set_text(&doom->button_remove_portal->text, "remove portal");
	ft_update_elem_background(doom->button_remove_portal, doom->palette.elem_elem_elem);
}

void	color_palette_init(t_color_palette *pal)
{
	pal->win = 0xff0e1111;
	pal->win_elem = 0xff232b2b;
	pal->elem_elem = 0xff353839;
	pal->elem_elem_elem = 0xffa6a6a4;
}

void	texture_button_init(t_editor *doom)
{
	SDL_Surface	 *temp;
	SDL_Rect	rect;
	t_xywh coord;
	int	gap;
	int i;
	int size;
	int x;
	int y;
	int amount;

	doom->texture_buttons = (t_element **)malloc(sizeof(t_element *) * doom->textures[0].max_textures);
	gap = 25;
	size = 55;
	i = 0;
	amount = doom->option.textures->surface->w / (gap + size);
	rect.w = doom->textures[0].x_size;
	rect.h = doom->textures[0].y_size;
	temp = SDL_CreateRGBSurface(0, doom->textures[0].x_size, doom->textures[0].y_size, 32, 0,0,0,0);
	while (i < doom->textures[0].max_textures)
	{
		rect.x = doom->textures[0].position[i][0];
		rect.y = doom->textures[0].position[i][1];
		SDL_BlitSurface(doom->textures[0].surface, &rect, temp, NULL);
		x = (i % amount) * (gap + size) + gap;
		y = (i / amount) * (gap + size) + gap;
		coord = ui_init_coords(x, y, size, size);
		doom->texture_buttons[i] = ui_create_button(doom->edit_window, coord, doom->option.textures);
		doom->texture_buttons[i]->f = &single_click;
		ft_set_element_image(doom->texture_buttons[i], temp, NULL);
		i++;
	}
}

void	sprite_button_init(t_editor *doom)
{
	SDL_Surface	 *temp;
	SDL_Rect	rect;
	t_xywh coord;
	int	gap;
	int i;
	int size;
	int x;
	int y;
	int amount;

	doom->sprite_buttons = (t_element **)malloc(sizeof(t_element *) * doom->sprites[0].max_textures);
	gap = 25;
	size = 55;
	i = 0;
	amount = doom->option.sprites->surface->w / (gap + size);
	rect.w = doom->sprites[0].x_size;
	rect.h = doom->sprites[0].y_size;
	temp = SDL_CreateRGBSurface(0, doom->sprites[0].x_size, doom->sprites[0].y_size, 32, 0,0,0,0);
	while (i < doom->sprites[0].max_textures)
	{
		rect.x = doom->sprites[0].position[i][0];
		rect.y = doom->sprites[0].position[i][1];
		SDL_BlitSurface(doom->sprites[0].surface, &rect, temp, NULL);
		x = (i % amount) * (gap + size) + gap;
		y = (i / amount) * (gap + size) + gap;
		coord = ui_init_coords(x, y, size, size);
		doom->sprite_buttons[i] = ui_create_button(doom->edit_window, coord, doom->option.sprites);
		doom->sprite_buttons[i]->f = &single_click;
		ft_set_element_image(doom->sprite_buttons[i], temp, NULL);
		i++;
	}
}

void	texture_init(t_editor *doom)
{
	load_texture(&doom->textures[0], "../textures/textures.bmp", 64, 64);
	split_texture(&doom->textures[0]);

	texture_button_init(doom);
}

void	texture_buttons(t_editor *doom, t_grid *grid)
{
	int i;

	if (grid->modify_wall == NULL) // nothing to modify
		return ;
	i = 0;
	while (i < doom->textures[0].max_textures)
	{
		if (doom->texture_buttons[i]->state == 1)
		{
			grid->modify_wall->texture_id = i;
			doom->texture_buttons[i]->shadow = 0;
			doom->texture_buttons[i]->offset_x = 5;
			doom->texture_buttons[i]->offset_y = 5;
		}
		if (grid->modify_wall->texture_id != i)
		{
			doom->texture_buttons[i]->shadow = 1;
			doom->texture_buttons[i]->offset_x = 0;
			doom->texture_buttons[i]->offset_y = 0;
		}
		i++;
	}
}

void	sprite_init(t_editor *doom)
{
	load_texture(&doom->sprites[0], "../textures/sprite.bmp", 128, 128);
	split_texture(&doom->sprites[0]);

	sprite_button_init(doom);
	doom->option.selected_sprite = -1;
}

void	sprite_buttons(t_editor *doom, t_grid *grid)
{
	int i;

	if (grid->modify_wall == NULL) // nothing to modify
		return ;
	i = 0;
	while (i < doom->sprites[0].max_textures)
	{
		if (doom->sprite_buttons[i]->state == 1)
		{
			doom->option.selected_sprite = i;
			doom->sprite_buttons[i]->shadow = 0;
			doom->sprite_buttons[i]->offset_x = 5;
			doom->sprite_buttons[i]->offset_y = 5;
		}
		if (doom->option.selected_sprite != i)
		{
			doom->sprite_buttons[i]->shadow = 1;
			doom->sprite_buttons[i]->offset_x = 0;
			doom->sprite_buttons[i]->offset_y = 0;
		}
		i++;
	}
}

void	option_menu_init(t_editor *doom)
{
	t_xywh coord;
	t_wall_edit	*option;

	doom->option.modify_sprite = NULL;
// menu
	option = &doom->option;
	coord = ui_init_coords(25, 25, ((doom->edit_window->surface->w / 3) * 2), doom->edit_window->surface->h - (doom->edit_window->surface->h / 4));
	option->menu = ui_create_surface(doom->edit_window, coord, NULL);
	option->menu->shadow = 0;
	ft_update_elem_background(option->menu, doom->palette.elem_elem);
// title bar
	coord = ui_init_coords(0, 0, option->menu->surface->w, 20);
	option->title = ui_create_surface(doom->edit_window, coord, option->menu);
	option->title->shadow = 0;
	ft_update_elem_background(option->title, doom->palette.elem_elem_elem);
	ft_set_text(&option->title->text, "title");
	ft_set_font(&option->title->text, "../libui/TTF/font.ttf", 20);
// view
	option->view = ui_create_surface(doom->edit_window, (t_xywh){25, 45, option->menu->surface->w - 50, option->menu->surface->h - (25 + 45)}, option->menu);
	option->view->shadow = 0;
	option->view->render = 0;
// info
	option->info = ui_create_surface(doom->edit_window, (t_xywh){25, doom->edit_window->surface->h - (doom->edit_window->surface->h / 4) + 50, option->menu->surface->w, doom->edit_window->surface->h - (doom->edit_window->surface->h - (doom->edit_window->surface->h / 4) + 75)}, NULL);
	ft_update_elem_background(option->info, doom->palette.elem_elem);
	option->info->shadow = 0;
// render
	option->show_render = ui_create_surface(doom->edit_window, (t_xywh){option->view->coord.x, option->view->coord.y,
											option->view->coord.w, option->view->coord.h}, option->menu);
	option->show_render->shadow = 0;
	option->show_render->render = 0;
// adding view
	option->add_view = ui_create_surface(doom->edit_window, (t_xywh){option->menu->coord.x + option->menu->coord.w + 25, 25, doom->edit_window->surface->w - (option->menu->coord.x + option->menu->coord.w) - 50, doom->edit_window->surface->h - 50}, NULL);
	ft_update_elem_background(option->add_view, doom->palette.elem_elem);
	option->add_view->shadow = 0;
// textures
	option->texture_button = ui_create_button(doom->edit_window, (t_xywh){25, 25, 100, 50}, option->add_view);
	option->texture_button->shadow = 0;
	option->texture_button->toggleable = 1;
	ft_update_elem_background(option->texture_button, doom->palette.elem_elem_elem);
	ft_set_text(&option->texture_button->text, "textures");

	option->textures = ui_create_surface(doom->edit_window, (t_xywh){25, 100,
											option->add_view->coord.w - 50, option->add_view->coord.h - 125}, option->add_view);
	option->textures->shadow = 0;
	option->textures->render = 0;
// sprite adding
	option->add_button = ui_create_button(doom->edit_window, (t_xywh){150, 25, 100, 50}, option->add_view);
	option->add_button->shadow = 0;
	option->add_button->toggleable = 1;
	ft_update_elem_background(option->add_button, doom->palette.elem_elem_elem);
	ft_set_text(&option->add_button->text, "add sprite");

	option->sprites = ui_create_surface(doom->edit_window, (t_xywh){25, 100,
											option->add_view->coord.w - 50, option->add_view->coord.h - 125}, option->add_view);
	option->sprites->shadow = 0;
	option->sprites->render = 0;
}

void	add_sector_button_prefab(t_editor *doom, char *str, t_xywh position)
{
	t_xywh coord;
	t_wall_edit *option;
	t_sector_edit *prefab;

	option = &doom->option;
	prefab = (t_sector_edit *)malloc(sizeof(t_sector_edit));
	add_to_list(&option->sector_edit_buttons, prefab, sizeof(t_sector_edit));
// text
	prefab->text = ui_create_surface(doom->edit_window, position, option->add_view);
	ft_update_elem_background(prefab->text, doom->palette.elem_elem);
	ft_set_text(&prefab->text->text, str);
	prefab->text->shadow = 0;
	prefab->text->render = 0;
// sub
	coord = ui_init_coords(175, 0, 50, 50);
	prefab->sub_button = ui_create_button(doom->edit_window, coord, prefab->text);
	prefab->sub_button->f = &single_click;
	ft_update_elem_background(prefab->sub_button, doom->palette.elem_elem_elem);
	ft_set_text(&prefab->sub_button->text, "-");
	prefab->sub_button->text.centered = 1;
	prefab->sub_button->shadow = 0;
	prefab->sub_button->render = 0;
// amount
	coord = ui_init_coords(250, 0, 50, 50);
	prefab->amount = ui_create_surface(doom->edit_window, coord, prefab->text);
	prefab->amount->f = &single_click;
	ft_update_elem_background(prefab->amount, doom->palette.elem_elem);
	ft_set_text(&prefab->amount->text, "not set");
	prefab->amount->text.centered = 1;
	prefab->amount->shadow = 0;
	prefab->amount->render = 0;

	prefab->f_amount = NULL;
// add
	coord = ui_init_coords(300, 0, 50, 50);
	prefab->add_button = ui_create_button(doom->edit_window, coord, prefab->text);
	prefab->add_button->f = &single_click;
	ft_update_elem_background(prefab->add_button, doom->palette.elem_elem_elem);
	ft_set_text(&prefab->add_button->text, "+");
	prefab->add_button->text.centered = 1;
	prefab->add_button->shadow = 0;
	prefab->add_button->render = 0;
}

void	sector_edit_buttons_init(t_editor *doom)
{
	t_xywh coord;
	t_wall_edit	*option;

	option = &doom->option;

	coord = ui_init_coords(25, 25, option->add_view->coord.w - 50, 50);
	add_sector_button_prefab(doom, "floor height", coord);

	coord = ui_init_coords(25, 100, option->add_view->coord.w - 50, 50);
	add_sector_button_prefab(doom, "ceiling height", coord);

	coord = ui_init_coords(25, 175, option->add_view->coord.w - 50, 50);
	add_sector_button_prefab(doom, "gravity", coord);

	coord = ui_init_coords(25, 250, option->add_view->coord.w - 50, 50);
	add_sector_button_prefab(doom, "lighting", coord);
}

void	entity_sprite_button_init(t_editor *doom)
{
	SDL_Surface	 *temp;
	SDL_Rect	rect;
	t_xywh coord;
	int	gap;
	int i;
	int size;
	int x;
	int y;
	int amount;

	doom->entity_sprite_buttons = (t_element **)malloc(sizeof(t_element *) * doom->entity_sprites[0].max_textures);
	gap = 25;
	size = 55;
	i = 0;
	amount = doom->option.ent_sprites->surface->w / (gap + size);
	rect.w = doom->entity_sprites[0].x_size;
	rect.h = doom->entity_sprites[0].y_size;
	temp = SDL_CreateRGBSurface(0, doom->entity_sprites[0].x_size, doom->entity_sprites[0].y_size, 32, 0,0,0,0);
	while (i < doom->entity_sprites[0].max_textures)
	{
		rect.x = doom->entity_sprites[0].position[i][0];
		rect.y = doom->entity_sprites[0].position[i][1];
		SDL_BlitSurface(doom->entity_sprites[0].surface, &rect, temp, NULL);
		x = (i % amount) * (gap + size) + gap;
		y = (i / amount) * (gap + size) + gap;
		coord = ui_init_coords(x, y, size, size);
		doom->entity_sprite_buttons[i] = ui_create_button(doom->edit_window, coord, doom->option.ent_sprites);
		doom->entity_sprite_buttons[i]->f = &single_click;
		ft_set_element_image(doom->entity_sprite_buttons[i], temp, NULL);
		i++;
	}
}

void	entity_sprite_init(t_editor *doom)
{
	load_texture(&doom->entity_sprites[0], "../textures/whiteofficer.bmp", 128, 128);
	split_texture(&doom->entity_sprites[0]);

	entity_sprite_button_init(doom);
}

void	entity_sprite_buttons(t_editor *doom, t_grid *grid)
{
	int i;

	if (grid->modify_entity == NULL) // nothing to modify
		return ;
	i = 0;
	while (i < doom->entity_sprites[0].max_textures)
	{
		if (doom->entity_sprite_buttons[i]->state == 1)
		{
			grid->modify_entity->sprite_id = i;
			doom->entity_sprite_buttons[i]->shadow = 0;
			doom->entity_sprite_buttons[i]->offset_x = 5;
			doom->entity_sprite_buttons[i]->offset_y = 5;
		}
		if (grid->modify_entity->sprite_id != i)
		{
			doom->entity_sprite_buttons[i]->shadow = 1;
			doom->entity_sprite_buttons[i]->offset_x = 0;
			doom->entity_sprite_buttons[i]->offset_y = 0;
		}
		i++;
	}
}

void	entity_edit_button_init(t_editor *doom)
{
	t_xywh coord;
	t_wall_edit *option;

	option = &doom->option;
/*
*	"RENDER VIEW"
*/
	option->ent_render_sprite = ui_create_surface(doom->edit_window, option->view->coord, option->menu);
	option->ent_render_sprite->shadow = 0;
	option->ent_render_sprite->render = 0;
/*
* "SPRITE" tab
*/
// the button
	coord = ui_init_coords(25, 25, 100, 50);
	option->ent_sprite_button = ui_create_button(doom->edit_window, coord, option->add_view);
	ft_update_elem_background(option->ent_sprite_button, doom->palette.elem_elem_elem);
	ft_set_text(&option->ent_sprite_button->text, "Sprite");
	option->ent_sprite_button->text.centered = 1;
	option->ent_sprite_button->toggleable = 1;
	option->ent_sprite_button->shadow = 0;
	option->ent_sprite_button->render = 0;
// the sprites
	coord = ui_init_coords(25, 100, option->add_view->coord.w - 50, option->add_view->coord.h - 125);
	option->ent_sprites = ui_create_surface(doom->edit_window, coord, option->add_view);
	ft_update_elem_background(option->ent_sprites, 0xffffffff);
	option->ent_sprites->shadow = 0;
	option->ent_sprites->render = 0;
/*
**	"OTHER" tab
*/
	coord = ui_init_coords(150, 25, 100, 50);
	option->ent_info_button = ui_create_button(doom->edit_window, coord, option->add_view);
	ft_update_elem_background(option->ent_info_button, doom->palette.elem_elem_elem);
	ft_set_text(&option->ent_info_button->text, "Other");
	option->ent_info_button->text.centered = 1;
	option->ent_info_button->toggleable = 1;
	option->ent_info_button->shadow = 0;
	option->ent_info_button->render = 0;
// the menu;
	coord = ui_init_coords(25, 125, option->add_view->surface->w - 50, option->add_view->surface->h - 150);
	option->ent_info_menu = ui_create_surface(doom->edit_window, coord, option->add_view);
	ft_update_elem_background(option->ent_info_menu, doom->palette.elem_elem);
	option->ent_info_menu->shadow = 0;
	option->ent_info_menu->render = 0;
// id text area
	coord = ui_init_coords(0, 0, 100, 50);
	option->ent_info_id_text = ui_create_surface(doom->edit_window, coord, option->ent_info_menu);
	ft_update_elem_background(option->ent_info_id_text, doom->palette.elem_elem);
	ft_set_text(&option->ent_info_id_text->text, "id: -1");
	option->ent_info_id_text->shadow = 0;
// health text area
	coord = ui_init_coords(0, 75, 100, 50);
	option->ent_info_health_text = ui_create_surface(doom->edit_window, coord, option->ent_info_menu);
	ft_update_elem_background(option->ent_info_health_text, doom->palette.elem_elem);
	ft_set_text(&option->ent_info_health_text->text, "health:");
	option->ent_info_health_text->shadow = 0;

	coord = ui_init_coords(100, 75, option->ent_info_menu->coord.w - 100, 50);
	option->ent_info_health_text_area = ui_create_textarea(doom->edit_window, coord, option->ent_info_menu);
	option->ent_info_health_text_area->shadow = 0;

// speed text area
	coord = ui_init_coords(0, 150, 100, 50);
	option->ent_info_speed_text = ui_create_surface(doom->edit_window, coord, option->ent_info_menu);
	ft_update_elem_background(option->ent_info_speed_text, doom->palette.elem_elem);
	ft_set_text(&option->ent_info_speed_text->text, "speed:");
	option->ent_info_speed_text->shadow = 0;

	coord = ui_init_coords(100, 150, option->ent_info_menu->coord.w - 100, 50);
	option->ent_info_speed_text_area = ui_create_textarea(doom->edit_window, coord, option->ent_info_menu);
	option->ent_info_speed_text_area->shadow = 0;

// armor text area
	coord = ui_init_coords(0, 225, 100, 50);
	option->ent_info_armor_text = ui_create_surface(doom->edit_window, coord, option->ent_info_menu);
	ft_update_elem_background(option->ent_info_armor_text, doom->palette.elem_elem);
	ft_set_text(&option->ent_info_armor_text->text, "armor:");
	option->ent_info_armor_text->shadow = 0;

	coord = ui_init_coords(100, 225, option->ent_info_menu->coord.w - 100, 50);
	option->ent_info_armor_text_area = ui_create_textarea(doom->edit_window, coord, option->ent_info_menu);
	option->ent_info_armor_text_area->shadow = 0;
// type drop down
	coord = ui_init_coords(0, 300, 200, 50);
	option->type_dropdown = ui_create_drop(doom->edit_window, coord, option->ent_info_menu);
	option->type_dropdown->shadow = 0;
	ft_set_text(&option->type_dropdown->text, "select type");
	ft_drop_down_add_item(option->type_dropdown, "neutral");
	ft_drop_down_add_item(option->type_dropdown, "friendly");
	ft_drop_down_add_item(option->type_dropdown, "enemy");
}
