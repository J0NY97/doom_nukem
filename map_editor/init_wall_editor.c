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

void	new_wall_texture_button(
		t_bui_element *parent, t_list **list, SDL_Surface *texture, int i)
{
	t_bui_element	*temp_elem;
	t_xywh			coord;
	char			*str;
	int				button_gap;
	int				amount_on_x;

	button_gap = 15;
	amount_on_x = floor(parent->position.w / (50 + button_gap + 5));
	coord = new_xywh(0, 0, 50, 50);
	coord.x = (i % (amount_on_x + 1)) * (coord.w + button_gap) + 5;
	coord.y = (i / (amount_on_x + 1)) * (coord.h + button_gap) + 70;
	str = ft_itoa(i);
	temp_elem = bui_new_element(parent, str, coord);
	ft_strdel(&str);
	SDL_BlitScaled(texture, NULL, temp_elem->surface[0], NULL);
	SDL_BlitScaled(texture, NULL, temp_elem->surface[1], NULL);
	SDL_BlitScaled(texture, NULL, temp_elem->surface[2], NULL);
	draw_rect_border(temp_elem->surface[ELEMENT_CLICK],
		(t_xywh){0, 0, temp_elem->position.w, temp_elem->position.h},
		0xff00ff00, 5);
	draw_rect_border(temp_elem->surface[ELEMENT_HOVER],
		(t_xywh){0, 0, temp_elem->position.w, temp_elem->position.h},
		0xff0000ff, 5);
	add_to_list(list, temp_elem, sizeof(t_bui_element));
}

void	portal_texture_tab_init(t_editor *editor)
{
	t_bui_element	**elems;

	elems = preset_tab_add(editor->wall_tab, "Wall Texture");
	bui_set_element_color(elems[0], editor->palette.light_blue);
	bui_set_element_color(elems[1], editor->palette.light_blue);
	editor->wall_texture_view = elems[1];
	bui_set_element_flags(editor->wall_texture_view,
		BUI_ELEMENT_DONT_UPDATE_STATE);
	free(elems);
}

void	wall_texture_tab_init(t_editor *editor)
{
	t_bui_element	**elems;

	elems = preset_tab_add(editor->wall_tab, "Portal Texture");
	bui_set_element_color(elems[0], editor->palette.granny_smith_apple);
	bui_set_element_color(elems[1], editor->palette.granny_smith_apple);
	editor->portal_texture_view = elems[1];
	bui_set_element_flags(editor->portal_texture_view,
		BUI_ELEMENT_DONT_UPDATE_STATE);
	free(elems);
}

void	wall_sprite_texture_tab_init(t_editor *editor)
{
	t_bui_element	**elems;

	elems = preset_tab_add(editor->wall_tab, "Wall Sprite");
	bui_set_element_color(elems[0], editor->palette.peach_crayola);
	bui_set_element_color(elems[1], editor->palette.peach_crayola);
	editor->wall_sprite_view = elems[1];
	bui_set_element_flags(editor->wall_sprite_view,
		BUI_ELEMENT_DONT_UPDATE_STATE);
	free(elems);
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
	portal_texture_tab_init(editor);
	wall_texture_tab_init(editor);
	wall_sprite_texture_tab_init(editor);
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

void	wall_texture_buttons_init(t_editor *editor)
{
	int	i;

	editor->wall_texture_buttons = NULL;
	editor->active_wall_texture = NULL;
	i = 0;
	while (i < editor->texture_amount)
	{
		new_wall_texture_button(editor->wall_texture_view,
			&editor->wall_texture_buttons, editor->texture_textures[i], i);
		i++;
	}
}

void	portal_texture_buttons_init(t_editor *editor)
{
	int	i;

	editor->portal_texture_buttons = NULL;
	editor->active_portal_texture = NULL;
	i = 0;
	while (i < editor->texture_amount)
	{
		new_wall_texture_button(editor->portal_texture_view,
			&editor->portal_texture_buttons, editor->texture_textures[i], i);
		i++;
	}
}

void	wall_sprite_texture_buttons_init(t_editor *editor)
{
	int	i;

	editor->wall_sprite_buttons = NULL;
	editor->active_wall_sprite = NULL;
	i = 0;
	while (i < editor->texture_amount)
	{
		new_wall_texture_button(editor->wall_sprite_view,
			&editor->wall_sprite_buttons, editor->texture_textures[i], i);
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
	wall_tab_init(editor);
	wall_texture_buttons_init(editor);
	portal_tab_init(editor);
	portal_texture_buttons_init(editor);
	wall_sprite_tab_init(editor);
	wall_sprite_texture_buttons_init(editor);
}
