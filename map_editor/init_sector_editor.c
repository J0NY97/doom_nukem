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

	button_gap = 15;
	amount_on_x = floor(parent->position.w / (70 + button_gap + 5));
	coord = new_xywh(0, 0, 50, 50);
	coord.x = (i % (amount_on_x + 1)) * (coord.w + button_gap) + 5;
	coord.y = (i / (amount_on_x + 1)) * (coord.h + button_gap) + 70;
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

void	sector_texture_buttons_init(t_editor *editor)
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
	sector_texture_buttons_init(editor);
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
