/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 13:59:23 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/19 14:23:13 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	add_portal(t_grid *grid)
{
	t_list	*sec;
	t_list	*wall;
	t_wall	*other;
	int		wall_one_sec;
	int		wall_two_sec;

	wall_one_sec = -1;
	wall_two_sec = -1;
	sec = grid->sectors;
	while (sec)
	{
		wall = ((t_sector *)sec->content)->walls;
		while (wall)
		{
			if (wall_compare(grid->modify_wall, wall->content))
			{
				if (grid->modify_wall == wall->content)
					wall_one_sec = ((t_sector *)sec->content)->id;
				else
				{
					other = wall->content;
					wall_two_sec = ((t_sector *)sec->content)->id;
				}
			}
			wall = wall->next;
		}
		sec = sec->next;
	}
	if (wall_two_sec == -1)
		return ;
	other->neighbor = wall_one_sec;
	grid->modify_wall->neighbor = wall_two_sec;
	if (other->neighbor != -1)
		other->solid = 0;
	if (grid->modify_wall->neighbor != -1)
		grid->modify_wall->solid = 0;
}

void	remove_portal(t_grid *grid)
{
	t_list	*s;
	t_list	*w;

	if (grid->modify_wall == NULL)
		return ;
	s = grid->sectors;
	while (s)
	{
		w = ((t_sector *)s->content)->walls;
		while (w)
		{
			if (wall_compare(w->content, grid->modify_wall))
			{
				((t_wall *)w->content)->neighbor = -1;
				((t_wall *)w->content)->solid = 1;
			}
			w = w->next;
		}
		s = s->next;
	}
}

int	entity_compare(t_entity *ent, t_entity *ity)
{
	if (vector_compare(ent->pos, ity->pos) &&
	ent->direction == ity->direction &&
	ft_strcmp(ent->preset->name, ity->preset->name) == 0)
		return (1);
	return (0);
}

void	remove_entity_from_list(t_list **entities, t_entity *entity)
{
	t_list	*curr;
	t_list	*prev;

	curr = *entities;
	if (curr && entity_compare(curr->content, entity))
	{
		*entities = curr->next;
		free_entity(curr->content, 0);
		free(curr);
	}
	else
	{
		while (curr)
		{
			if (entity_compare(curr->content, entity))
			{
				prev->next = curr->next;
				free_entity(curr->content, 0);
				free(curr);
			}
			else
				prev = curr;
			curr = prev->next;
		}
	}
}

void	other_box_events(t_editor *editor)
{
	if (bui_input(editor->map_name_input))
	{
		ft_strdel(&editor->mapname);
		editor->mapname = ft_strdup(editor->map_name_input->text);
		add_text_to_info_box(editor, "Map name changed successfully!");
	}
	if (bui_button(editor->button_save))
	{
		ft_strdel(&editor->mapname);
		editor->mapname = ft_strdup(editor->map_name_input->text);
		ft_strdel(&editor->fullpath);
		if (bui_button_toggle(editor->endless_tickbox))
			editor->fullpath = ft_sprintf(ROOT_PATH"map_editor/maps/%s%s",
					editor->mapname, ".endless");
		else if (bui_button_toggle(editor->story_tickbox))
			editor->fullpath = ft_sprintf(ROOT_PATH"map_editor/maps/%s%s",
					editor->mapname, ".story");
		set_map(editor);
		add_text_to_info_box(editor, "Map saved successfully!");
	}
	only_one_button_toggled_at_a_time(editor->map_type_tickboxes,
		&editor->active_map_type);
}

void	loop_buttons(t_editor *editor)
{
	t_rgba	new_col;

	other_box_events(editor);
	only_one_button_toggled_at_a_time(editor->select_mode_buttons,
		&editor->active_select_mode);
	if (editor->active_select_mode != NULL)
		bui_button_toggle(editor->active_select_mode);
	changer_prefab_events(editor->scaler, &editor->scale, 1);
	editor->scale = clamp(editor->scale, 1, 64);
	if (editor->info_box->text && SDL_GetTicks()
	- editor->info_box_start_timer >= editor->info_box_timer)
	{
		new_col = hex_to_rgba(editor->info_box->text_color);
		new_col.a -= 1;
		editor->info_box->text_color = rgba_to_hex(new_col);
		bui_change_element_text(editor->info_box,
			editor->info_box->text);
		if (new_col.a == 0)
			bui_remove_element_text(editor->info_box);	
	}
}

void	add_text_to_info_box(t_editor *editor, char *text)
{
	editor->info_box_start_timer = SDL_GetTicks();
	editor->info_box_timer = 5000;
	editor->info_box->text_color = 0x00ffffff;
	bui_change_element_text(editor->info_box, text);
}

int	get_list_len(t_list **list)
{
	int	i;
	t_list	*curr;

	i = 0;
	curr = *list;
	while (curr)
	{
		i++;
		curr = curr->next;
	}
	return (i);
}

void	re_id_sectors(t_list **sectors)
{
	t_list	*curr;
	int	id;

	id = 0;
	curr = *sectors;
	while (curr)
	{
		((t_sector *)curr->content)->id = id;
		curr = curr->next;
		id++;	
	}
}

void	recount_everything(t_editor *editor)
{
	t_list	*curr;
	t_list	*curr_sprite;

	editor->grid.sector_amount = get_list_len(&editor->grid.sectors);
	re_id_sectors(&editor->grid.sectors);
	editor->grid.wall_amount = 0;
	editor->grid.point_amount = get_list_len(&editor->grid.points);
	editor->grid.entity_amount = get_list_len(&editor->grid.entities);
	editor->grid.wall_sprite_amount = 0;
	curr = editor->grid.walls;
	while (curr)
	{
		editor->grid.wall_amount++;
		curr_sprite = ((t_wall *)curr->content)->sprites;
		while (curr_sprite)
		{
			editor->grid.wall_sprite_amount++;
			curr_sprite = curr_sprite->next;
		}
		curr = curr->next;
	}
}

void	remove_selected_point_from_all_walls_and_sectors(t_editor *editor)
{
	t_list	*wall;
	t_list	*sec;
	t_grid	*grid;

	grid = &editor->grid;
	wall = grid->walls;
	while (wall)
	{
		if (vector_in_wall(grid->modify_point->pos, wall->content))
		{
			sec = grid->sectors;
			while (sec)
			{
				remove_from_walls_non_free(
					&((t_sector *)sec->content)->walls,
					wall->content);
				sec = sec->next;
			}
			remove_from_walls(&grid->walls, wall->content);
			wall = grid->walls;
		}
		else
			wall = wall->next;
	}
	remove_from_points(&editor->grid.points, grid->modify_point);
}

void	remove_all_non_existing_sectors(t_editor *editor)
{
	t_list		*sec;
	t_sector	*sector;

	sec = editor->grid.sectors;
	while (sec)
	{
		sector = sec->content;
		sec = sec->next;
		if (sector && get_list_len(&sector->walls) == 0)
			remove_from_sectors(&editor->grid.sectors, sector);
	}
}

void	remove_all_lonely_points(t_editor *editor)
{
	t_list	*p;
	t_list	*wall;
	t_point	*point;
	int	found;

	found = 0;
	p = editor->grid.points;
	while (p)
	{
		point = p->content;
		p = p->next;
		wall = editor->grid.walls;
		while (wall)
		{
			if (vector_in_wall(point->pos, wall->content))
			{
				found = 1;
				break ;
			}
			wall = wall->next;
		}
		if (found == 0)
			remove_from_points(&editor->grid.points, point);
	}
}

void	remove_button_events(t_editor *editor, t_grid *grid)
{
	if (grid->modify_point != NULL)
		remove_selected_point_from_all_walls_and_sectors(editor);
	else if (grid->modify_wall != NULL)
	{
		remove_wall_from_its_sector(&editor->grid, grid->modify_wall);
		remove_from_walls(&editor->grid.walls, grid->modify_wall);
	}
	else if (grid->modify_sector != NULL)
	{
		remove_everything_from_list(&grid->modify_sector->walls);
		remove_from_sectors(&editor->grid.sectors, grid->modify_sector);
		remove_all_non_existing_portals(&editor->grid.sectors);
	}
	else if (grid->modify_entity != NULL)
		remove_entity_from_list(&grid->entities, grid->modify_entity);
	else
		return ;
	remove_all_non_existing_sectors(editor);
	remove_all_lonely_walls(&grid->walls, &grid->sectors);
	remove_all_lonely_points(editor);
	recount_everything(editor);
	grid->modify_point = NULL;
	grid->modify_wall = NULL;
	grid->modify_sector = NULL;
	grid->modify_entity = NULL;
}

void	selection_mode_buttons(t_editor *editor, t_grid *grid)
{
	if (bui_button(editor->button_remove))
		remove_button_events(editor, grid);
	else if (bui_button(editor->button_edit))
	{
		SDL_RestoreWindow(editor->new_edit_window->win);
		SDL_RaiseWindow(editor->new_edit_window->win);
	}
}
