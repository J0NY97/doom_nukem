#include "editor.h"

void	update_title_fps(SDL_Window *win, t_fps *fps)
{
	char	*str;

	str = ft_sprintf("Doom Editor [FPS: %d]", fps->fps);
	SDL_SetWindowTitle(win, str);
	ft_strdel(&str);
}

// NOTE: map_editor makes new window and libui.
void	map_editor(char *map)
{
	t_editor	*editor;
	t_bui_libui	*libui;

	t_fps *fps = ft_memalloc(sizeof(t_fps));
	editor = ft_memalloc(sizeof(t_editor));

	libui = bui_new_libui();

	// basically send mouse events when focus change.
	// doesnt work for some reason, it works the first couple times and then nothing.... hello my name is sdl
	SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
//	SDL_SetHintWithPriority(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1", SDL_HINT_OVERRIDE);
//	ft_printf("Hint set? %s\n", SDL_GetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH));

	editor->libui = libui;

	editor->fullpath = ft_strdup(map);
	// Spaghett way of getting the map name from fullpath
	char **mapname = ft_strsplit(map, '/');
	char *final;
	int i = 0;
	while (mapname[i + 1])
		i++;
	if (ft_strendswith(mapname[i], ".endless") == 0)
		final = ft_strremove(mapname[i], ".endless");
	else if (ft_strendswith(mapname[i], ".story") == 0)
		final = ft_strremove(mapname[i], ".story");
	else if (ft_strendswith(mapname[i], ".doom") == 0)
		final = ft_strremove(mapname[i], ".doom");
	else
		final = ft_strdup(mapname[i]);
	editor->mapname = ft_strdup(final);
//	ft_strdel(&final); // for some reason you dont have to free this....
	free_array(mapname);
	// End of spaghett

ft_printf("Map Name set to: %s, %s\n", editor->mapname, editor->fullpath);


	grid_init(editor);
ft_printf("Grid done!\n");
	color_palette_init(&editor->palette);
ft_printf("palette done!\n");
	window_init(editor, libui);
ft_printf("Main window init.\n");
	edit_window_init(editor, libui);
ft_printf("Edit window init.\n");

	bui_set_window_color(editor->window, 0xff000000);
	bui_set_window_color(editor->new_edit_window, 0xff000000);

ft_printf("libui done!\n");


	toolbox_init(editor);
ft_printf("toolbox done!\n");
	info_area_init(editor);
ft_printf("Info area done!\n");
	button_init(editor);
ft_printf("button done!\n");
	grid_init1(editor);
ft_printf("Grid1 done!\n");

ft_printf("Ntity din dan don!\n");
	read_map_file(editor); // map getter
ft_printf("Map Got!\n");

	editor->grid.modify_sprite = NULL;
	// New stuff

	//texture_init(doom);
	//sprite_init(doom);
	//entity_sprite_init(doom);
ft_printf("Starting to loop!\n");
	while (libui->run)
	{
		bui_fps_func(fps);
		update_title_fps(editor->window->win, fps);
		bui_event_handler_new(libui);

		draw_grid(editor, &editor->grid);
		hover_calc(editor, &editor->grid);
		if (bui_button_toggle(editor->button_draw))
		{
			click_calc(editor, &editor->grid);
			check_selected(&editor->grid);
		}
		else
		{
			// doom->grid.selected1 = EMPTY_VEC;
			// doom->grid.selected2 = EMPTY_VEC;
			selection(editor, &editor->grid);

			if (editor->grid.modify_wall == NULL) // improtant?
				editor->grid.modify_sprite = NULL;
			drag_calc(editor, &editor->grid);
			draw_selected_point(editor, &editor->grid);
			draw_selected_wall(&editor->grid);
			draw_selected_sector(editor, &editor->grid);
			draw_selected_entity(&editor->grid);
			selection_mode_buttons(editor, &editor->grid);
			selected_option_menu(editor, &editor->grid);
		}
		unselect_selected(editor, &editor->grid);
		// boundaries(doom, &doom->grid);
		update_real_dimensions(&editor->grid);
		//draw_walls(&doom->grid, &doom->grid.walls, 0xffffffff); // this is just for debugging purposes, to know if you remove a wall it actually is removed.
		draw_sectors(&editor->grid);
		draw_points(&editor->grid, editor->grid.points);
		draw_entities(editor);
		// draw spawn
		if (!vector_is_empty(editor->spawn.pos))
			gfx_draw_vector(editor->grid.elem->active_surface, 0xff00ff00, 6, gfx_vector_multiply(editor->spawn.pos, editor->grid.gap));
	// @Note: this is here so that it will be drawn on top of the walls
	// Draw line from the last positioned vertex to the current mouse hover position.
		if (!vector_is_empty(editor->grid.selected1) && bui_button_toggle(editor->button_draw))
		{
			gfx_draw_line(editor->grid.elem->active_surface, 0xffffff00,
						gfx_vector_multiply(editor->grid.selected1, editor->grid.gap),
						gfx_vector_multiply(editor->grid.hover, editor->grid.gap));
			if (editor->grid.modify_sector != NULL && editor->grid.modify_sector->first_point != NULL)
				gfx_draw_line(editor->grid.elem->active_surface, 0xffffff00,
							gfx_vector_multiply(editor->grid.hover, editor->grid.gap),
							gfx_vector_multiply(editor->grid.modify_sector->first_point->pos, editor->grid.gap));
		}
		draw_hover_info(editor, &editor->grid);
		loop_buttons(editor);

		bui_render_new(libui);
	}
	ft_putstr("[map_editor] Bye!\n");

	editor_free(editor);
	bui_libui_quit(libui);
	return ;
}
