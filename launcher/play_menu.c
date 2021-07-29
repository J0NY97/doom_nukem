/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:56:29 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 16:20:40 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

t_ui_menu_play	*play_menu_init(t_bui_element *parent_elem)
{
	t_ui_menu_play	*play_menu;
	t_xywh			c;

	play_menu = ft_memalloc(sizeof(t_ui_menu_play));
	play_menu->menu = new_main_menu_menu(parent_elem, "Play");
	// Play tab
	c.x = play_menu->menu->position.w / 8 / 2;
	c.y = play_menu->menu->position.w / 8 / 2;
	c.w = play_menu->menu->position.w - c.x * 2;
	c.h = play_menu->menu->position.h - c.y * 2;
	play_menu->play_tabs = bui_new_tab_preset(play_menu->menu, "tab", c);
	bui_set_element_color(play_menu->play_tabs->tabsystem, 0x00);
	// Endless
	endless_tab_init(play_menu);
	// Story
	story_tab_init(play_menu);
	return (play_menu);
}

void	launch_game(
		t_launcher *launcher, t_ui_menu_play *menu,
		t_ui_menu_settings *settings, char *text)
{
	char	*arr[10];

	arr[0] = ft_sprintf("%s", GAME_PATH"doom");
	arr[1] = ft_sprintf("%s%s", MAP_PATH, text);
	arr[2] = ft_sprintf("-size=%s", settings->resolution_drop->active->text);
	arr[3] = ft_sprintf("-res=%.2f",
			(float)settings->texture_scale_slider->value / 100);
	arr[4] = ft_sprintf("-mouse=%.3fx%.3f",
			(float)settings->mouse_x_slider->value / 1000,
			(float)settings->mouse_y_slider->value / 1000);
	if (ft_strequ(menu->difficulty_dropdown->active->text, "easy"))
		arr[5] = ft_sprintf("-diff=1");
	else if (ft_strequ(menu->difficulty_dropdown->active->text, "hard"))
		arr[5] = ft_sprintf("-diff=3");
	else
		arr[5] = ft_sprintf("-diff=2");
	arr[6] = ft_sprintf("-fov=%d", settings->fov_slider->value);
	arr[7] = ft_strdup("-launcher");
	if (settings->developer_mode_toggle->toggle)
		arr[8] = ft_strdup("-debug");
	else
		arr[8] = ft_strdup("");
	arr[9] = NULL;
	free_launcher(launcher);
	execv(arr[0], arr);
}

char	*check_map_clicked(t_ui_menu_play *menu)
{
	char	*text;

	text = NULL;
	if (menu->endless_view->show)
	{
		preset_dropdown_events(menu->difficulty_dropdown);
		text = get_clicked_map_button(menu->endless_maps);
		if (text)
			ft_stradd(&text, ".endless");
	}
	else if (menu->story_view->show)
	{
		text = get_clicked_map_button(menu->story_maps);
		if (text)
			ft_stradd(&text, ".story");
	}
	return (text);
}

void	play_menu_events(
		t_launcher *launcher, t_ui_menu_play *menu,
		t_ui_menu_settings *settings)
{
	char	*text;

	text = NULL;
	preset_tab_events(menu->play_tabs);
	if (menu->story_button->was_clicked_last_frame)
	{
		bui_set_element_color(menu->endless_button, 0xffb3c0a4);
		bui_set_element_color(menu->story_button, 0xff505168);
	}
	else if (menu->endless_button->was_clicked_last_frame)
	{
		bui_set_element_color(menu->endless_button, 0xff505168);
		bui_set_element_color(menu->story_button, 0xffb3c0a4);
	}
	text = check_map_clicked(menu);
	if (text)
		launch_game(launcher, menu, settings, text);
}
