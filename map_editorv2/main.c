/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 12:27:49 by jsalmi            #+#    #+#             */
/*   Updated: 2020/09/30 13:02:56 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

int	main(int argc, char **argv)
{
	t_editor	*doom;
	t_bui_libui	*libui;

	if (!(doom = (t_editor *)malloc(sizeof(t_editor))))
		exit (1);
	memset(doom, 0, sizeof(t_editor));
	if (!(args_parser(&doom->filename, argc, argv)))
		exit (1);
	libui = bui_new_libui();
	doom->libui = libui;
	color_palette_init(&doom->palette);
	window_init(doom, libui);
	toolbox_init(doom);
	button_init(doom);
	grid_init(doom);

	
	// NOTE: this needs to be called before map getter so that we can put the correct preset in the correct entity
	init_entity_presets(&doom->entity_presets, "entities.preset");
	read_map_file(doom); // map getter

	option_menu_init(doom);
	sector_edit_buttons_init(doom);
	entity_edit_button_init(doom);

	// New stuff
	edit_window_init(doom, libui);

	/*
	texture_init(doom);
	sprite_init(doom);
	entity_sprite_init(doom);
	*/
	while (libui->run)
	{

		draw_grid(doom, &doom->grid);
		hover_calc(doom, &doom->grid);
		if (bui_button_toggle(doom->button_draw))
		{
			click_calc(doom, &doom->grid);
			check_selected(doom, &doom->grid);
		}
		else
		{
			// doom->grid.selected1 = EMPTY_VEC;
			// doom->grid.selected2 = EMPTY_VEC;
			selection(doom, &doom->grid, &libui->event);

			if (doom->grid.modify_wall == NULL) // improtant?
				doom->option.modify_sprite = NULL;
			drag_calc(doom, &doom->grid, &libui->event);
			draw_selected_point(doom, &doom->grid);
			draw_selected_wall(doom, &doom->grid);
			draw_selected_sector(doom, &doom->grid);
			draw_selected_entity(doom, &doom->grid);
			selection_mode_buttons(doom, &doom->grid);
			selected_option_menu(doom, &doom->grid, libui);
		}
		unselect_selected(doom, &doom->grid, &libui->event);
		// boundaries(doom, &doom->grid);
		update_real_dimensions(doom, &doom->grid);
		//draw_walls(&doom->grid, &doom->grid.walls, 0xffffffff); // this is just for debugging purposes, to know if you remove a wall it actually is removed.
		draw_sectors(doom, &doom->grid);
		draw_points(doom, &doom->grid, doom->grid.points);
		draw_entities(doom, &doom->grid);
		// draw spawn
		if (!vector_is_empty(doom->spawn.pos))
			gfx_draw_vector(doom->grid.elem->active_surface, 0xff00ff00, 6, gfx_vector_multiply(doom->spawn.pos, doom->grid.gap));
	// @Note: this is here so that it will be drawn on top of the walls
	// Draw line from the last positioned vertex to the current mouse hover position.
		if (!vector_is_empty(doom->grid.selected1) && bui_button_toggle(doom->button_draw))
		{
			gfx_draw_line(doom->grid.elem->active_surface, 0xffffff00,
						gfx_vector_multiply(doom->grid.selected1, doom->grid.gap),
						gfx_vector_multiply(doom->grid.hover, doom->grid.gap));
			if (doom->grid.modify_sector != NULL && doom->grid.modify_sector->first_point != NULL)
				gfx_draw_line(doom->grid.elem->active_surface, 0xffffff00,
							gfx_vector_multiply(doom->grid.hover, doom->grid.gap),
							gfx_vector_multiply(doom->grid.modify_sector->first_point->pos, doom->grid.gap));
		}
		draw_hover_info(doom, &doom->grid);
		draw_selected_sector_info(doom, &doom->grid);
		loop_buttons(doom);

		bui_render_the_event(libui);
	}
	ft_putstr("Bye!");
	return (0);
}
