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
	t_xywh coord;

	coord = ui_init_coords(0, 0, 1920, 1080);
	//coord = ui_init_coords(0, 0, 1280, 720);
	char *title = ft_strjoiner("Doom Nukem Map Editor : ", doom->filename, NULL);
	//doom->window = bui_new_window(libui, title, coord, SDL_WINDOW_RESIZABLE, doom->palette.win);
	doom->window = bui_new_window(libui, title, coord, 0, doom->palette.win);
	ft_strdel(&title);
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

	// NOTE: enable this if you dont want the grid to be modular taken from the toolbox size
/*	coord = ui_init_coords(doom->window->active_surface->w / 6 + 20, 10,
			(doom->window->active_surface->w / 6) * 5 - 30, doom->window->active_surface->h - 20);
			*/
	coord.x = doom->toolbox->position.w + 20;
	coord.y = 10;
	coord.w = doom->window->active_surface->w - coord.x - 10;
	coord.h = doom->window->active_surface->h - 20;
	doom->grid.elem = bui_new_menu(doom->window, "grid", coord);
	bui_set_element_color(doom->grid.elem, doom->palette.win_elem);
	doom->grid.elem->update = 0;

	doom->grid.hover = EMPTY_VEC;
	doom->spawn.pos = (t_vector) {.x = 0, .y = 0, .z = 0};
	doom->spawn.direction = 0;
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
	t_editor *editor = doom;
	t_xywh coord;

	coord = ui_init_coords(10, 10, doom->window->active_surface->w / 6, doom->window->active_surface->h - 20);
	doom->toolbox = bui_new_menu(doom->window, "toolbox", coord);
	bui_set_element_color(doom->toolbox, doom->palette.win_elem);

	// Selection mode buttons
	int select_gap = 10;
	int select_w = 20;
	coord = ui_init_coords(70, 25, 150, 50);
	editor->select_mode = bui_new_element(editor->toolbox, "Select", coord);
	bui_set_element_color(editor->select_mode, editor->palette.elem_elem);
		//vertex button
	coord = ui_init_coords((0 * (select_w + select_gap)) + select_gap, 20, 20, 20);
	editor->select_mode_vertex = bui_new_element(editor->select_mode, "Vertex", coord);
		//wall button
	coord = ui_init_coords((1 * (select_w + select_gap)) + select_gap, 20, 20, 20);
	editor->select_mode_wall = bui_new_element(editor->select_mode, "Wall", coord);
		// sector button
	coord = ui_init_coords((2 * (select_w + select_gap)) + select_gap, 20, 20, 20);
	editor->select_mode_sector = bui_new_element(editor->select_mode, "Sector", coord);
		//entity button
	coord = ui_init_coords((3 * (select_w + select_gap)) + select_gap, 20, 20, 20);
	editor->select_mode_entity = bui_new_element(editor->select_mode, "Entity", coord);

	editor->select_mode_vertex->text_y = -20;
	editor->select_mode_wall->text_y = -20;
	editor->select_mode_sector->text_y = -20;
	editor->select_mode_entity->text_y = -20;
	bui_set_element_image_from_path(editor->select_mode_vertex, ELEMENT_DEFAULT, "../engine/ui/ui_images/selection_mode_vertex.png");
	bui_set_element_image_from_path(editor->select_mode_vertex, ELEMENT_HOVER, "../engine/ui/ui_images/selection_mode_vertex_click.png");
	bui_set_element_image_from_path(editor->select_mode_vertex, ELEMENT_CLICK, "../engine/ui/ui_images/selection_mode_vertex_click.png");
	bui_set_element_image_from_path(editor->select_mode_wall, ELEMENT_DEFAULT, "../engine/ui/ui_images/selection_mode_wall.png");
	bui_set_element_image_from_path(editor->select_mode_wall, ELEMENT_HOVER, "../engine/ui/ui_images/selection_mode_wall_click.png");
	bui_set_element_image_from_path(editor->select_mode_wall, ELEMENT_CLICK, "../engine/ui/ui_images/selection_mode_wall_click.png");
	bui_set_element_image_from_path(editor->select_mode_sector, ELEMENT_DEFAULT, "../engine/ui/ui_images/selection_mode_sector.png");
	bui_set_element_image_from_path(editor->select_mode_sector, ELEMENT_HOVER, "../engine/ui/ui_images/selection_mode_sector_click.png");
	bui_set_element_image_from_path(editor->select_mode_sector, ELEMENT_CLICK, "../engine/ui/ui_images/selection_mode_sector_click.png");
	bui_set_element_image_from_path(editor->select_mode_entity, ELEMENT_DEFAULT, "../engine/ui/ui_images/selection_mode_entity.png");
	bui_set_element_image_from_path(editor->select_mode_entity, ELEMENT_HOVER, "../engine/ui/ui_images/selection_mode_entity_click.png");
	bui_set_element_image_from_path(editor->select_mode_entity, ELEMENT_CLICK, "../engine/ui/ui_images/selection_mode_entity_click.png");
		// putting them in list
	// NOTE: these are there so that we can use the only one toggled at a time function
	editor->select_mode_buttons = NULL;
	editor->active_select_mode = NULL;
	add_to_list(&editor->select_mode_buttons, editor->select_mode_vertex, sizeof(t_bui_element));
	add_to_list(&editor->select_mode_buttons, editor->select_mode_wall, sizeof(t_bui_element));
	add_to_list(&editor->select_mode_buttons, editor->select_mode_sector, sizeof(t_bui_element));
	add_to_list(&editor->select_mode_buttons, editor->select_mode_entity, sizeof(t_bui_element));


	// info area init
	coord = ui_init_coords(10, (doom->toolbox->position.h / 4 + 10),
			doom->toolbox->position.w - 20, (doom->toolbox->position.h / 4) * 3 - 20);
	doom->info_area = bui_new_element(doom->toolbox, "info area", coord);
	bui_set_element_color(doom->info_area, doom->palette.elem_elem);

	// TODO: put this in a infoarea_init function or something, but remove from this function
	doom->hover_info = bui_new_element(doom->info_area, "hover info", (t_xywh) {10, 20, 100, 50});
	bui_set_element_color(doom->hover_info, doom->info_area->color);

	doom->selected_sector_info = bui_new_element(doom->info_area, "selected sector info", (t_xywh) {10, 75, 100, 50});
	bui_set_element_color(doom->selected_sector_info, doom->info_area->color);

	// new scale changer
	coord = ui_init_coords(editor->info_area->position.w - 110, 320, 100, 40);
	editor->scaler = new_changer_prefab(editor->info_area, "Scale", coord);
}

void	button_init(t_editor *doom)
{
	t_xywh coord;
	t_editor *editor = doom;

	int gap = 10;
	int button_w = 20;

	coord = ui_init_coords(10, 25, 50, 50);
	editor->draw_mode = bui_new_element(editor->toolbox, "Draw", coord);
	bui_set_element_color(editor->draw_mode, editor->palette.elem_elem);

	coord = ui_init_coords((0 * (button_w + gap)) + gap, 20, button_w, button_w);
	doom->button_draw = bui_new_element(editor->draw_mode, "draw", coord);
	doom->button_draw->text_y = -20;
	bui_set_element_image_from_path(editor->button_draw, ELEMENT_DEFAULT, "../engine/ui/ui_images/draw_mode.png");
	bui_set_element_image_from_path(editor->button_draw, ELEMENT_HOVER, "../engine/ui/ui_images/draw_mode_click.png");
	bui_set_element_image_from_path(editor->button_draw, ELEMENT_CLICK, "../engine/ui/ui_images/draw_mode_click.png");
	// add draw button to the list of select_buttons
	add_to_list(&editor->select_mode_buttons, editor->button_draw, sizeof(t_bui_element));

// Other mode
	coord = ui_init_coords(editor->select_mode->position.x + editor->select_mode->position.w + 10, editor->select_mode->position.y, 50, 50);
	editor->other_mode = bui_new_element(editor->toolbox, "Other", coord);
	bui_set_element_color(editor->other_mode, editor->palette.elem_elem);

	coord = ui_init_coords((0 * (button_w + gap)) + gap, 20, button_w, button_w);
	editor->button_save = bui_new_element(editor->other_mode, "save", coord);
	editor->button_save->text_y = -20;
	bui_set_element_image_from_path(editor->button_save, ELEMENT_DEFAULT, "../engine/ui/ui_images/save_button.png");
	bui_set_element_image_from_path(editor->button_save, ELEMENT_HOVER, "../engine/ui/ui_images/save_button_click.png");
	bui_set_element_image_from_path(editor->button_save, ELEMENT_CLICK, "../engine/ui/ui_images/save_button_click.png");

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

	// These are taken from coolors.co,
	// which is the reason they may have weird names
	pal->granny_smith_apple = 0xffadf7b6;
	pal->peach_crayola = 0xffffc09f;
	pal->light_blue = 0xffa0ced9;
}

// TODO: you can probably take from this mallia how to do the texture thingy majig when youre actually making it.
/*
void	texture_init(t_editor *doom)
{
	load_texture(&doom->textures[0], "../textures/textures.bmp", 64, 64);
	split_texture(&doom->textures[0]);

	texture_button_init(doom);
}
*/

t_changer_prefab	*new_changer_prefab(t_bui_element *parent_menu, char *title, t_xywh coord)
{
	t_xywh temp_coord;
	t_changer_prefab *prefab;

	prefab = ft_memalloc(sizeof(t_changer_prefab));
// menu
	prefab->menu = bui_new_element(parent_menu, title, coord);
// sub
	temp_coord = ui_init_coords(0, coord.h - 20, 20, 20);
	prefab->sub_button = bui_new_element(prefab->menu, "-", temp_coord);
// add
	temp_coord = ui_init_coords(coord.w - 20, coord.h - 20, 20, 20);
	prefab->add_button = bui_new_element(prefab->menu, "+", temp_coord);
// value
	temp_coord.x = prefab->sub_button->position.x + prefab->sub_button->position.w;
	temp_coord.w = prefab->menu->position.w - temp_coord.x - (prefab->menu->position.w - prefab->add_button->position.x);
	prefab->value = bui_new_element(prefab->menu, "not set", temp_coord);

	return (prefab);
}

// TODO: figure out how to do so it only changes the text if you actually clicked either of the buttons.
// TODO: maybe take in the value you want to change already in the initter. this would leave room for min and max here.
void	changer_prefab_events(t_changer_prefab *changer, int *current_value, int change_amount)
{
	char *str = NULL;
	
	if (bui_button(changer->add_button))
		*current_value += change_amount;
	else if (bui_button(changer->sub_button))
		*current_value -= change_amount;
	str = ft_sprintf("%d", *current_value);
	bui_change_element_text(changer->value, str);
	ft_strdel(&str);
}

void	changer_prefab_events_float(t_changer_prefab *changer, float *current_value, float change_amount)
{
	char *str = NULL;
	
	if (bui_button(changer->add_button))
		*current_value += change_amount;
	else if (bui_button(changer->sub_button))
		*current_value -= change_amount;
	str = ft_sprintf("%.1f", *current_value);
	bui_change_element_text(changer->value, str);
	ft_strdel(&str);
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
// TODO: NOTE: the ceiling- and floor texture count should be gotten from the same place as where you laod the textures.
	// this is just a demonstration
	t_bui_element *temp_elem;
	char *str;
	int floor_texture_count = 9;
	int ceiling_texture_count = 20;
	int i = 0;
	
	int offset_x = 20;
	int offset_y = 50;
	int button_gap = 20;
	int amount_on_x = floor(editor->sector_floor_menu->position.w / (50 + button_gap + offset_x));
	while (i < floor_texture_count)
	{
		coord.w = 50;
		coord.h = 50;
		coord.x = (i % (amount_on_x + 1)) * (coord.w + button_gap) + offset_x;
		coord.y = (i / (amount_on_x + 1)) * (coord.h + button_gap) + offset_y;
		str = ft_sprintf("%d", i);
		temp_elem = bui_new_element(editor->sector_floor_menu, str, coord);
		ft_strdel(&str);
		bui_set_element_color(temp_elem, 0xff06D6A0);
		add_to_list(&editor->floor_texture_buttons, temp_elem, sizeof(t_bui_element));
		i++;
	}
	i = 0;
	while (i < ceiling_texture_count)
	{
		coord = ui_init_coords(i * 20 + (i * 50), 50, 50, 50);
		coord.x = (i % (amount_on_x + 1)) * (coord.w + button_gap) + offset_x;
		coord.y = (i / (amount_on_x + 1)) * (coord.h + button_gap) + offset_y;
		str = ft_sprintf("%d", i);
		temp_elem = bui_new_element(editor->sector_ceiling_menu, str, coord);
		ft_strdel(&str);
		bui_set_element_image_from_path(temp_elem, ELEMENT_DEFAULT, "../engine/ui/ui_images/doom.jpg");
		add_to_list(&editor->ceiling_texture_buttons, temp_elem, sizeof(t_bui_element));
		i++;
	}

	// Init the ceiling- and floor height... etc. buttons
	coord.x = 5;
	coord.w = 100; 
	coord.h = 40; 
	coord.y = (25 * 1) + (40 * 0);
	editor->floor_height = new_changer_prefab(editor->edit_toolbox_sector, "floor height", coord);
	coord.y = (25 * 2) + (40 * 1);
	editor->ceiling_height = new_changer_prefab(editor->edit_toolbox_sector, "ceiling height", coord);
	coord.y = (25 * 3) + (40 * 2);
	editor->gravity = new_changer_prefab(editor->edit_toolbox_sector, "gravity", coord);
	coord.y = (25 * 4) + (40 * 3);
	editor->lighting = new_changer_prefab(editor->edit_toolbox_sector, "lighting", coord);

	// floor & ceiling texture scale
	coord.y = (25 * 6) + (40 * 5);
	editor->floor_scale = new_changer_prefab(editor->edit_toolbox_sector, "floor texture scale", coord);
	coord.y = (25 * 7) + (40 * 6);
	editor->ceiling_scale = new_changer_prefab(editor->edit_toolbox_sector, "ceiling texture scale", coord);

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
	// Note3; but yet not made it tho...
	t_bui_element **elems;
	
	elems = preset_tab_add(editor->wall_tab, "Wall Texture");
	bui_set_element_color(elems[0], editor->palette.light_blue);
	bui_set_element_color(elems[1], editor->palette.light_blue);
	editor->wall_texture_view = elems[1]; 

	elems = preset_tab_add(editor->wall_tab, "Portal Texture");
	bui_set_element_color(elems[0], editor->palette.granny_smith_apple);
	bui_set_element_color(elems[1], editor->palette.granny_smith_apple);
	editor->portal_texture_view = elems[1];

	elems = preset_tab_add(editor->wall_tab, "Wall Sprite");
	bui_set_element_color(elems[0], editor->palette.peach_crayola);
	bui_set_element_color(elems[1], editor->palette.peach_crayola);
	editor->wall_sprite_view = elems[1]; 

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

	// wall texture solidity tick box
	coord = ui_init_coords(115, 20, 100, 20);
	editor->wall_solid = bui_new_element(editor->wall_texture_view, "Solid:", coord);
	bui_set_element_color(editor->wall_solid, ((t_bui_element *)editor->wall_solid->parent)->color);

	coord = ui_init_coords(40, 0, 20, 20);
	editor->wall_solid_tick = bui_new_element(editor->wall_solid, " ", coord);
	bui_set_element_image_from_path(editor->wall_solid_tick, ELEMENT_DEFAULT, "../engine/ui/ui_images/tick_box_off.png");
	bui_set_element_image_from_path(editor->wall_solid_tick, ELEMENT_HOVER, "../engine/ui/ui_images/tick_box_hover.png");
	bui_set_element_image_from_path(editor->wall_solid_tick, ELEMENT_CLICK, "../engine/ui/ui_images/tick_box_on.png");


	// wall sprite view elements
	coord = ui_init_coords(5, 20, 50, 20);
	editor->add_wall_sprite_button = bui_new_element(editor->wall_sprite_view, "add sprite", coord);
	bui_set_element_color(editor->add_wall_sprite_button, 0xff06D6A0);

	coord = ui_init_coords(editor->add_wall_sprite_button->position.x + editor->add_wall_sprite_button->position.w + 15, 20, 50, 20);
	editor->remove_wall_sprite_button = bui_new_element(editor->wall_sprite_view, "remove sprite", coord);
	bui_set_element_color(editor->remove_wall_sprite_button, 0xff06D6A0);


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
	int button_w = 50;
	int button_gap = 15;
	int offset_x = 5;
	int offset_y = 70;
	int amount_on_x;

	// wall texture buttons
	editor->wall_texture_buttons = NULL;
	editor->active_wall_texture = NULL;

	amount_on_x = floor(editor->wall_texture_view->position.w / (button_w + button_gap + offset_x));
	coord.w = button_w;
	coord.h = button_w;
	while (i < texture_count)
	{
		str = ft_itoa(i);
		coord.x = (i % (amount_on_x + 1)) * (coord.w + button_gap) + offset_x;
		coord.y = (i / (amount_on_x + 1)) * (coord.h + button_gap) + offset_y;
		temp_elem = bui_new_element(editor->wall_texture_view, str, coord);
		ft_strdel(&str);
		bui_set_element_image_from_path(temp_elem, ELEMENT_DEFAULT, "../engine/ui/ui_images/doom.jpg");
		bui_set_element_image_from_path(temp_elem, ELEMENT_CLICK, "../engine/ui/ui_images/doom.jpg");
		bui_set_element_image_from_path(temp_elem, ELEMENT_HOVER, "../engine/ui/ui_images/doom.jpg");
		draw_rect_border(temp_elem->surface[ELEMENT_CLICK], 0, 0, temp_elem->position.w, temp_elem->position.h, 0xff00ff00, 5);
		draw_rect_border(temp_elem->surface[ELEMENT_HOVER], 0, 0, temp_elem->position.w, temp_elem->position.h, 0xff0000ff, 5);
		add_to_list(&editor->wall_texture_buttons, temp_elem, sizeof(t_bui_element));
		i++;
	}

	// portal texture buttons
	editor->portal_texture_buttons = NULL;
	editor->active_portal_texture = NULL;
	offset_y = 50;
	amount_on_x = floor(editor->portal_texture_view->position.w / (button_w + button_gap + offset_x));
	texture_count = 10;
	i = 0;
	while (i < texture_count)
	{
		str = ft_itoa(i);
		coord.x = (i % (amount_on_x + 1)) * (coord.w + button_gap) + offset_x;
		coord.y = (i / (amount_on_x + 1)) * (coord.h + button_gap) + offset_y;
		temp_elem = bui_new_element(editor->portal_texture_view, str, coord);
		ft_strdel(&str);
		bui_set_element_image_from_path(temp_elem, ELEMENT_DEFAULT, "../engine/ui/ui_images/doom.jpg");
		bui_set_element_image_from_path(temp_elem, ELEMENT_CLICK, "../engine/ui/ui_images/doom.jpg");
		bui_set_element_image_from_path(temp_elem, ELEMENT_HOVER, "../engine/ui/ui_images/doom.jpg");
		draw_rect_border(temp_elem->surface[ELEMENT_CLICK], 0, 0, temp_elem->position.w, temp_elem->position.h, 0xff00ff00, 5);
		draw_rect_border(temp_elem->surface[ELEMENT_HOVER], 0, 0, temp_elem->position.w, temp_elem->position.h, 0xff0000ff, 5);
		add_to_list(&editor->portal_texture_buttons, temp_elem, sizeof(t_bui_element));
		i++;
	}

	// wall sprite buttons
	editor->wall_sprite_buttons = NULL;
	editor->active_wall_sprite = NULL;
	amount_on_x = floor(editor->wall_sprite_view->position.w / (button_w + button_gap + offset_x));
	texture_count = 5;
	i = 0;
	while (i < texture_count)
	{
		str = ft_itoa(i);
		coord.x = (i % (amount_on_x + 1)) * (coord.w + button_gap) + offset_x;
		coord.y = (i / (amount_on_x + 1)) * (coord.h + button_gap) + offset_y;
		temp_elem = bui_new_element(editor->wall_sprite_view, str, coord);
		ft_strdel(&str);
		bui_set_element_image_from_path(temp_elem, ELEMENT_DEFAULT, "../engine/ui/ui_images/doom.jpg");
		bui_set_element_image_from_path(temp_elem, ELEMENT_CLICK, "../engine/ui/ui_images/doom.jpg");
		bui_set_element_image_from_path(temp_elem, ELEMENT_HOVER, "../engine/ui/ui_images/doom.jpg");
		draw_rect_border(temp_elem->surface[ELEMENT_CLICK], 0, 0, temp_elem->position.w, temp_elem->position.h, 0xff00ff00, 5);
		draw_rect_border(temp_elem->surface[ELEMENT_HOVER], 0, 0, temp_elem->position.w, temp_elem->position.h, 0xff0000ff, 5);
		add_to_list(&editor->wall_sprite_buttons, temp_elem, sizeof(t_bui_element));
		i++;
	}
}

/*
 ** Adds a new button at x, y to the prefered list.
*/
// TODO: add this to better_libui? this means that the textures should be saved there too.
// 	but dont add any textures before the dynamic texture path and font path is done.
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

// list is the linked list you will add the t_entity_preset to from path
// TODO: there are alot of sizeof() maybe store the size somewhere so you dont have to call is 20 times
// 	might not have a difference sine youre only calling this function when you startup the program.
// NOTE: first line in the file is just description of the value in that column.
// TODO: have an int array with all the different descriptions of value  and then take them from there when you store it...
// 	aka first read the first line and store those value descriptions and store them in an array, hard to explain.
// 	t.ex "name" = index 0, "scale" = index 1 ...
void	init_entity_presets(t_list **list, char *path)
{
	t_entity_preset *preset;
	char *line;
	char **arr;
	int fd;
	int i = 0;

	fd = open(path, O_RDONLY);
	get_next_line(fd, &line);
	ft_strdel(&line);
	while (get_next_line(fd, &line))
	{
		arr = ft_strsplit(line, '\t');
		preset = new_entity_preset();
		preset->name = ft_strdup(arr[0]);
		preset->scale = ft_atof(arr[1]);

		if (ft_strcmp(arr[2], "hostile") == 0)
			preset->mood = ENTITY_TYPE_HOSTILE;
		else if (ft_strcmp(arr[2], "affable") == 0)
			preset->mood = ENTITY_TYPE_FRIENDLY;
		else if (ft_strcmp(arr[2], "neutral") == 0)
			preset->mood = ENTITY_TYPE_NEUTRAL;

		preset->health = ft_atoi(arr[3]);
		preset->damage = ft_atoi(arr[4]);
		preset->speed = ft_atoi(arr[5]);

		if (ft_strcmp(arr[6], "melee") == 0)
			preset->attack_style = ENTITY_STYLE_MELEE;
		else if (ft_strcmp(arr[6], "ranged") == 0)
			preset->attack_style = ENTITY_STYLE_RANGED;
		else
			preset->attack_style = ENTITY_STYLE_NONE;

		preset->flying = ft_atoi(arr[7]);

		ft_strdel(&line);
		i++;
		add_to_list(list, preset, sizeof(t_entity_preset));
	}
ft_printf("[init_entity_presets] %d entity presets read.\n", i);
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
	t_list *curr;
	t_entity_preset *preset;

	curr = editor->entity_presets;
	while (curr)
	{
		preset = curr->content;
		preset_dropdown_add_element(editor->entity_type_drop, preset->name);
		curr = curr->next;
	}

	// radio buttons for entity direction
	int start_x = editor->edit_toolbox_entity->position.w * 0.5f - 50; // this -50 == radio_parent->position.w * 0.5 
	int start_y = editor->edit_toolbox_entity->position.h * 0.5f;
	coord = ui_init_coords(start_x, start_y, 100, 100);
	t_bui_element *radio_parent = bui_new_element(editor->edit_toolbox_entity, "direction", coord);

	start_x = radio_parent->position.w * 0.5f - 7; // this -7 == radio_button->position.w * 0.5
	start_y = 25; 
	new_radio_button(&editor->entity_direction_radio_buttons,
			radio_parent, start_x, start_y, "270");
	new_radio_button(&editor->entity_direction_radio_buttons,
			radio_parent, start_x + 25, start_y + 25, "0");

	new_radio_button(&editor->entity_direction_radio_buttons,
			radio_parent, start_x, start_y + 50, "90");
	new_radio_button(&editor->entity_direction_radio_buttons,
			radio_parent, start_x - 25, start_y + 25, "180");

}

