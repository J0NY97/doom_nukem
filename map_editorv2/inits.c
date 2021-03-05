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

void	window_init(t_editor *doom, t_bui_libui *libui)
{
	t_xywh coord = ui_init_coords(0, 0, 1920, 1080);
	char *title = ft_strjoiner("Doom Nukem Map Editor : ", doom->filename, NULL);
	Uint32 bg_color = doom->palette.win;
	doom->window = bui_new_window(libui, title, coord, 0, bg_color);
	ft_strdel(&title);

	coord = ui_init_coords(500, 500, 1500, 750);
	title = ft_strdup("Editor");
	bg_color = doom->palette.win;
	doom->edit_window = bui_new_window(libui, title, coord, 0, bg_color);
	ft_strdel(&title);
	SDL_MinimizeWindow(doom->edit_window->win);
}

void	grid_init(t_editor *doom)
{
	t_xywh coord;

	coord = ui_init_coords(doom->window->active_surface->w / 4 + 20, 10,
			(doom->window->active_surface->w / 4) * 3 - 30, doom->window->active_surface->h - 20);
	doom->grid.elem = bui_new_menu(doom->window, "grid", coord);
	bui_set_element_color(doom->grid.elem, doom->palette.win_elem);
	doom->grid.elem->update = 0;

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
	doom->grid.x = doom->grid.coords.x;
	doom->grid.y = doom->grid.coords.y;
	doom->grid.dimensions = doom->grid.coords;
}

void	toolbox_init(t_editor *doom)
{
	t_xywh coord;

	coord = ui_init_coords(10, 10, doom->window->active_surface->w / 4, doom->window->active_surface->h - 20);
	doom->toolbox = bui_new_menu(doom->window, "toolbox", coord);
	bui_set_element_color(doom->toolbox, doom->palette.win_elem);

	coord = ui_init_coords(10, (doom->toolbox->position.h / 4 + 10),
			doom->toolbox->position.w - 20, (doom->toolbox->position.h / 4) * 3 - 20);
	doom->info_area = bui_new_element(doom->toolbox, "info area", coord);
	bui_set_element_color(doom->info_area, doom->palette.elem_elem);

	doom->hover_info = bui_new_element(doom->info_area, "hover info", (t_xywh) {10, 20, 200, 50});
	bui_set_element_color(doom->hover_info, doom->info_area->color);

	doom->selected_sector_info = bui_new_element(doom->info_area, "selected sector info", (t_xywh) {10, 75, 200, 50});
	bui_set_element_color(doom->selected_sector_info, doom->info_area->color);
}

void	button_init(t_editor *doom)
{
	t_xywh coord;

	coord = ui_init_coords(25, 25, 100, 50);
	doom->button_draw = bui_new_element(doom->toolbox, "draw", coord);
	bui_set_element_color(doom->button_draw, doom->palette.elem_elem);

	coord = ui_init_coords(150, 25, 100, 50);
	doom->button_select = bui_new_element(doom->toolbox, "select", coord);
	bui_set_element_color(doom->button_select, doom->palette.elem_elem);

	coord = ui_init_coords(275, 25, 100, 50);
	doom->button_save = bui_new_element(doom->toolbox, "save", coord);
	bui_set_element_color(doom->button_save, doom->palette.elem_elem);

// deletion button
	coord = ui_init_coords(doom->info_area->position.w - 110, 25, 100, 50);
	doom->button_remove = bui_new_element(doom->info_area, "remove", coord);
	bui_set_element_color(doom->button_remove, doom->palette.elem_elem_elem);
// edit button
	coord = ui_init_coords(doom->info_area->position.w - 110, 100, 100, 50);
	doom->button_edit = bui_new_element(doom->info_area, "edit", coord);
	bui_set_element_color(doom->button_edit, doom->palette.elem_elem_elem);
// add button
	coord = ui_init_coords(doom->info_area->position.w - 110, 175, 100, 50);
	doom->button_add = bui_new_element(doom->info_area, "add portal", coord);
	bui_set_element_color(doom->button_add, doom->palette.elem_elem_elem);
// remove button
	coord = ui_init_coords(doom->info_area->position.w - 110, 250, 100, 50);
	doom->button_remove_portal = bui_new_element(doom->info_area, "remove portal" , coord);
	bui_set_element_color(doom->button_remove_portal, doom->palette.elem_elem_elem);
}

void	color_palette_init(t_color_palette *pal)
{
	pal->win = 0xff0e1111;
	pal->win_elem = 0xff232b2b;
	pal->elem_elem = 0xff353839;
	pal->elem_elem_elem = 0xffa6a6a4;
}

/*
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

	doom->texture_buttons = (t_bui_element **)malloc(sizeof(t_bui_element *) * doom->textures[0].max_textures);
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
*/

/*
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

	doom->sprite_buttons = (t_bui_element **)malloc(sizeof(t_bui_element *) * doom->sprites[0].max_textures);
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
*/

/*
void	texture_init(t_editor *doom)
{
	load_texture(&doom->textures[0], "../textures/textures.bmp", 64, 64);
	split_texture(&doom->textures[0]);

	texture_button_init(doom);
}
*/

void	texture_buttons(t_editor *doom, t_grid *grid)
{
	int i;

	if (grid->modify_wall == NULL) // nothing to modify
		return ;
	i = 0;
	while (i < doom->textures[0].max_textures)
	{
		// NOTE: this here is so that it stands out which button you have pressed.
		// 	curretnly doesnt work in better libui.
		// 	(its all the commented out stuff.
		if (bui_button(doom->libui, doom->texture_buttons[i]))
		{
			grid->modify_wall->texture_id = i;
			/*
			doom->texture_buttons[i]->shadow = 0;
			doom->texture_buttons[i]->offset_x = 5;
			doom->texture_buttons[i]->offset_y = 5;
			*/
		}
		/*
		if (grid->modify_wall->texture_id != i)
		{
			doom->texture_buttons[i]->shadow = 1;
			doom->texture_buttons[i]->offset_x = 0;
			doom->texture_buttons[i]->offset_y = 0;
		}
		*/
		i++;
	}
}

/*
void	sprite_init(t_editor *doom)
{
	load_texture(&doom->sprites[0], "../textures/sprite.bmp", 128, 128);
	split_texture(&doom->sprites[0]);

	sprite_button_init(doom);
	doom->option.selected_sprite = -1;
}
*/

void	sprite_buttons(t_editor *doom, t_grid *grid)
{
	int i;

	if (grid->modify_wall == NULL) // nothing to modify
		return ;
	i = 0;
	while (i < doom->sprites[0].max_textures)
	{
		// TODO: this is so that its more obvious which button is pressed.
		// 	currently doesnt work in better libui
		if (bui_button(doom->libui, doom->sprite_buttons[i]))
		{
			doom->option.selected_sprite = i;
			/*
			doom->sprite_buttons[i]->shadow = 0;
			doom->sprite_buttons[i]->offset_x = 5;
			doom->sprite_buttons[i]->offset_y = 5;
			*/
		}
		/*
		if (doom->option.selected_sprite != i)
		{
			doom->sprite_buttons[i]->shadow = 1;
			doom->sprite_buttons[i]->offset_x = 0;
			doom->sprite_buttons[i]->offset_y = 0;
		}
		*/
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
	coord = ui_init_coords(25, 25, ((doom->edit_window->active_surface->w / 3) * 2), doom->edit_window->active_surface->h - (doom->edit_window->active_surface->h / 4));
	option->menu = bui_new_menu(doom->edit_window, "menu", coord);
	bui_set_element_color(option->menu, doom->palette.elem_elem);
// title bar
	coord = ui_init_coords(0, 0, option->menu->position.w, 20);
	option->title = bui_new_element(option->menu, "title", coord);
	bui_set_element_color(option->title, doom->palette.elem_elem_elem);
// view
	option->view = bui_new_element(option->menu, "view", (t_xywh){25, 45, option->menu->position.w - 50, option->menu->position.h - (25 + 45)});
	option->view->show = 0;
// info
	option->info = bui_new_menu(doom->edit_window, "info", (t_xywh){25, doom->edit_window->active_surface->h - (doom->edit_window->active_surface->h / 4) + 50, option->menu->position.w, doom->edit_window->active_surface->h - (doom->edit_window->active_surface->h - (doom->edit_window->active_surface->h / 4) + 75)});
	bui_set_element_color(option->info, doom->palette.elem_elem);
// render
	// WTF is this and why isnt it just the view thing? or is the view thing just a guide for where to uput this?
	//  jesus if i may say so myself
	option->show_render = bui_new_element(option->menu, "show render", (t_xywh){option->view->position.x, option->view->position.y,	option->view->position.w, option->view->position.h});
	option->show_render->show = 0;
// adding view
	option->add_view = bui_new_menu(doom->edit_window, "add view", (t_xywh){option->menu->position.x + option->menu->position.w + 25, 25, doom->edit_window->active_surface->w - (option->menu->position.x + option->menu->position.w) - 50, doom->edit_window->active_surface->h - 50});
	bui_set_element_color(option->add_view, doom->palette.elem_elem);
// textures
	// NOTE; this is like the textures tab button thingy majig
	option->texture_button = bui_new_element(option->add_view, "texture button", (t_xywh){25, 25, 100, 50});
	bui_set_element_color(option->texture_button, doom->palette.elem_elem_elem);
	option->texture_button->show = 0;

	option->textures = bui_new_element(option->add_view, "textures", (t_xywh){25, 100, option->add_view->position.w - 50, option->add_view->position.h - 125});
	option->textures->show = 0;
// sprite adding
	option->add_button = bui_new_element(option->add_view, "add sprite", (t_xywh){150, 25, 100, 50});
	bui_set_element_color(option->add_button, doom->palette.elem_elem_elem);
	option->add_button->show = 0;

	option->sprites = bui_new_element(option->add_view, "sprites", (t_xywh){25, 100, option->add_view->position.w - 50, option->add_view->position.h - 125});
	option->sprites->show = 0;
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
	prefab->text = bui_new_element(option->add_view, str, position);
	bui_set_element_color(prefab->text, doom->palette.elem_elem);
	prefab->text->show = 0;
// sub
	coord = ui_init_coords(175, 0, 50, 50);
	prefab->sub_button = bui_new_element(prefab->text, "-", coord);
	bui_set_element_color(prefab->sub_button, doom->palette.elem_elem_elem);
	prefab->sub_button->show = 1;
// amount
	coord = ui_init_coords(250, 0, 50, 50);
	prefab->amount = bui_new_element(prefab->text, "not set", coord);
	bui_set_element_color(prefab->amount, doom->palette.elem_elem);
	prefab->amount->show = 1;

	prefab->f_amount = NULL;
// add
	coord = ui_init_coords(300, 0, 50, 50);
	prefab->add_button = bui_new_element(prefab->text, "+", coord);
	bui_set_element_color(prefab->add_button, doom->palette.elem_elem_elem);
	prefab->add_button->show = 1;
}

void	sector_edit_buttons_init(t_editor *doom)
{
	t_xywh coord;
	t_wall_edit	*option;

	option = &doom->option;

	coord = ui_init_coords(25, 25, option->add_view->position.w - 50, 50);
	add_sector_button_prefab(doom, "floor height", coord);

	coord = ui_init_coords(25, 100, option->add_view->position.w - 50, 50);
	add_sector_button_prefab(doom, "ceiling height", coord);

	coord = ui_init_coords(25, 175, option->add_view->position.w - 50, 50);
	add_sector_button_prefab(doom, "gravity", coord);

	coord = ui_init_coords(25, 250, option->add_view->position.w - 50, 50);
	add_sector_button_prefab(doom, "lighting", coord);
}

/*
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

	doom->entity_sprite_buttons = (t_bui_element **)malloc(sizeof(t_bui_element *) * doom->entity_sprites[0].max_textures);
	gap = 25;
	size = 55;
	i = 0;
	amount = doom->option.ent_sprites->position.w / (gap + size);
	rect.w = doom->entity_sprites[0].x_size;
	rect.h = doom->entity_sprites[0].y_size;
	temp = create_surface(doom->entity_sprites[0].x_size, doom->entity_sprites[0].y_size);
	while (i < doom->entity_sprites[0].max_textures)
	{
		rect.x = doom->entity_sprites[0].position[i][0];
		rect.y = doom->entity_sprites[0].position[i][1];
		SDL_BlitSurface(doom->entity_sprites[0].surface, &rect, temp, NULL);
		x = (i % amount) * (gap + size) + gap;
		y = (i / amount) * (gap + size) + gap;
		coord = ui_init_coords(x, y, size, size);
		doom->entity_sprite_buttons[i] = bui_new_element(doom->option.ent_sprites, "temp", coord);
		// TODO: make this
		//ft_set_element_image(doom->entity_sprite_buttons[i], temp, NULL);
		i++;
	}
}
*/

/*
void	entity_sprite_init(t_editor *doom)
{
	load_texture(&doom->entity_sprites[0], "../textures/whiteofficer.bmp", 128, 128);
	split_texture(&doom->entity_sprites[0]);

	entity_sprite_button_init(doom);
}
*/

void	entity_sprite_buttons(t_editor *doom, t_grid *grid)
{
	int i;

	if (grid->modify_entity == NULL) // nothing to modify
		return ;
	i = 0;
	while (i < doom->entity_sprites[0].max_textures)
	{
		// NOTE: this makes it more obvious which button is pressed.
		// 	currently doesnt work in better_libui
		// 	(all the commented out code i mean)
		if (bui_button(doom->libui, doom->entity_sprite_buttons[i]))
		{
			grid->modify_entity->sprite_id = i;
			/*
			doom->entity_sprite_buttons[i]->shadow = 0;
			doom->entity_sprite_buttons[i]->offset_x = 5;
			doom->entity_sprite_buttons[i]->offset_y = 5;
			*/
		}
		/*
		if (grid->modify_entity->sprite_id != i)
		{
			doom->entity_sprite_buttons[i]->shadow = 1;
			doom->entity_sprite_buttons[i]->offset_x = 0;
			doom->entity_sprite_buttons[i]->offset_y = 0;
		}
		*/
		i++;
	}
}

void	entity_edit_button_init(t_editor *doom)
{
	t_xywh coord;
	t_wall_edit *option;

	option = &doom->option;
//"RENDER VIEW"
	option->ent_render_sprite = bui_new_element(option->menu, "entity render view", option->view->position);
	option->ent_render_sprite->show = 0;
// "SPRITE" tab
// the button
	coord = ui_init_coords(25, 25, 100, 50);
	option->ent_sprite_button = bui_new_element(option->add_view, "the button", coord);
	bui_set_element_color(option->ent_sprite_button, doom->palette.elem_elem_elem);
	option->ent_sprite_button->show = 0;
// the sprites
	coord = ui_init_coords(25, 100, option->add_view->position.w - 50, option->add_view->position.h - 125);
	option->ent_sprites = bui_new_element(option->add_view, "the sprites", coord);
	bui_set_element_color(option->ent_sprites, 0xffffffff);
	option->ent_sprites->show = 0;
//	"OTHER" tab
	coord = ui_init_coords(150, 25, 100, 50);
	option->ent_info_button = bui_new_element(option->add_view, "Other", coord);
	bui_set_element_color(option->ent_info_button, doom->palette.elem_elem_elem);
	option->ent_info_button->show = 0;
// the menu;
	coord = ui_init_coords(25, 125, option->add_view->position.w - 50, option->add_view->position.h - 150);
	option->ent_info_menu = bui_new_element(option->add_view, "the menu", coord);
	bui_set_element_color(option->ent_info_menu, doom->palette.elem_elem);
	option->ent_info_menu->show = 0;
// id text area
	coord = ui_init_coords(0, 0, 100, 50);
	option->ent_info_id_text = bui_new_element(option->ent_info_menu, "id: -1", coord);
	bui_set_element_color(option->ent_info_id_text, doom->palette.elem_elem);
// health text area
	coord = ui_init_coords(0, 75, 100, 50);
	option->ent_info_health_text = bui_new_element(option->ent_info_menu, "health: ", coord);
	bui_set_element_color(option->ent_info_health_text, doom->palette.elem_elem);

	coord = ui_init_coords(100, 75, option->ent_info_menu->position.w - 100, 50);
	option->ent_info_health_text_area = bui_new_element(option->ent_info_menu, " text area", coord);

// speed text area
	coord = ui_init_coords(0, 150, 100, 50);
	option->ent_info_speed_text = bui_new_element(option->ent_info_menu, "speed: ", coord);
	bui_set_element_color(option->ent_info_speed_text, doom->palette.elem_elem);

	coord = ui_init_coords(100, 150, option->ent_info_menu->position.w - 100, 50);
	option->ent_info_speed_text_area = bui_new_element(option->ent_info_menu, "text arae", coord);

// armor text area
	coord = ui_init_coords(0, 225, 100, 50);
	option->ent_info_armor_text = bui_new_element(option->ent_info_menu, "armor: ", coord);
	bui_set_element_color(option->ent_info_armor_text, doom->palette.elem_elem);

	coord = ui_init_coords(100, 225, option->ent_info_menu->position.w - 100, 50);
	option->ent_info_armor_text_area = bui_new_element(option->ent_info_menu, "text-area", coord);
// type drop down
	coord = ui_init_coords(0, 300, 200, 50);
	// TODO: make this drop down
	option->type_dropdown = bui_new_element(option->ent_info_menu, "select_type", coord);
	/*
	ft_drop_down_add_item(option->type_dropdown, "neutral");
	ft_drop_down_add_item(option->type_dropdown, "friendly");
	ft_drop_down_add_item(option->type_dropdown, "enemy");
	*/
}
