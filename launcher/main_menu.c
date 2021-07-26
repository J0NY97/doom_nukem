/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 12:27:49 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/26 12:37:18 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

void	load_useless_stuff(t_launcher *launcher)
{
	launcher->mc_button
		= load_image(ICON_PATH"mc_button.png");
	launcher->mc_button_hover
		= load_image(ICON_PATH"mc_button_hover.png");
	launcher->mc_button_click
		= load_image(ICON_PATH"mc_button_click.png");
	launcher->mc_button_x3
		= load_image(ICON_PATH"mc_button_x3.png");
	launcher->mc_button_x3_hover
		= load_image(ICON_PATH"mc_button_x3_hover.png");
	launcher->mc_button_x3_click
		= load_image(ICON_PATH"mc_button_x3_click.png");
	launcher->gray_blue = 0xff505168;
	launcher->greenish = 0xffb3c0a4;
}

void	the_menu_init(t_launcher *launcher)
{
	// Settings
	launcher->settings_menu = settings_menu_init(launcher->menu);
	launcher->settings_menu->menu->show = 0;
	// Play menu
	launcher->play_menu = play_menu_init(launcher->menu);
	launcher->play_menu->menu->show = 0;
	// Editor menu
	launcher->editor_menu = editor_menu_init(launcher->menu);
	launcher->editor_menu->menu->show = 0;
	// Init the map buttons for needing menus.
	init_play_menu_map_buttons(launcher->play_menu, launcher->map_list);
	init_editor_menu_map_buttons(launcher->editor_menu, launcher->map_list);
}

t_launcher	*launcher_init(t_bui_window *win)
{
	t_launcher	*launcher;

	launcher = ft_memalloc(sizeof(t_launcher));
	// useless INIT
	load_useless_stuff(launcher);
	// Menu	
	launcher->menu = bui_new_menu(win, NULL,
			(t_xywh){0, 0, win->position.w, win->position.h});
	bui_set_element_image_from_path(launcher->menu, ELEMENT_DEFAULT,
		ICON_PATH"test2.bmp", NULL);
	// Getting map stuff
	launcher->map_dir = ft_strdup(ROOT_PATH"map_editor/maps");
	launcher->map_list = NULL;
	launcher->map_list = get_maps(launcher->map_dir);
	// Menu init
	the_menu_init(launcher);
	// button adding to list
	launcher->play_button = new_main_menu_button(launcher, "play", 0);
	launcher->editor_button = new_main_menu_button(launcher, "editor", 1);
	launcher->settings_button = new_main_menu_button(launcher, "settings", 2);
	launcher->quit_button = new_main_menu_button(launcher, "quit", 3);
	launcher->active_button = NULL;
	add_to_list(&launcher->main_menu_buttons,
		launcher->settings_button, sizeof(t_bui_element));
	add_to_list(&launcher->main_menu_buttons,
		launcher->play_button, sizeof(t_bui_element));
	add_to_list(&launcher->main_menu_buttons,
		launcher->editor_button, sizeof(t_bui_element));
	return (launcher);
}

void	main_menu_events(t_launcher *launcher, t_bui_libui *libui)
{
	launcher->settings_menu->menu->show = 0;
	launcher->play_menu->menu->show = 0;
	launcher->editor_menu->menu->show = 0;
	only_one_button_toggled_at_a_time(launcher->main_menu_buttons,
		&launcher->active_button);
	if (bui_button_toggle(launcher->settings_button))
	{
		launcher->settings_menu->menu->show = 1;
		settings_menu_events(launcher->settings_menu);
	}
	if (bui_button_toggle(launcher->play_button))
	{
		launcher->play_menu->menu->show = 1;
		play_menu_events(launcher->play_menu, launcher->settings_menu);
	}
	if (bui_button_toggle(launcher->editor_button))
	{
		launcher->editor_menu->menu->show = 1;
		editor_menu_events(launcher->editor_menu);
	}
	if (bui_button(launcher->quit_button))
		libui->run = 0;
}
