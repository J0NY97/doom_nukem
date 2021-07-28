/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_wall_editor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:19:59 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 13:20:00 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	texture_buttons_init(t_editor *editor)
{
	int	i;

	editor->wall_texture_buttons = NULL;
	editor->active_wall_texture = NULL;
	editor->wall_sprite_buttons = NULL;
	editor->active_wall_sprite = NULL;
	editor->portal_texture_buttons = NULL;
	editor->active_portal_texture = NULL;
	i = 0;
	while (i < editor->texture_amount)
	{
		new_texture_button(editor->wall_texture_view,
			&editor->wall_texture_buttons, editor->texture_textures[i], i);
		new_texture_button(editor->wall_sprite_view,
			&editor->wall_sprite_buttons, editor->texture_textures[i], i);
		new_texture_button(editor->portal_texture_view,
			&editor->portal_texture_buttons, editor->texture_textures[i], i);
		i++;
	}
}

void	init_wall_editor(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(5, 5, editor->edit_window->position.w * 0.20f,
			editor->edit_window->position.h - 10);
	editor->edit_toolbox_wall = bui_new_menu(editor->edit_window,
			"New Toolbox", coord);
	coord = new_xywh(editor->edit_toolbox_sector->position.x
			+ editor->edit_toolbox_sector->position.w + 5, 5,
			editor->edit_window->position.w
			- editor->edit_toolbox_sector->position.w - 15,
			editor->edit_window->position.h - 10);
	editor->edit_view_wall = bui_new_menu(editor->edit_window,
			"New View", coord);
	editor->edit_view_wall->update = 0;
	tabsystem_wall_editor_init(editor);
	texture_buttons_init(editor);
}
