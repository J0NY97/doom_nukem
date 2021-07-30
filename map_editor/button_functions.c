/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 13:59:23 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 15:27:51 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	select_mode_shortcut(t_editor *editor)
{
	if (key_pressed(editor->libui, SDL_SCANCODE_1))
		editor->active_select_mode = editor->button_draw;
	else if (key_pressed(editor->libui, SDL_SCANCODE_2))
		editor->active_select_mode = editor->select_mode_vertex;
	else if (key_pressed(editor->libui, SDL_SCANCODE_3))
		editor->active_select_mode = editor->select_mode_wall;
	else if (key_pressed(editor->libui, SDL_SCANCODE_4))
		editor->active_select_mode = editor->select_mode_sector;
	else if (key_pressed(editor->libui, SDL_SCANCODE_5))
		editor->active_select_mode = editor->select_mode_entity;
	if (key_pressed(editor->libui, SDL_SCANCODE_S))
		save_button_events(editor);
	if (key_pressed(editor->libui, SDL_SCANCODE_E)
		&& !key_repeat(editor->libui, SDL_SCANCODE_E))
		edit_button_event(editor);
}

void	loop_buttons(t_editor *editor)
{
	t_rgba	new_col;

	other_box_events(editor);
	select_mode_shortcut(editor);
	only_one_button_toggled_at_a_time(editor->select_mode_buttons,
		&editor->active_select_mode);
	if (editor->active_select_mode != NULL)
		bui_button_toggle(editor->active_select_mode);
	changer_prefab_events(editor->scaler, &editor->scale, 1);
	editor->scale = ft_clamp(editor->scale, 1, 64);
	if (editor->info_box->text && SDL_GetTicks()
		- editor->info_box_start_timer >= editor->info_box_timer)
	{
		new_col = hex_to_rgba(editor->info_box->text_color);
		new_col.a -= 1;
		editor->info_box->text_color = rgba_to_hex(new_col);
		bui_set_element_text_color(editor->info_box, rgba_to_hex(new_col));
		if (new_col.a == 0)
			bui_remove_element_text(editor->info_box);
	}
}

void	add_text_to_info_box(t_editor *editor, char *text)
{
	editor->info_box_start_timer = SDL_GetTicks();
	editor->info_box_timer = 5000;
	editor->info_box->text_color = 0x00ffffff;
	bui_set_element_text(editor->info_box, text);
}

void	re_id_sectors(t_list **sectors)
{
	t_list	*curr;
	int		id;

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

	editor->grid.sector_amount = ft_lstlen(editor->grid.sectors);
	re_id_sectors(&editor->grid.sectors);
	editor->grid.wall_amount = 0;
	editor->grid.point_amount = ft_lstlen(editor->grid.points);
	editor->grid.entity_amount = ft_lstlen(editor->grid.entities);
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
