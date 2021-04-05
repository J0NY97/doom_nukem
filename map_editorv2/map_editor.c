#include "editor.h"

// NOTE: map_editor makes new window and libui.
void	map_editor(char *map)
{
	t_editor	*editor;
	t_bui_libui	*libui;

	editor = malloc(sizeof(t_editor));
	memset(editor, 0, sizeof(t_editor));

	libui = bui_new_libui();
	editor->libui = libui;
	window_init(editor, libui);
ft_printf("libui done!\n");
	editor->mapname = ft_strdup(map);
	editor->fullpath = ft_strjoiner("../../map_editorv2/maps/", editor->mapname, NULL);
ft_printf("Map Name set!\n");

	color_palette_init(&editor->palette);
ft_printf("palette done!\n");
	toolbox_init(editor);
ft_printf("toolbox done!\n");
	button_init(editor);
ft_printf("button done!\n");
	grid_init(editor);
ft_printf("Grid done!\n");

	
	// NOTE: this needs to be called before map getter so that we can put the correct preset in the correct entity
	init_entity_presets(&editor->entity_presets, "../../map_editorv2/entities.preset");
	// TODO: figure out better place for this.
	editor->default_entity = new_entity_preset();
	read_map_file(editor); // map getter
ft_printf("Map Got!\n");

	// TODO: why is this the only modify variable that is in the option and not grid ?
	editor->grid.modify_sprite = NULL;
	// New stuff
	edit_window_init(editor, libui);
ft_printf("Edit window init.\n");

	/*
	texture_init(doom);
	sprite_init(doom);
	entity_sprite_init(doom);
	*/
ft_printf("Starting to loop!\n");
	while (libui->run)
	{
		draw_grid(editor, &editor->grid);
		hover_calc(editor, &editor->grid);
		if (bui_button_toggle(editor->button_draw))
		{
			click_calc(editor, &editor->grid);
			check_selected(editor, &editor->grid);
		}
		else
		{
			// doom->grid.selected1 = EMPTY_VEC;
			// doom->grid.selected2 = EMPTY_VEC;
			selection(editor, &editor->grid, &libui->event);

			if (editor->grid.modify_wall == NULL) // improtant?
				editor->grid.modify_sprite = NULL;
			drag_calc(editor, &editor->grid, &libui->event);
			draw_selected_point(editor, &editor->grid);
			draw_selected_wall(editor, &editor->grid);
			draw_selected_sector(editor, &editor->grid);
			draw_selected_entity(editor, &editor->grid);
			selection_mode_buttons(editor, &editor->grid);
			selected_option_menu(editor, &editor->grid, libui);
		}
		unselect_selected(editor, &editor->grid, &libui->event);
		// boundaries(doom, &doom->grid);
		update_real_dimensions(editor, &editor->grid);
		//draw_walls(&doom->grid, &doom->grid.walls, 0xffffffff); // this is just for debugging purposes, to know if you remove a wall it actually is removed.
		draw_sectors(editor, &editor->grid);
		draw_points(editor, &editor->grid, editor->grid.points);
		draw_entities(editor, &editor->grid);
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
		draw_selected_sector_info(editor, &editor->grid);
		loop_buttons(editor);

		bui_render_the_event(libui);
	}
	ft_putstr("[map_editor] Bye!\n");
	
	bui_libui_quit(libui);
	return ;
}
