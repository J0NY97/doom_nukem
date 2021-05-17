/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 13:59:23 by jsalmi            #+#    #+#             */
/*   Updated: 2021/05/17 18:27:44 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	add_portal(t_grid *grid)
{
	t_list *curr_wall;
	t_list *curr_sec;
	t_wall *wall_one;
	int		wall_one_sector;
	t_wall *wall_two;
	int		wall_two_sector;

	if (grid->modify_wall == NULL)
		return ;
ft_printf("Adding portal\n");
	wall_one = NULL;
	wall_two = NULL;
	wall_one_sector = -1;
	wall_two_sector = -1;
	curr_sec = grid->sectors;
	while (curr_sec)
	{
		curr_wall = ((t_sector *)curr_sec->content)->walls;
		while (curr_wall)
		{
			if (wall_compare(grid->modify_wall, curr_wall->content))
			{
				if (wall_one == NULL)
				{
					wall_one = curr_wall->content;
					wall_one_sector = ((t_sector *)curr_sec->content)->id;
				}
				else if (wall_two == NULL)
				{
					wall_two = curr_wall->content;
					wall_two_sector = ((t_sector *)curr_sec->content)->id;
				}
				else
					ft_printf("Error: add portal\n");
			}
			curr_wall = curr_wall->next;
		}
		curr_sec = curr_sec->next;
	}
	if (wall_one != NULL && wall_two != NULL)
	{
		wall_one->neighbor = wall_two_sector;
		wall_two->neighbor = wall_one_sector;
		wall_one->solid = 0;
		wall_two->solid = 0;
	}
}

void	remove_portal(t_grid *grid)
{
	t_list *s;
	t_list *w;

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

int		entity_compare(t_entity *ent, t_entity *ity)
{
	if (vector_compare(ent->pos, ity->pos) &&
		ent->direction == ity->direction &&
		ft_strcmp(ent->preset->name, ity->preset->name) == 0)
	{
		return (1);
	}
	return (0);
}

void	remove_entity_from_list(t_list **entities, t_entity *entity)
{
	t_list *curr;
	t_list *prev;

	curr = *entities;
	if (curr == NULL)
		return ;
	if (entity_compare(curr->content, entity))
	{
		*entities = curr->next;
		free(curr->content);
		free(curr);
		return ;
	}
	else
	{
		while (curr)
		{
			if (entity_compare(curr->content, entity))
			{
				prev->next = curr->next;
				free(curr->content);
				free(curr);
				return ;
			}
			else
				prev = curr;
			curr = prev->next;
		}
	}
}

void	loop_buttons(t_editor *editor)
{
	if (bui_button(editor->button_save))
	{
		ft_strdel(&editor->fullpath);
		// TODO: the .doom should be either endless or story, when you have the tickboxes for them.
		if (bui_button_toggle(editor->endless_tickbox))
			editor->fullpath = ft_sprintf("./maps/%s%s", editor->mapname, ".endless");
		else if (bui_button_toggle(editor->story_tickbox))
			editor->fullpath = ft_sprintf("./maps/%s%s", editor->mapname, ".story");
		else
			editor->fullpath = ft_sprintf("./maps/%s%s", editor->mapname, ".doom");
		set_map(editor);
	}

	// Returns 1 when enter is pressed.
	if (bui_input(editor->map_name_input))
	{
ft_printf("Map name was changed from %s ", editor->mapname);
		ft_strdel(&editor->mapname);
		editor->mapname = ft_strdup(editor->map_name_input->text);
ft_printf("to %s.\n", editor->mapname);
	}
	only_one_button_toggled_at_a_time(editor->map_type_tickboxes, &editor->active_map_type);


	// NOTE: the draw button is in this select_mode_buttons list
	// the selection mode buttons
	only_one_button_toggled_at_a_time(editor->select_mode_buttons, &editor->active_select_mode);
	if (editor->active_select_mode != NULL)
		bui_button_toggle(editor->active_select_mode);

	// scale changer prefab
	changer_prefab_events(editor->scaler, &editor->scale, 1);
	editor->scale = clamp(editor->scale, 1, 64);
}

void	recount_everything(t_editor *doom)
{
	t_list *curr;
	int id;

	doom->grid.wall_amount = 0;
	doom->grid.point_amount = 0;
	doom->grid.entity_amount = 0;
	doom->grid.wall_sprite_amount = 0;
	// id = 0;
	// curr = doom->grid.sectors;
	// while (curr)
	// {
	// 	((t_sector *)curr->content)->id = id;
	// 	id++;
	// 	curr = curr->next;
	// }
	// doom->grid.sector_amount = id;
	curr = doom->grid.walls;
	t_list *curr_sprite;
	while (curr)
	{
		doom->grid.wall_amount++;

		curr_sprite = ((t_wall *)curr->content)->sprites;
		while (curr_sprite)
		{
			doom->grid.wall_sprite_amount++;
			curr_sprite = curr_sprite->next;
		}

		curr = curr->next;
	}
	curr = doom->grid.points;
	while (curr)
	{
		doom->grid.point_amount++;
		curr = curr->next;
	}
	id = 0;
	curr = doom->grid.entities;
	while (curr)
	{
		((t_entity *)curr->content)->id = id;
		id++;
		curr = curr->next;
	}
	doom->grid.entity_amount = id;
}

void	selection_mode_buttons(t_editor *doom, t_grid *grid)
{
	if (bui_button(doom->button_remove))
	{
		if (grid->modify_point != NULL)
		{
			// Loop through all sectors
			//	Loop through all walls in that sector
			//	 Check if the point you have selected is in wall
			//	  Remove that wall from all sectors
			ft_printf("Starting to remove point.\n");
			t_list		*wall;
			t_list *sec;

			wall = grid->walls;
			while (wall)
			{
				if (vector_in_wall(grid->modify_point->pos, wall->content))
				{
					ft_printf("Point with that vector found in a wall.\n");
					sec = grid->sectors;
					while (sec)
					{
						remove_from_walls_non_free(&((t_sector *)sec->content)->walls, wall->content);
						sec = sec->next;
					}
					remove_from_walls(&grid->walls, wall->content);
					wall = grid->walls;
				}
				else
					wall = wall->next;
			}
			remove_from_points(&doom->grid.points, grid->modify_point);
		}
		else if (grid->modify_wall != NULL)
		{
			printf("Removing wall.\n");
			remove_wall_from_its_sector(&doom->grid, grid->modify_wall);
			remove_from_walls(&doom->grid.walls, grid->modify_wall);
		}
		else if (grid->modify_sector != NULL)
		{
			ft_putstr("Removing sector.\n");
			remove_everything_from_list(&grid->modify_sector->walls);
			remove_from_sectors(&doom->grid.sectors, grid->modify_sector);
			remove_all_walls_not_a_part_of_a_sector(&grid->walls, &grid->sectors);
			remove_all_points_not_a_part_of_a_wall(&grid->points, &grid->walls);
			remove_all_non_existing_portals(&doom->grid.sectors);
		}
		else if (grid->modify_entity != NULL)
		{
			ft_putstr("Removing entity.\n");
			remove_entity_from_list(&grid->entities, grid->modify_entity);
		}
		else
			return ;
		recount_everything(doom);
		grid->modify_point = NULL;
		grid->modify_wall = NULL;
		grid->modify_sector = NULL;
		grid->modify_entity = NULL;
	}
	else if (bui_button(doom->button_edit))
	{
		SDL_RestoreWindow(doom->new_edit_window->win);
		SDL_RaiseWindow(doom->new_edit_window->win);
	}
}
