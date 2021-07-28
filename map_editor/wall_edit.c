/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_edit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:42:19 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:42:40 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	wall_texture_button_events(t_editor *editor)
{
	char	*temp;

	temp = ft_itoa(editor->grid.modify_wall->texture_id);
	if (only_one_button_toggled_at_a_time(editor->wall_texture_buttons,
			&editor->active_wall_texture))
	{
	}
	else
		toggle_on_element_with_text(editor->wall_texture_buttons,
			&editor->active_wall_texture, temp);
	ft_strdel(&temp);
	if (editor->active_wall_texture != NULL)
		editor->grid.modify_wall->texture_id
			= ft_atoi(editor->active_wall_texture->text);
}

void	wall_texture_view_events(t_editor *editor)
{
	changer_prefab_events_float(editor->texture_scale_changer,
		&editor->grid.modify_wall->texture_scale, 0.1f);
	editor->grid.modify_wall->texture_scale
		= ft_fclamp(editor->grid.modify_wall->texture_scale, 0.1f, 64.0f);
	editor->wall_solid_tick->toggle = editor->grid.modify_wall->solid;
	if (bui_button(editor->wall_solid_tick))
	{
		if (editor->wall_solid_tick->toggle == 1)
			editor->grid.modify_wall->solid = 0;
		else
			editor->grid.modify_wall->solid = 1;
	}
	if (editor->grid.modify_wall->neighbor_sector != NULL)
		editor->wall_portal_tick->toggle = 1;
	else
		editor->wall_portal_tick->toggle = 0;
	if (bui_button(editor->wall_portal_tick))
	{
		if (editor->wall_portal_tick->toggle == 1)
			remove_portal(&editor->grid);
		else
			add_portal(&editor->grid);
	}
	wall_texture_button_events(editor);
}

void	portal_texture_view_events(t_editor *editor)
{
	char	*temp;

	temp = ft_itoa(editor->grid.modify_wall->portal_texture_id);
	if (only_one_button_toggled_at_a_time(editor->portal_texture_buttons,
			&editor->active_portal_texture))
	{
	}
	else
		toggle_on_element_with_text(editor->portal_texture_buttons,
			&editor->active_portal_texture, temp);
	ft_strdel(&temp);
	if (editor->active_portal_texture != NULL)
		editor->grid.modify_wall->portal_texture_id
			= ft_atoi(editor->active_portal_texture->text);
}
