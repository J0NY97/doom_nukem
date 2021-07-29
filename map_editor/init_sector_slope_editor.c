/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sector_slope_editor.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:24:11 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 13:24:12 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	sector_slope_floor_changer_init(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(10, editor->slope_sector->position.y
			+ editor->slope_sector->position.h + 10, 100, 20);
	editor->slope_floor_title = bui_new_element(editor->slope_edit_menu,
			"Floor", coord);
	bui_set_element_color(editor->slope_floor_title, 0xff0000ff);
	coord = new_xywh(editor->slope_floor_title->position.x,
			editor->slope_floor_title->position.y
			+ editor->slope_floor_title->position.h + 10, 100, 40);
	editor->slope_floor_wall_changer
		= new_changer_prefab(editor->slope_edit_menu, "Wall ID", coord);
	bui_set_element_text(editor->slope_floor_wall_changer->sub_button, "<");
	bui_set_element_text(editor->slope_floor_wall_changer->add_button, ">");
	coord = new_xywh(editor->slope_floor_wall_changer->menu->position.x
			+ editor->slope_floor_wall_changer->menu->position.w + 10,
			editor->slope_floor_wall_changer->menu->position.y, 100, 40);
	editor->slope_floor_angle_changer
		= new_changer_prefab(editor->slope_edit_menu, "Slope Angle", coord);
}

void	sector_slope_ceiling_changer_init(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(10,
			editor->slope_floor_wall_changer->menu->position.y
			+ editor->slope_floor_wall_changer->menu->position.h + 10, 100, 20);
	editor->slope_ceiling_title = bui_new_element(editor->slope_edit_menu,
			"Ceiling", coord);
	bui_set_element_color(editor->slope_ceiling_title, 0xff00ff00);
	coord = new_xywh(editor->slope_ceiling_title->position.x,
			editor->slope_ceiling_title->position.y
			+ editor->slope_ceiling_title->position.h + 10, 100, 40);
	editor->slope_ceiling_wall_changer
		= new_changer_prefab(editor->slope_edit_menu, "Wall ID", coord);
	bui_set_element_text(editor->slope_ceiling_wall_changer->sub_button,
		"<");
	bui_set_element_text(editor->slope_ceiling_wall_changer->add_button,
		">");
	coord = new_xywh(editor->slope_ceiling_wall_changer->menu->position.x
			+ editor->slope_ceiling_wall_changer->menu->position.w + 10,
			editor->slope_ceiling_wall_changer->menu->position.y, 100, 40);
	editor->slope_ceiling_angle_changer
		= new_changer_prefab(editor->slope_edit_menu, "Slope Angle", coord);
}

void	sector_slope_edit_menu_init(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(editor->sector_texture_menu->position.x
			+ editor->sector_texture_menu->position.w + 5, 5,
			editor->edit_window->position.w
			- editor->sector_texture_menu->position.w
			- editor->edit_toolbox_sector->position.w - 20,
			editor->edit_window->position.h - 10);
	editor->slope_edit_menu = bui_new_menu(editor->edit_window,
			"Slope Edit", coord);
	bui_set_element_color(editor->slope_edit_menu,
		editor->palette.elem_elem_elem);
	coord = new_xywh(10, 20, editor->slope_edit_menu->position.w - 20,
			editor->slope_edit_menu->position.h * 0.5);
	editor->slope_sector = bui_new_element(editor->slope_edit_menu,
			"Sector", coord);
	bui_set_element_flags(editor->slope_sector, BUI_ELEMENT_DONT_UPDATE_STATE);
	sector_slope_floor_changer_init(editor);
	sector_slope_ceiling_changer_init(editor);
}
