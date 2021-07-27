/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_render0.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 16:23:13 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 16:23:56 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

void	mouse_update(t_bui_libui *libui, SDL_Event e)
{
	if (e.button.type == SDL_MOUSEBUTTONDOWN)
	{
		libui->mouse_down = 1;
		libui->mouse_down_last_frame = 1;
		libui->mouse_click_x = libui->mouse_x;
		libui->mouse_click_y = libui->mouse_y;
		libui->mouse_buttons[e.button.button] = 1;
	}
	if (e.button.type == SDL_MOUSEBUTTONUP)
	{
		libui->mouse_down = 0;
		libui->mouse_buttons[e.button.button] = 0;
	}
	else if (e.type == SDL_MOUSEWHEEL)
	{
		libui->mouse_wheel_x = e.wheel.x;
		libui->mouse_wheel_y = e.wheel.y;
	}
}

void	get_mouse_states(t_bui_libui *libui)
{
	int	rx;
	int	ry;

	libui->last_global_mouse_x = libui->global_mouse_x;
	libui->last_global_mouse_y = libui->global_mouse_y;
	SDL_GetGlobalMouseState(&libui->global_mouse_x, &libui->global_mouse_y);
	libui->last_mouse_x = libui->mouse_x;
	libui->last_mouse_y = libui->mouse_y;
	SDL_GetMouseState(&libui->mouse_x, &libui->mouse_y);
	libui->last_relative_mouse_x = libui->relative_mouse_x;
	libui->last_relative_mouse_y = libui->relative_mouse_y;
	SDL_GetRelativeMouseState(&rx, &ry);
	libui->relative_mouse_x += rx;
	libui->relative_mouse_y += ry;
	if (libui->mouse_down
		&& (libui->mouse_x != libui->last_mouse_x
			|| libui->mouse_y != libui->last_mouse_y))
		libui->mouse_drag = 1;
	else
		libui->mouse_drag = 0;
}

void	bui_update_elements(t_bui_libui *libui)
{
	t_bui_window	*win;
	t_list			*curr_win;
	t_list			*curr_menu;

	curr_win = libui->windows;
	while (curr_win)
	{
		win = curr_win->content;
		curr_menu = win->menus;
		while (curr_menu)
		{
			update_element(curr_menu->content, win->win);
			curr_menu = curr_menu->next;
		}
		curr_win = curr_win->next;
	}
}

void	bui_event_handler(t_bui_libui *libui)
{
	SDL_Event	e;

	libui->mouse_down_last_frame = 0;
	libui->last_key = NULL;
	libui->mouse_wheel_x = 0;
	libui->mouse_wheel_y = 0;
	e = libui->event;
	while (SDL_PollEvent(&e))
	{
		if ((e.type == SDL_WINDOWEVENT
				&& e.window.event == SDL_WINDOWEVENT_CLOSE)
			|| key_pressed(libui, SDL_SCANCODE_ESCAPE))
			libui->run = 0;
		else if (e.key.type == SDL_KEYDOWN || e.key.type == SDL_KEYUP)
		{
			libui->keys[e.key.keysym.scancode] = e.key.type == SDL_KEYDOWN;
			key_was(libui, e.key.keysym, e.key.type == SDL_KEYDOWN);
		}
		mouse_update(libui, e);
	}
	get_mouse_states(libui);
	bui_update_elements(libui);
}
