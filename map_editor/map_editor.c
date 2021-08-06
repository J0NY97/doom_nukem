/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:55:18 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 15:06:58 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	update_title_fps(SDL_Window *win, t_fps *fps)
{
	char	*str;

	str = ft_sprintf("Doom Editor [FPS: %d]", fps->fps);
	SDL_SetWindowTitle(win, str);
	ft_strdel(&str);
}

void	init(t_editor *editor, t_bui_libui *libui, char *map)
{
	editor->libui = libui;
	editor->fullpath = ft_strdup(map);
	editor->mapname = get_mapname_from_path(map);
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
	grid_init(editor);
	color_palette_init(&editor->palette);
	window_init(editor, libui);
	edit_window_init(editor, libui);
	bui_set_window_color(editor->window, 0xff000000);
	bui_set_window_color(editor->edit_window, 0xff000000);
	toolbox_init(editor);
	grid_init1(editor);
	read_map_file(editor);
}

void	general_events(t_editor *editor)
{
	update_general_info_element(editor);
	if (key_pressed(editor->libui, SDL_SCANCODE_B))
		unselect_selected(editor, &editor->grid);
	update_real_dimensions(&editor->grid);
	draw_sectors(&editor->grid);
	draw_points(&editor->grid, editor->grid.points);
	draw_entities(editor);
	gfx_draw_vector(editor->grid.elem->active_surface, 0xff00ff00, 6,
		gfx_vector_multiply(editor->spawn.pos, editor->grid.gap));
	if (!vector_is_empty(editor->grid.selected1)
		&& bui_button_toggle(editor->button_draw))
	{
		gfx_draw_line(editor->grid.elem->active_surface, 0xffffff00,
			gfx_vector_multiply(editor->grid.selected1, editor->grid.gap),
			gfx_vector_multiply(editor->grid.hover, editor->grid.gap));
		if (editor->grid.modify_sector != NULL
			&& editor->grid.modify_sector->first_point != NULL)
			gfx_draw_line(editor->grid.elem->active_surface, 0xffffff00,
				gfx_vector_multiply(editor->grid.hover, editor->grid.gap),
				gfx_vector_multiply(
					editor->grid.modify_sector->first_point->pos,
					editor->grid.gap));
	}
	draw_hover_info(editor, &editor->grid);
	loop_buttons(editor);
}

void	button_choice_event(t_editor *editor)
{
	if (movement(editor))
		return ;
	if (bui_button_toggle(editor->button_draw))
	{
		click_calc(editor, &editor->grid);
		check_selected(&editor->grid);
	}
	else
	{
		selection(editor, &editor->grid);
		if (editor->grid.modify_wall == NULL)
			editor->grid.modify_sprite = NULL;
		draw_selected_point(editor, &editor->grid);
		draw_selected_wall(&editor->grid);
		draw_selected_sector(editor, &editor->grid);
		draw_selected_entity(&editor->grid);
		selection_mode_buttons(editor, &editor->grid);
		selected_option_menu(editor, &editor->grid);
	}
}

void	map_editor(char *map)
{
	t_editor	*editor;
	t_bui_libui	*libui;
	t_fps		*fps;

	fps = ft_memalloc(sizeof(t_fps));
	editor = ft_memalloc(sizeof(t_editor));
	libui = bui_new_libui();
	init(editor, libui, map);
	while (libui->run)
	{
		bui_fps_func(fps);
		update_title_fps(editor->window->win, fps);
		bui_event_handler(libui);
		draw_grid(editor, &editor->grid);
		hover_calc(editor, &editor->grid);
		button_choice_event(editor);
		general_events(editor);
		bui_render(libui);
	}
	ft_putstr("[map_editor] Bye!\n");
	free(fps);
	bui_libui_quit(libui);
	editor_free(editor);
}
