/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:43:34 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 16:08:27 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	other_box_events(t_editor *editor)
{
	if (bui_input(editor->map_name_input))
	{
		ft_strdel(&editor->mapname);
		editor->mapname = ft_strdup(editor->map_name_input->text);
		add_text_to_info_box(editor, "Map name changed successfully!");
	}
	if (bui_button(editor->button_save))
		save_button_events(editor);
	only_one_button_toggled_at_a_time(editor->map_type_tickboxes,
		&editor->active_map_type);
}

void	save_button_events(t_editor *editor)
{
	ft_strdel(&editor->mapname);
	editor->mapname = ft_strdup(editor->map_name_input->text);
	ft_strdel(&editor->fullpath);
	if (bui_button_toggle(editor->endless_tickbox))
		editor->fullpath = ft_sprintf(MAP_PATH"%s%s",
				editor->mapname, ".endless");
	else if (bui_button_toggle(editor->story_tickbox))
		editor->fullpath = ft_sprintf(MAP_PATH"%s%s",
				editor->mapname, ".story");
	remove_all_non_existing_sectors(editor);
	remove_all_lonely_walls(&editor->grid.walls, &editor->grid.sectors);
	remove_all_non_existing_portals(&editor->grid.sectors);
	remove_all_lonely_points(editor);
	recount_everything(editor);
	set_map(editor);
	add_text_to_info_box(editor, "Map saved successfully!");
}

void	remove_button_events(t_editor *editor, t_grid *grid)
{
	if (grid->modify_point != NULL)
		remove_selected_point_from_all_walls_and_sectors(editor);
	else if (grid->modify_wall != NULL)
	{
		remove_wall_from_its_sector(&editor->grid, grid->modify_wall);
		remove_from_list_if_with(&editor->grid.walls, grid->modify_wall,
			&pointer_compare, &free_wall);
	}
	else if (grid->modify_sector != NULL)
	{
		ft_lstdel(&grid->modify_sector->walls, &dummy_free_er);
		remove_from_list_if_with(&editor->grid.sectors, grid->modify_sector,
			&pointer_compare, &free_sector);
		grid->modify_sector = NULL;
	}
	else if (grid->modify_entity != NULL)
		remove_from_list_if_with(&grid->entities, grid->modify_entity,
			&pointer_compare, &free_entity);
	else
		return ;
	remove_all_non_existing_sectors(editor);
	remove_all_lonely_walls(&grid->walls, &grid->sectors);
	remove_all_non_existing_portals(&editor->grid.sectors);
	remove_all_lonely_points(editor);
	recount_everything(editor);
	unselect_selected(editor, grid);
}

void	edit_button_event(t_editor *editor)
{
	SDL_RestoreWindow(editor->edit_window->win);
	SDL_RaiseWindow(editor->edit_window->win);
}

void	selection_mode_buttons(t_editor *editor, t_grid *grid)
{
	if (bui_button(editor->button_remove))
		remove_button_events(editor, grid);
	else if (bui_button(editor->button_edit))
		edit_button_event(editor);
}
