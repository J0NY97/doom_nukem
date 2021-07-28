/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sector_editor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:19:51 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 13:19:52 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	new_texture_button(
			t_bui_element *parent, t_list **list, SDL_Surface *texture, int i)
{
	t_bui_element	*temp_elem;
	t_xywh			coord;
	int				button_gap;
	int				amount_on_x;
	char			*str;

	button_gap = 20;
	amount_on_x = floor(parent->position.w / (50 + button_gap + 20));
	coord = new_xywh(0, 0, 50, 50);
	coord.x = (i % (amount_on_x + 1)) * (coord.w + button_gap) + 20;
	coord.y = (i / (amount_on_x + 1)) * (coord.h + button_gap) + 50;
	str = ft_sprintf("%d", i);
	temp_elem = bui_new_element(parent, str, coord);
	ft_strdel(&str);
	SDL_BlitScaled(texture, NULL, temp_elem->surface[0], NULL);
	SDL_BlitScaled(texture, NULL, temp_elem->surface[1], NULL);
	SDL_BlitScaled(texture, NULL, temp_elem->surface[2], NULL);
	bui_set_element_state_border(temp_elem, 2, 0xff00ff00, ELEMENT_HOVER);
	bui_set_element_state_border(temp_elem, 2, 0xff00ffff, ELEMENT_CLICK);
	add_to_list(list, temp_elem, sizeof(t_bui_element));
}

void	texture_buttons_init(t_editor *editor)
{
	int	i;

	i = 0;
	editor->sector_texture_buttons = NULL;
	while (i < editor->texture_amount)
	{
		new_texture_button(editor->sector_texture_menu,
			&editor->sector_texture_buttons,
			editor->texture_textures[i], i);
		i++;
	}
}

void	floor_and_ceiling_texture_menu_init(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(editor->edit_toolbox_sector->position.x
			+ editor->edit_toolbox_sector->position.w + 5, 5,
			(editor->edit_window->position.w
				- editor->edit_toolbox_sector->position.w - 15) * 0.5,
			editor->edit_window->position.h - 10);
	editor->sector_texture_menu = bui_new_menu(editor->edit_window,
			"Sector Texture Menu", coord);
	bui_set_element_color(editor->sector_texture_menu,
		editor->palette.elem_elem_elem);
	texture_buttons_init(editor);
	coord = new_xywh(editor->sector_texture_menu->position.w - 100,
			0, 50, 20);
	editor->floor_texture_title
		= bui_new_element(editor->sector_texture_menu, "Floor", coord);
	bui_set_element_color(editor->floor_texture_title, 0xff0000ff);
	coord = new_xywh(editor->sector_texture_menu->position.w - 50,
			0, 50, 20);
	editor->ceiling_texture_title
		= bui_new_element(editor->sector_texture_menu, "Ceiling", coord);
	bui_set_element_color(editor->ceiling_texture_title, 0xff00ff00);
}

void	sector_edit_changer_inits(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(5, (25 * 1) + (40 * 0), 100, 40);
	editor->floor_height = new_changer_prefab(editor->edit_toolbox_sector,
			"floor height", coord);
	coord.y = (25 * 2) + (40 * 1);
	editor->ceiling_height = new_changer_prefab(editor->edit_toolbox_sector,
			"ceiling height", coord);
	coord.y = (25 * 3) + (40 * 2);
	editor->gravity = new_changer_prefab(editor->edit_toolbox_sector,
			"gravity", coord);
	coord.y = (25 * 4) + (40 * 3);
	editor->lighting = new_changer_prefab(editor->edit_toolbox_sector,
			"lighting", coord);
	coord.y = (25 * 6) + (40 * 5);
	editor->floor_scale = new_changer_prefab(editor->edit_toolbox_sector,
			"floor texture scale", coord);
	coord.y = (25 * 7) + (40 * 6);
	editor->ceiling_scale = new_changer_prefab(editor->edit_toolbox_sector,
			"ceiling texture scale", coord);
}

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

void	init_sector_editor(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(5, 5, editor->edit_window->position.w * 0.20f,
			editor->edit_window->position.h - 10);
	editor->edit_toolbox_sector = bui_new_menu(editor->edit_window,
			"New Toolbox", coord);
	bui_set_element_color(editor->edit_toolbox_sector,
		editor->palette.elem_elem_elem);
	floor_and_ceiling_texture_menu_init(editor);
	sector_edit_changer_inits(editor);
	sector_slope_edit_menu_init(editor);
}
