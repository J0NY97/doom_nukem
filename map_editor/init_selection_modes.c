/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_selection_modes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:19:53 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 13:19:54 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_bui_element	*new_selection_mode_button(
		t_bui_element *parent, char *image_path, int i)
{
	t_bui_element	*button;
	t_xywh			coord;
	int				w;
	int				gap;

	w = 32;
	gap = 10;
	coord = new_xywh((i * (w + gap)) + gap, 20, w, w);
	button = bui_new_element(parent, NULL, coord);
	bui_set_element_image_from_path(button, ELEMENT_ALL, image_path, NULL);
	bui_set_element_state_border(button, 2, 0xff0080ff, ELEMENT_HOVER);
	bui_set_element_state_border(button, 2, 0xff0080ff, ELEMENT_CLICK);
	return (button);
}

void	selection_mode_button_init(t_editor *editor)
{
	editor->select_mode_buttons = NULL;
	editor->active_select_mode = NULL;
	editor->button_draw = new_selection_mode_button(editor->select_mode,
			ICON_PATH"draw_mode.png", 0);
	editor->select_mode_vertex = new_selection_mode_button(editor->select_mode,
			ICON_PATH"selection_mode_vertex.png", 1);
	editor->select_mode_wall = new_selection_mode_button(editor->select_mode,
			ICON_PATH"selection_mode_wall.png", 2);
	editor->select_mode_sector = new_selection_mode_button(editor->select_mode,
			ICON_PATH"selection_mode_sector.png", 3);
	editor->select_mode_entity = new_selection_mode_button(editor->select_mode,
			ICON_PATH"selection_mode_entity.png", 4);
	add_to_list(&editor->select_mode_buttons, editor->button_draw,
		sizeof(t_bui_element));
	add_to_list(&editor->select_mode_buttons, editor->select_mode_vertex,
		sizeof(t_bui_element));
	add_to_list(&editor->select_mode_buttons, editor->select_mode_wall,
		sizeof(t_bui_element));
	add_to_list(&editor->select_mode_buttons, editor->select_mode_sector,
		sizeof(t_bui_element));
	add_to_list(&editor->select_mode_buttons, editor->select_mode_entity,
		sizeof(t_bui_element));
}
