/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings_menu.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:57:08 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 16:11:49 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

void	settings_resolution_init(t_ui_menu_settings *settings, t_xywh c, int *i)
{
	t_bui_element	*menu;

	menu = settings->menu;
	settings->resolution_drop
		= bui_new_dropdown_preset(menu, NULL, get_next_pos(c, *i));
	*i += 1;
	settings->resolution_drop->drop->text_x = 5;
	bui_center_element_text_y(settings->resolution_drop->drop);
	bui_set_element_image_to_states(settings->resolution_drop->arrow,
		ICON_PATH"mc_button.png",
		ICON_PATH"mc_button_hover.png",
		ICON_PATH"mc_button_click.png");
	new_bass_element(settings->resolution_drop, "2560x1390");
	new_bass_element(settings->resolution_drop, "1920x1080");
	settings->resolution_drop->active
		= settings->resolution_drop->elements->content;
	new_bass_element(settings->resolution_drop, "1280x720");
	new_bass_element(settings->resolution_drop, "1024x768");
}

void	settings_developer_init(t_ui_menu_settings *settings, t_xywh c, int *i)
{
	t_bui_element	*menu;
	t_xywh			img_pos;

	menu = settings->menu;
	settings->developer_mode_toggle
		= bui_new_element(menu, NULL, get_next_pos(c, *i));
	img_pos = settings->developer_mode_toggle->position;
	img_pos.w = settings->developer_mode_toggle->position.h;
	img_pos.h = settings->developer_mode_toggle->position.h;
	bui_element_resize(settings->developer_mode_toggle, img_pos);
	bui_set_element_color(settings->developer_mode_toggle, 0x00);
	bui_set_element_image_to_states(settings->developer_mode_toggle,
		ICON_PATH"tick_box_off.png",
		ICON_PATH"tick_box_hover.png",
		ICON_PATH"tick_box_on.png");
	*i += 1;
}

t_ui_menu_settings	*settings_menu_init(t_bui_element *parent_elem)
{
	t_ui_menu_settings	*settings;
	t_xywh				c;
	int					i;

	settings = ft_memalloc(sizeof(t_ui_menu_settings));
	settings->menu = new_main_menu_menu(parent_elem, "Settings");
	c = new_xywh(0, 20, 100, 20);
	c.x = settings->menu->position.w / 8 / 2;
	c.y = settings->menu->position.w / 8 / 2;
	// Texts
	settings_text_init(settings, c);
	i = 0;
	c.x += c.w + 5;
	// Sliders
	settings_slider_init(settings, c, &i);
	// fps
	settings_developer_init(settings, c, &i);
	// resolution drop
	settings_resolution_init(settings, c, &i);
	return (settings);
}

void	settings_menu_events(t_ui_menu_settings *menu)
{
	preset_slider_events(menu->fov_slider);
	preset_slider_events(menu->mouse_x_slider);
	preset_slider_events(menu->mouse_y_slider);
	bui_button_toggle(menu->developer_mode_toggle);
	preset_slider_events(menu->texture_scale_slider);
	preset_dropdown_events(menu->resolution_drop);
}
