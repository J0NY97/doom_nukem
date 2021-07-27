/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_preset_console.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 14:05:22 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 14:07:10 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

t_preset_console	*bui_new_console_preset(t_bui_window *win, t_xywh pos)
{
	t_preset_console	*console;
	t_bui_element		*menu;
	t_xywh				view_pos;

	console = (t_preset_console *)ft_memalloc(sizeof(t_preset_console));
	console->menu = bui_new_menu_preset(win, "Console", pos);
	menu = console->menu->menu;
	console->carrot = bui_new_element(menu, " >:", (t_xywh){0,
			menu->position.h - 20, 20, 20});
	console->send = bui_new_element(menu, "send", (t_xywh){
			menu->position.w - 50, menu->position.h - 20, 50, 20});
	console->input = bui_new_element(menu, " ", (t_xywh){
			console->carrot->position.w, console->carrot->position.y,
			menu->position.w - (console->send->position.w
				+ console->carrot->position.w), console->carrot->position.h});
	view_pos.x = 1;
	view_pos.y = console->menu->title_bar->position.y
		+ console->menu->title_bar->position.h + 1;
	view_pos.w = menu->position.w - (view_pos.x * 2);
	view_pos.h = menu->position.h - view_pos.y - console->input->position.h - 1;
	console->view = bui_new_element(menu, "", view_pos);
	console_log(console, "4 8 15 16 23 42");
	return (console);
}

void	preset_console_events(t_preset_console *console)
{
	t_bui_libui	*libui;

	libui = console->menu->menu->libui;
	preset_menu_events(console->menu);
	if (bui_button(console->send)
		|| (console->input->toggle && key_pressed(libui, SDL_SCANCODE_RETURN)))
	{
		console_log(console, console->input->text);
		if (console->latest_command)
			ft_strdel(&console->latest_command);
		console->latest_command = ft_strdup(console->input->text);
		bui_change_element_text(console->input, " ");
	}
	bui_input(console->input);
	if (libui->mouse_wheel_y != 0)
		console->view->text_y += libui->mouse_wheel_y
			* TTF_FontLineSkip(console->view->font);
}

void	console_log(t_preset_console *console, char *text)
{
	char	*str;

	str = ft_sprintf("%s>: %s\n", console->view->text, text);
	bui_change_element_text(console->view, str);
	ft_strdel(&str);
	console->lines++;
	console->view->text_y = console->view->position.h
		- (console->lines * TTF_FontLineSkip(console->view->font));
}
