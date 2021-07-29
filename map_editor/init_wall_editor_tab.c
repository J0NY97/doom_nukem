/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_wall_editor_tab.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 14:15:46 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 14:16:04 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_bui_element	*add_new_wall_texture_tab(
		t_preset_tab *tab, char *text, Uint32 color)
{
	t_bui_element	**elems;
	t_bui_element	*view;

	elems = preset_tab_add(tab, text);
	bui_set_element_color(elems[0], color);
	bui_set_element_color(elems[1], color);
	view = elems[1];
	view->update_state = 0;
	free(elems);
	return (view);
}

void	wall_tab_init(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(5, 20, 80, 40);
	editor->texture_scale_changer
		= new_changer_prefab(editor->wall_texture_view, "texture scale", coord);
	bui_set_element_color(editor->texture_scale_changer->menu, 0xff06D6A0);
	bui_set_element_color(editor->texture_scale_changer->sub_button,
		0xff06D6A0);
	bui_set_element_color(editor->texture_scale_changer->value, 0xff06D6A0);
	bui_set_element_color(editor->texture_scale_changer->add_button,
		0xff06D6A0);
	coord = new_xywh(115, 20, 100, 20);
	editor->wall_solid = bui_new_element(editor->wall_texture_view,
			"Solid:", coord);
	bui_set_element_color(editor->wall_solid,
		((t_bui_element *)editor->wall_solid->parent)->color);
	coord = new_xywh(40, 0, 20, 20);
	editor->wall_solid_tick = bui_new_element(editor->wall_solid, NULL, coord);
	bui_set_element_image_to_states(editor->wall_solid_tick,
		ICON_PATH"tick_box_off.png",
		ICON_PATH"tick_box_hover.png",
		ICON_PATH"tick_box_on.png");
}

void	portal_tab_init(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(115, 40, 100, 20);
	editor->wall_portal = bui_new_element(editor->wall_texture_view,
			"Portal:", coord);
	bui_set_element_color(editor->wall_portal,
		((t_bui_element *)editor->wall_portal->parent)->color);
	coord = new_xywh(40, 0, 20, 20);
	editor->wall_portal_tick = bui_new_element(editor->wall_portal,
			NULL, coord);
	bui_set_element_image_to_states(editor->wall_portal_tick,
		ICON_PATH"tick_box_off.png",
		ICON_PATH"tick_box_hover.png",
		ICON_PATH"tick_box_on.png");
}

void	wall_sprite_tab_init(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(95, 20, 80, 20);
	editor->add_wall_sprite_button = bui_new_element(editor->wall_sprite_view,
			"add sprite", coord);
	bui_set_element_color(editor->add_wall_sprite_button, 0xff06D6A0);
	coord = new_xywh(95, 40, 80, 20);
	editor->remove_wall_sprite_button
		= bui_new_element(editor->wall_sprite_view, "remove sprite", coord);
	bui_set_element_color(editor->remove_wall_sprite_button, 0xff06D6A0);
	coord = new_xywh(5, 20, 80, 40);
	editor->sprite_scale_changer = new_changer_prefab(editor->wall_sprite_view,
			"sprite scale", coord);
	bui_set_element_color(editor->sprite_scale_changer->menu, 0xff06D6A0);
	bui_set_element_color(editor->sprite_scale_changer->sub_button, 0xff06D6A0);
	bui_set_element_color(editor->sprite_scale_changer->value, 0xff06D6A0);
	bui_set_element_color(editor->sprite_scale_changer->add_button, 0xff06D6A0);
	coord = new_xywh(0, 0, 50, 20);
	editor->sprite_changer = new_changer_prefab(editor->wall_sprite_view,
			"Wall Sprite ID", coord);
	editor->selected_sprite = 0;
}

void	tabsystem_wall_editor_init(t_editor *editor)
{
	t_xywh			coord;

	coord = new_xywh(5, 20,
			editor->edit_toolbox_wall->position.w - 10,
			editor->edit_toolbox_wall->position.h - 25);
	editor->wall_tab = bui_new_tab_preset(editor->edit_toolbox_wall,
			"texture tabs", coord);
	bui_set_element_color(editor->wall_tab->tabsystem,
		((t_bui_element *)editor->wall_tab->tabsystem->parent)->color);
	editor->wall_tab->tabsystem->update_state = 0;
	editor->wall_texture_view = add_new_wall_texture_tab(editor->wall_tab,
			"Wall Texture", editor->palette.light_blue);
	editor->portal_texture_view = add_new_wall_texture_tab(editor->wall_tab,
			"Portal Texture", editor->palette.granny_smith_apple);
	editor->wall_sprite_view = add_new_wall_texture_tab(editor->wall_tab,
			"Wall Sprite", editor->palette.peach_crayola);
	wall_tab_init(editor);
	portal_tab_init(editor);
	wall_sprite_tab_init(editor);
}
