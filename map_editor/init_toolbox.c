/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_toolbox.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:19:57 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 13:19:58 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	button_init_info_area(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(editor->info_area->position.w - 110, 25, 32, 32);
	editor->button_remove = bui_new_element(editor->info_area, NULL, coord);
	bui_set_element_image_to_states(editor->button_remove,
		ICON_PATH"remove_button.png",
		ICON_PATH"remove_button_click.png",
		ICON_PATH"remove_button_click.png");
	coord = new_xywh(editor->info_area->position.w
			- 110 + 32 + 10, 25, 32, 32);
	editor->button_edit = bui_new_element(editor->info_area, NULL, coord);
	bui_set_element_image_to_states(editor->button_edit,
		ICON_PATH"edit_button.png",
		ICON_PATH"edit_button_click.png",
		ICON_PATH"edit_button_click.png");
}

t_bui_element	*new_info(t_bui_element *parent, char *text, t_xywh c)
{
	t_bui_element	*elem;

	elem = bui_new_element(parent, text, c);
	bui_set_element_color(elem, parent->color);
	elem->text_color = 0xffffffff;
	return (elem);
}

void	info_area_init(t_editor *editor, t_xywh c)
{
	t_xywh	coord;

	editor->info_area = bui_new_element(editor->toolbox, NULL, c);
	editor->info_area->update_state = 0;
	bui_set_element_color(editor->info_area, editor->palette.elem_elem);
	coord = new_xywh(10, 20, 100, 50);
	editor->hover_info = new_info(editor->info_area, NULL, coord);
	coord = new_xywh(10, 75, 100, 50);
	editor->selected_sector_info = new_info(editor->info_area, NULL, coord);
	coord = new_xywh(10, 130, 100, 55);
	editor->selected_vector_info = new_info(editor->info_area, NULL, coord);
	coord = new_xywh(editor->info_area->position.w - 110, 100, 100, 40);
	editor->scaler = new_changer_prefab(editor->info_area, "Map Scale", coord);
	coord = new_xywh(10, editor->selected_vector_info->position.y
			+ editor->selected_vector_info->position.h + 5,
			editor->info_area->position.w - 20, 40);
	editor->general_info = new_info(editor->info_area, NULL, coord);
	coord = new_xywh(10, editor->info_area->position.h - 40 - 10,
			editor->info_area->position.w - 20, 40);
	editor->info_box = new_info(editor->info_area, NULL, coord);
	editor->info_box->update_state = 0;
	bui_set_element_border(editor->info_box, 1, editor->palette.light_gray);
	button_init_info_area(editor);
}

void	selection_mode_init(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(10, 25, editor->toolbox->position.w - 20, 55);
	editor->select_mode = bui_new_element(editor->toolbox, "Mode", coord);
	editor->select_mode->update_state = 0;
	bui_set_element_text_font(editor->select_mode, TTF_PATH"DroidSans.ttf",
		editor->select_mode->font_size, 0xffffffff);
	bui_set_element_color(editor->select_mode, editor->palette.elem_elem);
	selection_mode_button_init(editor);
}

void	toolbox_init(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(10, 10, editor->window->active_surface->w / 7,
			editor->window->active_surface->h - 20);
	editor->toolbox = bui_new_menu(editor->window, "toolbox", coord);
	editor->toolbox->update_state = 0;
	bui_set_element_color(editor->toolbox, editor->palette.win_elem);
	selection_mode_init(editor);
	button_init(editor);
	coord = new_xywh(10, editor->other_mode->position.y
			+ editor->other_mode->position.h + 10, editor->toolbox->position.w
			- 20, 300);
	info_area_init(editor, coord);
}
