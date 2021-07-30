/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 12:19:16 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 14:19:24 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	window_init(t_editor *editor, t_bui_libui *libui)
{
	t_xywh	coord;

	coord = new_xywh(10, 10, 1920, 1080);
	editor->window = bui_new_window(libui, "Doom Editor", coord, 0);
	bui_set_window_color(editor->window, editor->palette.win);
	bui_set_window_icon(editor->window, ICON_PATH"damage.png");
}

void	edit_window_init(t_editor *editor, t_bui_libui *libui)
{
	t_xywh	coord;

	coord = new_xywh(500, 500, 1000, 500);
	editor->edit_window = bui_new_window(libui, "Editor", coord, 0);
	bui_set_window_color(editor->edit_window, editor->palette.win);
	SDL_MinimizeWindow(editor->edit_window->win);
	editor->edit->menu = bui_new_menu(editor->window, "Editor", coord);
	init_sector_editor(editor);
	init_wall_editor(editor);
	init_entity_editor(editor);
}

void	grid_init1(t_editor *editor)
{
	t_xywh	coord;

	coord.x = editor->toolbox->position.w + 20;
	coord.y = 10;
	coord.w = editor->window->active_surface->w - coord.x - 10;
	coord.h = editor->window->active_surface->h - 20;
	editor->grid.elem = bui_new_menu(editor->window, "grid", coord);
	bui_set_element_color(editor->grid.elem, editor->palette.win_elem);
	editor->grid.elem->update = 0;
	editor->grid.hover = (t_vector){0, 0, 0};
	editor->grid.last_hover = (t_vector){0, 0, 0};
	editor->spawn.pos = (t_vector){.x = 0, .y = 0, .z = 0};
	editor->spawn.direction = 0;
	editor->scale = 1;
	editor->grid.font = TTF_OpenFont(TTF_PATH"DroidSans.ttf", 20);
}

void	grid_init(t_editor *editor)
{
	editor->grid.selected1 = (t_vector){0, 0, 0};
	editor->grid.selected2 = (t_vector){0, 0, 0};
	editor->grid.modify_wall = NULL;
	editor->grid.modify_point = NULL;
	editor->grid.modify_sector = NULL;
	editor->grid.modify_entity = NULL;
	editor->grid.modify_sprite = NULL;
	editor->grid.points = NULL;
	editor->grid.walls = NULL;
	editor->grid.sectors = NULL;
	editor->grid.entities = NULL;
	editor->grid.gap = 10;
	editor->grid.point_amount = 0;
	editor->grid.wall_amount = 0;
	editor->grid.sector_amount = 0;
	editor->grid.entity_amount = 0;
	editor->grid.x = editor->grid.coords.x;
	editor->grid.y = editor->grid.coords.y;
	editor->grid.dimensions = editor->grid.coords;
	load_all_textures(editor);
	init_entity_presets(&editor->entity_presets);
	editor->default_entity = new_entity_preset("default_entity_name");
}

void	color_palette_init(t_color_palette *pal)
{
	pal->win = 0xff0e1111;
	pal->win_elem = 0xff232b2b;
	pal->elem_elem = 0xff353839;
	pal->elem_elem_elem = 0xffa6a6a4;
	pal->light_gray = 0xffa6a6a4;
	pal->granny_smith_apple = 0xffadf7b6;
	pal->peach_crayola = 0xffffc09f;
	pal->light_blue = 0xffa0ced9;
}
