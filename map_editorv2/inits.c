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
	doom->window = bui_new_window(libui, title, coord, 0, doom->palette.win);
	ft_strdel(&title);

	coord = ui_init_coords(500, 500, 1500, 750);
	doom->edit_window = bui_new_window(libui, "Editor", coord, 0, doom->palette.win);
	SDL_MinimizeWindow(doom->edit_window->win);
}

void	edit_window_init(t_editor *editor, t_bui_libui *libui)
{
	t_xywh coord;

	// New edit window, replace this with the old one after its done.
	coord = ui_init_coords(500, 500, 1000, 500);
	editor->new_edit_window = bui_new_window(libui, "New Editor", coord, 0, editor->palette.win);
	SDL_MinimizeWindow(editor->new_edit_window->win);
	
	init_sector_editor(editor);
	init_wall_editor(editor);
	init_entity_editor(editor);
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

	// map thingy
	doom->scale = 1;
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

	// TODO: put this in a infoarea_init function or something, but remove from this function
	doom->hover_info = bui_new_element(doom->info_area, "hover info", (t_xywh) {10, 20, 200, 50});
	bui_set_element_color(doom->hover_info, doom->info_area->color);

	doom->selected_sector_info = bui_new_element(doom->info_area, "selected sector info", (t_xywh) {10, 75, 200, 50});
	bui_set_element_color(doom->selected_sector_info, doom->info_area->color);

	// scale changer
	coord = ui_init_coords(300, 360, 100, 40);
	doom->scale_menu = bui_new_element(doom->info_area, "scale", coord);
	coord = ui_init_coords(0, 20, 100, 20);
	doom->scale_button = bui_new_element(doom->scale_menu, "-1", coord);
	doom->scale_button->text_x = 50;

	coord.x = 0;
	coord.y = 0;
	coord.w = 20;
	coord.h = 20;
	doom->scale_decrease = bui_new_element(doom->scale_button, "-", coord);
	coord.x = 80;
	doom->scale_increase = bui_new_element(doom->scale_button, "+", coord);
}

void	button_init(t_editor *doom)
{
	t_xywh coord;

	coord = ui_init_coords(25, 25, 100, 50);
	doom->button_draw = bui_new_element(doom->toolbox, "draw", coord);
	bui_set_element_color(doom->button_draw, doom->palette.elem_elem);
	draw_rect_border(doom->button_draw->surface[ELEMENT_CLICK], 0, 0, doom->button_draw->position.w, doom->button_draw->position.h, 0xff0000ff, 2);

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
		if (bui_button(doom->texture_buttons[i]))
		{
			grid->modify_wall->texture_id = i;
		}
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

// TODO: Redundant: this function might we redundrand
void	sprite_buttons(t_editor *doom, t_grid *grid)
{
	int i;

	if (grid->modify_wall == NULL) // nothing to modify
		return ;
	i = 0;
	while (i < doom->sprites[0].max_textures)
	{
		if (bui_button(doom->sprite_buttons[i]))
		{
			doom->option.selected_sprite = i;
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

t_sector_edit	*new_add_sector_button_prefab(t_bui_element *parent_menu, char *title, int x, int y)
{
	t_xywh coord;
	t_sector_edit *prefab;

	prefab = malloc(sizeof(t_sector_edit));
	memset(prefab, 0, sizeof(t_sector_edit));
// menu
	coord = ui_init_coords(x, y, 100, 40);
	prefab->menu = bui_new_element(parent_menu, title, coord);
// sub
	coord = ui_init_coords(0, 20, 20, 20);
	prefab->sub_button = bui_new_element(prefab->menu, "-", coord);
// amount
	coord = ui_init_coords(20, 20, 60, 20);
	prefab->amount = bui_new_element(prefab->menu, "not set", coord);
// add
	coord = ui_init_coords(100 - 20, 20, 20, 20);
	prefab->add_button = bui_new_element(prefab->menu, "+", coord);
	return (prefab);
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

void	init_sector_editor(t_editor *editor)
{
	t_xywh coord;

	// edit toolbox- and view element
	coord = ui_init_coords(5, 5, editor->new_edit_window->position.w * 0.20f, editor->new_edit_window->position.h - 10);
	editor->edit_toolbox_sector = bui_new_menu(editor->new_edit_window, "New Toolbox", coord);

	coord = ui_init_coords(editor->edit_toolbox_sector->position.x + editor->edit_toolbox_sector->position.w + 5, 5,
			editor->new_edit_window->position.w - editor->edit_toolbox_sector->position.w - 15,
			editor->new_edit_window->position.h - 10);
	editor->edit_view_sector = bui_new_menu(editor->new_edit_window, "New View", coord);

	// floor texture menu
	coord = ui_init_coords(0, 20,
			editor->edit_view_sector->position.w * 0.5,
			editor->edit_view_sector->position.h - 20);
	editor->sector_floor_menu = bui_new_element(editor->edit_view_sector, "Floor Texture", coord);

	// ceiling texture menu
	coord = ui_init_coords(editor->edit_view_sector->position.w * 0.5, 20,
			editor->edit_view_sector->position.w * 0.5,
			editor->edit_view_sector->position.h - 20);
	editor->sector_ceiling_menu = bui_new_element(editor->edit_view_sector, "Ceiling Texture", coord);
	bui_set_element_color(editor->sector_ceiling_menu, 0xff06D6A0);

// TODO: from a texture file take all the textures and make buttons of them and show them on both of the menus above.
	// this is just a demonstration
	t_bui_element *temp_elem;
	char *str;
	int floor_texture_count = 5;
	int i = 0;
	while (i++ < floor_texture_count)
	{
		// these coords need to be placed correctly on the view so that they lok nice aka use modulo
		coord = ui_init_coords(i * 20 + (i * 50), 50, 50, 50);
		str = ft_sprintf("%d", i);
		temp_elem = bui_new_element(editor->sector_floor_menu, str, coord);
		ft_strdel(&str);
		bui_set_element_color(temp_elem, 0xff06D6A0);
		add_to_list(&editor->floor_texture_buttons, temp_elem, sizeof(t_bui_element));
	}
	i = 0;
	while (i++ < floor_texture_count)
	{
		coord = ui_init_coords(i * 20 + (i * 50), 50, 50, 50);
		str = ft_sprintf("%d", i);
		temp_elem = bui_new_element(editor->sector_ceiling_menu, str, coord);
		ft_strdel(&str);
		bui_set_element_image_from_path(temp_elem, ELEMENT_DEFAULT, "../engine/ui/ui_images/doom.jpg");
		add_to_list(&editor->ceiling_texture_buttons, temp_elem, sizeof(t_bui_element));
	}

	// Init the ceiling- and floor height... etc. buttons
	editor->floor_height = new_add_sector_button_prefab(editor->edit_toolbox_sector, "floor height", 5, (25 * 1) + (40 * 0));
	editor->ceiling_height = new_add_sector_button_prefab(editor->edit_toolbox_sector, "ceiling height", 5, (25 * 2) + (40 * 1));
	editor->gravity = new_add_sector_button_prefab(editor->edit_toolbox_sector, "gravity", 5, (25 * 3) + (40 * 2));
	editor->lighting = new_add_sector_button_prefab(editor->edit_toolbox_sector, "lighting", 5, (25 * 4) + (40 * 3));
}

void	init_wall_editor(t_editor *editor)
{
	t_xywh coord;

	// edit toolbox- and view element
	coord = ui_init_coords(5, 5, editor->new_edit_window->position.w * 0.20f, editor->new_edit_window->position.h - 10);
	editor->edit_toolbox_wall = bui_new_menu(editor->new_edit_window, "New Toolbox", coord);

	coord = ui_init_coords(editor->edit_toolbox_sector->position.x + editor->edit_toolbox_sector->position.w + 5, 5,
			editor->new_edit_window->position.w - editor->edit_toolbox_sector->position.w - 15,
			editor->new_edit_window->position.h - 10);
	editor->edit_view_wall = bui_new_menu(editor->new_edit_window, "New View", coord);
	editor->edit_view_wall->update = 0;

	// tabsystem in toolbox
	coord = ui_init_coords(5, 20, editor->edit_toolbox_wall->position.w - 10, editor->edit_toolbox_wall->position.h - 25);
	editor->wall_tab = bui_new_tab_preset(editor->edit_toolbox_wall, "texture tabs", coord);
	// Note; if you want to add these tabs youre adding, you have to save them somewhere, [0] is tab button [1] is the view
	// Note2; it seems that i have planned to make the tab system more dynamic, aka you can have differnece sized buttons
	editor->wall_texture_view = preset_tab_add(editor->wall_tab, "Wall Texture")[1];
	editor->portal_texture_view = preset_tab_add(editor->wall_tab, "Portal Texture")[1];
	editor->wall_sprite_view = preset_tab_add(editor->wall_tab, "Wall Sprite")[1];

	// wall textures view elements
	coord = ui_init_coords(5, 20, 100, 40);
	editor->wall_scale = bui_new_element(editor->wall_texture_view, "texture scale", coord);
	bui_set_element_color(editor->wall_scale, 0xff06D6A0);

	coord = ui_init_coords(0, 20, 20, 20);
	editor->wall_scale_sub = bui_new_element(editor->wall_scale, "-", coord);
	bui_set_element_color(editor->wall_scale_sub, 0xff06D6A0);

	coord = ui_init_coords(20, 20, 60, 20);
	editor->wall_scale_value = bui_new_element(editor->wall_scale, "not set", coord);
	bui_set_element_color(editor->wall_scale_value, 0xff06D6A0);
	
	coord = ui_init_coords(80, 20, 20, 20);
	editor->wall_scale_add = bui_new_element(editor->wall_scale, "+", coord);
	bui_set_element_color(editor->wall_scale_add, 0xff06D6A0);

	// wall sprite view elements
	coord = ui_init_coords(5, 20, 50, 20);
	editor->add_wall_sprite_button = bui_new_element(editor->wall_sprite_view, "add sprite", coord);
	bui_set_element_color(editor->add_wall_sprite_button, 0xff06D6A0);

		// wall sprite scale elements
	coord = ui_init_coords(5, editor->wall_sprite_view->position.h - 45, 100, 40);
	editor->sprite_scale = bui_new_element(editor->wall_sprite_view, "sprite scale", coord);
	bui_set_element_color(editor->sprite_scale, 0xff06D6A0);

	coord = ui_init_coords(0, 20, 20, 20);
	editor->sprite_scale_sub = bui_new_element(editor->sprite_scale, "-", coord);
	bui_set_element_color(editor->sprite_scale_sub, 0xff06D6A0);

	coord = ui_init_coords(20, 20, 40, 20);
	editor->sprite_scale_value = bui_new_element(editor->sprite_scale, "not set", coord);
	bui_set_element_color(editor->sprite_scale_value, 0xff06D6A0);

	coord = ui_init_coords(80, 20, 20, 20);
	editor->sprite_scale_add = bui_new_element(editor->sprite_scale, "+", coord);
	bui_set_element_color(editor->sprite_scale_add, 0xff06D6A0);

	// TODO: make this modular on the y axis aswell
	t_bui_element *temp_elem;
	char *str;
	int texture_count = 5;
	int i = 0;
	int temp_x;

	// wall texture buttons
	editor->wall_texture_buttons = NULL;
	while (i++ < texture_count)
	{
		str = ft_itoa(i);
		temp_x = (i * 5) + ((i - 1) * 50);
		coord = ui_init_coords(temp_x, 70, 50, 50);
		temp_elem = bui_new_element(editor->wall_texture_view, str, coord);
		ft_strdel(&str);
		bui_set_element_image_from_path(temp_elem, ELEMENT_DEFAULT, "../engine/ui/ui_images/doom.jpg");
		bui_set_element_image_from_path(temp_elem, ELEMENT_CLICK, "../engine/ui/ui_images/doom.jpg");
		bui_set_element_image_from_path(temp_elem, ELEMENT_HOVER, "../engine/ui/ui_images/doom.jpg");
		draw_rect_border(temp_elem->surface[ELEMENT_CLICK], 0, 0, temp_elem->position.w, temp_elem->position.h, 0xff00ff00, 5);
		draw_rect_border(temp_elem->surface[ELEMENT_HOVER], 0, 0, temp_elem->position.w, temp_elem->position.h, 0xff0000ff, 5);
		add_to_list(&editor->wall_texture_buttons, temp_elem, sizeof(t_bui_element));
	}

	// portal texture buttons
	editor->portal_texture_buttons = NULL;
	texture_count = 2;
	i = 0;
	while (i++ < texture_count)
	{
		str = ft_itoa(i);
		temp_x = (i * 5) + ((i - 1) * 50);
		coord = ui_init_coords(temp_x, 50, 50, 50);
		temp_elem = bui_new_element(editor->portal_texture_view, str, coord);
		ft_strdel(&str);
		bui_set_element_image_from_path(temp_elem, ELEMENT_DEFAULT, "../engine/ui/ui_images/doom.jpg");
		bui_set_element_image_from_path(temp_elem, ELEMENT_CLICK, "../engine/ui/ui_images/doom.jpg");
		bui_set_element_image_from_path(temp_elem, ELEMENT_HOVER, "../engine/ui/ui_images/doom.jpg");
		draw_rect_border(temp_elem->surface[ELEMENT_CLICK], 0, 0, temp_elem->position.w, temp_elem->position.h, 0xff00ff00, 5);
		draw_rect_border(temp_elem->surface[ELEMENT_HOVER], 0, 0, temp_elem->position.w, temp_elem->position.h, 0xff0000ff, 5);
		add_to_list(&editor->portal_texture_buttons, temp_elem, sizeof(t_bui_element));
	}

	// wall sprite buttons
	editor->wall_sprite_buttons = NULL;
	texture_count = 1;
	i = 0;
	while (i++ < texture_count)
	{
		str = ft_itoa(i);
		temp_x = (i * 5) + ((i - 1) * 50);
		coord = ui_init_coords(temp_x, 50, 50, 50);
		temp_elem = bui_new_element(editor->wall_sprite_view, str, coord);
		ft_strdel(&str);
		bui_set_element_image_from_path(temp_elem, ELEMENT_DEFAULT, "../engine/ui/ui_images/doom.jpg");
		bui_set_element_image_from_path(temp_elem, ELEMENT_CLICK, "../engine/ui/ui_images/doom.jpg");
		bui_set_element_image_from_path(temp_elem, ELEMENT_HOVER, "../engine/ui/ui_images/doom.jpg");
		draw_rect_border(temp_elem->surface[ELEMENT_CLICK], 0, 0, temp_elem->position.w, temp_elem->position.h, 0xff00ff00, 5);
		draw_rect_border(temp_elem->surface[ELEMENT_HOVER], 0, 0, temp_elem->position.w, temp_elem->position.h, 0xff0000ff, 5);
		add_to_list(&editor->wall_sprite_buttons, temp_elem, sizeof(t_bui_element));
	}
}

/*
 ** Adds a new button at x, y to the prefered list.
*/
// TODO: remove the str, its only for debugging before i actually know how i will get the values  from the buttons for direction.
void	new_radio_button(t_list **list, t_bui_element *parent, int x, int y, char *str)
{
	t_xywh coord;
	t_bui_element *radio;

	coord = ui_init_coords(x, y, 15, 15);
	radio = bui_new_element(parent, str, coord);
	radio->text_y = -100; // This is because we dont want to display the text, only store it for the map file
	bui_set_element_color(radio, 0x00);
	bui_set_element_image_from_path(radio, ELEMENT_DEFAULT,"../engine/ui/ui_images/radio_button_off.png");
	bui_set_element_image_from_path(radio, ELEMENT_HOVER,"../engine/ui/ui_images/radio_button_hover.png");
	bui_set_element_image_from_path(radio, ELEMENT_CLICK,"../engine/ui/ui_images/radio_button_on.png");
	add_to_list(list, radio, sizeof(t_bui_element));
}

void	init_entity_editor(t_editor *editor)
{
	t_xywh coord;

	// Toolbox menu
	coord = ui_init_coords(5, 5, editor->new_edit_window->position.w * 0.20f, editor->new_edit_window->position.h - 10);
	editor->edit_toolbox_entity = bui_new_menu(editor->new_edit_window, "Entity Toolbox", coord);

	// View menu
	coord = ui_init_coords(editor->edit_toolbox_sector->position.x + editor->edit_toolbox_sector->position.w + 5, 5,
			editor->new_edit_window->position.w - editor->edit_toolbox_sector->position.w - 15,
			editor->new_edit_window->position.h - 10);
	editor->edit_view_entity = bui_new_menu(editor->new_edit_window, "Entity View", coord);

	// Drop down menu for all the entity preset types
	coord = ui_init_coords(5, 20, editor->edit_toolbox_entity->position.w - 10, 20);
	editor->entity_type_drop = bui_new_dropdown_preset(editor->edit_toolbox_entity, "Entity types", coord);

	// adding elements to the drop
	preset_dropdown_add_element(editor->entity_type_drop, "Medkit");
	preset_dropdown_add_element(editor->entity_type_drop, "Ammokit");
	preset_dropdown_add_element(editor->entity_type_drop, "Alfred");
	preset_dropdown_add_element(editor->entity_type_drop, "Peter");
	preset_dropdown_add_element(editor->entity_type_drop, "Carlos");

	// radio buttons for entity direction
	int start_x = editor->edit_toolbox_entity->position.w * 0.5f - 50; // this -50 == radio_parent->position.w * 0.5 
	int start_y = editor->edit_toolbox_entity->position.h * 0.5f;
	coord = ui_init_coords(start_x, start_y, 100, 100);
	t_bui_element *radio_parent = bui_new_element(editor->edit_toolbox_entity, "direction", coord);

	start_x = radio_parent->position.w * 0.5f - 7; // this -7 == radio_button->position.w * 0.5
	start_y = 25; 
	new_radio_button(&editor->entity_direction_radio_buttons,
			radio_parent, start_x, start_y, "90");
	new_radio_button(&editor->entity_direction_radio_buttons,
			radio_parent, start_x + 25, start_y + 25, "0");
	// setting default button
	editor->active_direction_button = editor->entity_direction_radio_buttons->content;

	new_radio_button(&editor->entity_direction_radio_buttons,
			radio_parent, start_x, start_y + 50, "270");
	new_radio_button(&editor->entity_direction_radio_buttons,
			radio_parent, start_x - 25, start_y + 25, "180");

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
		if (bui_button(doom->entity_sprite_buttons[i]))
		{
			grid->modify_entity->sprite_id = i;
		}
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
	option->ent_sprite_button = bui_new_element(option->add_view, "Sprites", coord);
	bui_set_element_color(option->ent_sprite_button, doom->palette.elem_elem_elem);
	option->ent_sprite_button->show = 0;
// the sprites view
	coord = ui_init_coords(25, 100, option->add_view->position.w - 50, option->add_view->position.h - 125);
	option->ent_sprites = bui_new_element(option->add_view, "the sprites", coord);
	bui_set_element_color(option->ent_sprites, 0xffffffff);
	option->ent_sprites->show = 0;
//	"OTHER" tab
	coord = ui_init_coords(150, 25, 100, 50);
	option->ent_info_button = bui_new_element(option->add_view, "Other", coord);
	bui_set_element_color(option->ent_info_button, doom->palette.elem_elem_elem);
	option->ent_info_button->show = 0;
// the menu; (this is the main viewer that changes when you click on the tabs.
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
