/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 15:31:55 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	wall_option(t_editor *editor)
{
	editor->edit_view_wall->show = 1;
	editor->edit_toolbox_wall->show = 1;
	preset_tab_events(editor->wall_tab);
	if (editor->wall_texture_view->show == 1)
		wall_texture_view_events(editor);
	else if (editor->portal_texture_view->show == 1)
		portal_texture_view_events(editor);
	else if (editor->wall_sprite_view->show == 1)
		wall_sprite_view_events(editor);
	wall_render(editor);
}

void	entity_option(t_editor *editor)
{
	t_list	*curr;

	editor->edit_toolbox_entity->show = 1;
	editor->edit_view_entity->show = 1;
	entity_drop_down_event(editor);
	curr = editor->entity_direction_radio_buttons;
	while (curr)
	{
		if (ft_atoi(((t_bui_element *)curr->content)->text)
			== editor->grid.modify_entity->direction)
			editor->active_direction_button = curr->content;
		curr = curr->next;
	}
	only_one_button_toggled_at_a_time(
		editor->entity_direction_radio_buttons,
		&editor->active_direction_button);
	editor->grid.modify_entity->direction
		= ft_atoi(editor->active_direction_button->text);
	draw_selected_entity_texture(editor);
}

void	sector_option(t_editor *editor)
{
	editor->sector_texture_menu->show = 1;
	editor->edit_toolbox_sector->show = 1;
	editor->slope_edit_menu->show = 1;
	sector_f_and_c_button_events(editor);
	draw_selected_f_and_c_button(editor);
	sector_changer_prefab_events(editor);
	floor_ceiling_id_changer_prefab_events(editor);
	draw_sector_viewer(editor, editor->slope_sector->active_surface);
	floor_ceiling_slope_changer_prefab_events(editor);
}

void	selected_option_menu(t_editor *doom, t_grid *grid)
{
	doom->sector_texture_menu->show = 0;
	doom->edit_toolbox_sector->show = 0;
	doom->slope_edit_menu->show = 0;
	doom->edit_view_wall->show = 0;
	doom->edit_toolbox_wall->show = 0;
	doom->edit_toolbox_entity->show = 0;
	doom->edit_view_entity->show = 0;
	if (grid->modify_wall != NULL)
		wall_option(doom);
	else if (grid->modify_sector != NULL)
		sector_option(doom);
	else if (grid->modify_entity != NULL)
		entity_option(doom);
}
