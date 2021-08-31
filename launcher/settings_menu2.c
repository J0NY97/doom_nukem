/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings_menu2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 16:39:17 by jsalmi            #+#    #+#             */
/*   Updated: 2021/08/31 14:53:23 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

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

void	settings_slider_init(t_ui_menu_settings *settings, t_xywh c, int *i)
{
	t_bui_element	*menu;

	menu = settings->menu;
	settings->fov_slider = new_setting_slider(menu, get_next_pos(c, *i));
	bui_set_slider_values(settings->fov_slider, 90, 50, 100);
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
