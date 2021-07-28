/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fandc_edit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:42:12 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:42:13 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	loop_all_fandc_texture_buttons(t_editor *editor)
{
	t_list	*curr;

	curr = editor->sector_texture_buttons;
	while (curr)
	{
		if (((t_bui_element *)curr->content)->was_clicked_last_frame)
		{
			if (mouse_pressed(editor->libui, SDL_BUTTON_LEFT))
				editor->active_floor_texture = curr->content;
			else if (mouse_pressed(editor->libui, SDL_BUTTON_RIGHT))
				editor->active_ceiling_texture = curr->content;
		}
		curr = curr->next;
	}
}

void	sector_f_and_c_button_events(t_editor *editor)
{
	char	*ceil_tex;
	char	*floor_tex;

	ceil_tex = ft_itoa(editor->grid.modify_sector->ceiling_texture);
	floor_tex = ft_itoa(editor->grid.modify_sector->floor_texture);
	editor->active_ceiling_texture = bui_get_element_with_text_from_list(
			editor->sector_texture_buttons, ceil_tex);
	editor->active_floor_texture = bui_get_element_with_text_from_list(
			editor->sector_texture_buttons, floor_tex);
	ft_strdel(&ceil_tex);
	ft_strdel(&floor_tex);
	loop_all_fandc_texture_buttons(editor);
	editor->grid.modify_sector->floor_texture
		= ft_atoi(editor->active_floor_texture->text);
	editor->grid.modify_sector->ceiling_texture
		= ft_atoi(editor->active_ceiling_texture->text);
}

void	draw_selected_f_and_c_button(t_editor *editor)
{
	t_xywh	c;

	if (editor->active_floor_texture)
	{
		c = new_xywh(0, 0,
				editor->active_floor_texture->active_surface->w,
				editor->active_floor_texture->active_surface->h);
		draw_rect_border(editor->active_floor_texture->active_surface,
			c, 0xff0000ff, 2);
	}
	if (editor->active_ceiling_texture)
	{
		c = new_xywh(2, 2,
				editor->active_ceiling_texture->active_surface->w - 4,
				editor->active_ceiling_texture->active_surface->h - 4);
		draw_rect_border(
			editor->active_ceiling_texture->active_surface,
			c, 0xff00ff00, 2);
	}
}

void	floor_ceiling_id_changer_prefab_events(t_editor *editor)
{
	int	wall_amount;

	wall_amount = ft_lstlen(editor->grid.modify_sector->walls);
	changer_prefab_events(editor->slope_floor_wall_changer,
		&editor->grid.modify_sector->floor_slope_wall_id, 1);
	if (editor->grid.modify_sector->floor_slope_wall_id >= wall_amount)
		editor->grid.modify_sector->floor_slope_wall_id = 0;
	else if (editor->grid.modify_sector->floor_slope_wall_id < 0)
		editor->grid.modify_sector->floor_slope_wall_id = wall_amount - 1;
	changer_prefab_events(editor->slope_ceiling_wall_changer,
		&editor->grid.modify_sector->ceiling_slope_wall_id, 1);
	if (editor->grid.modify_sector->ceiling_slope_wall_id >= wall_amount)
		editor->grid.modify_sector->ceiling_slope_wall_id = 0;
	else if (editor->grid.modify_sector->ceiling_slope_wall_id < 0)
		editor->grid.modify_sector->ceiling_slope_wall_id = wall_amount - 1;
}

void	floor_ceiling_slope_changer_prefab_events(t_editor *editor)
{
	changer_prefab_events(editor->slope_floor_angle_changer,
		&editor->grid.modify_sector->floor_slope, 1);
	if (editor->grid.modify_sector->floor_slope > 45)
		editor->grid.modify_sector->floor_slope = -45;
	else if (editor->grid.modify_sector->floor_slope < -45)
		editor->grid.modify_sector->floor_slope = 45;
	changer_prefab_events(editor->slope_ceiling_angle_changer,
		&editor->grid.modify_sector->ceiling_slope, 1);
	if (editor->grid.modify_sector->ceiling_slope > 45)
		editor->grid.modify_sector->ceiling_slope = -45;
	else if (editor->grid.modify_sector->ceiling_slope < -45)
		editor->grid.modify_sector->ceiling_slope = 45;
}
