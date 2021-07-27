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

t_bui_element	*new_setting_text(t_bui_element *parent, char *text, t_xywh c)
{
	t_bui_element	*elem;

	elem = bui_new_element(parent, text, c);
	elem->update_state = 0;
	bui_set_element_image_from_path(elem, ELEMENT_DEFAULT,
		ICON_PATH"mc_button_x3.png", NULL);
	elem->text_x = 5;
	bui_center_element_text_y(elem);
	return (elem);
}

t_preset_slider	*new_setting_slider(t_bui_element *parent, t_xywh pos)
{
	t_preset_slider	*slider;
	t_xywh			t;

	slider = bui_new_slider_preset(parent, pos);
	t = slider->button->position;
	t.w += 2;
	bui_element_resize(slider->button, t);
	bui_set_element_color(slider->slider, 0xffDCC48E);
	bui_set_element_image_from_path(slider->slider, ELEMENT_ALL,
		ICON_PATH"slider.png", NULL);
	return (slider);
}

void	settings_text_init(t_ui_menu_settings *settings, t_xywh c)
{
	int				i;
	t_bui_element	*menu;

	i = 0;
	menu = settings->menu;
	settings->fov = new_setting_text(menu, "FOV", get_next_pos(c, i++));
	settings->mouse_x = new_setting_text(menu, "Mouse_x", get_next_pos(c, i++));
	settings->mouse_y = new_setting_text(menu, "Mouse_y", get_next_pos(c, i++));
	settings->texture_scale
		= new_setting_text(menu, "Texture Scale", get_next_pos(c, i++));
	settings->developer_mode
		= new_setting_text(menu, "Developer", get_next_pos(c, i++));
	settings->resolution
		= new_setting_text(menu, "Resolution", get_next_pos(c, i++));
}

void	settings_slider_init(t_ui_menu_settings *settings, t_xywh c, int *i)
{
	t_bui_element	*menu;

	menu = settings->menu;
	settings->fov_slider = new_setting_slider(menu, get_next_pos(c, *i));
	bui_set_slider_values(settings->fov_slider, 75, 45, 120);
	*i += 1;
	settings->mouse_x_slider = new_setting_slider(menu, get_next_pos(c, *i));
	bui_set_slider_values(settings->mouse_x_slider, 5, 1, 10);
	*i += 1;
	settings->mouse_y_slider = new_setting_slider(menu, get_next_pos(c, *i));
	bui_set_slider_values(settings->mouse_y_slider, 5, 1, 10);
	*i += 1;
	settings->texture_scale_slider
		= new_setting_slider(menu, get_next_pos(c, *i));
	bui_set_slider_values(settings->texture_scale_slider, 80, 30, 100);
	*i += 1;
}

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
