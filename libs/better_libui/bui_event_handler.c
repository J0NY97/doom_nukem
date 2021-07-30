/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_event_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 12:00:28 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 12:00:29 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

void	libui_events_clear(t_bui_libui *libui)
{
	memset(libui->keys, 0, sizeof(char) * SDL_NUM_SCANCODES);
	memset(libui->mouse_buttons, 0, sizeof(char) * 5);
	libui->mouse_down = 0;
	libui->mouse_down_last_frame = 0;
	libui->mouse_drag = 0;
	libui->mouse_x = -1;
	libui->mouse_y = -1;
	libui->last_mouse_x = -1;
	libui->last_mouse_y = -1;
	libui->mouse_click_x = -1;
	libui->mouse_click_y = -1;
	libui->global_mouse_x = -1;
	libui->global_mouse_y = -1;
}

void	key_was(t_bui_libui *libui, SDL_Keysym key, int type)
{
	SDL_Keycode	k;

	k = key.sym;
	if (type == 1)
		libui->last_key = (char *)SDL_GetKeyName(k);
	else
		libui->last_key = NULL;
}

int	key_pressed(t_bui_libui *libui, int key)
{
	if (key < 0 || key > SDL_NUM_SCANCODES)
		return (0);
	return (libui->keys[key]);
}

int	key_repeat(t_bui_libui *libui, int key)
{
	if (key < 0 || key > SDL_NUM_SCANCODES)
		return (0);
	return (libui->keys[key] > 1);
}

int	key_press(t_bui_libui *libui, char *key)
{
	if (libui->last_key != NULL
		&& ft_strcmp(libui->last_key, key) == 0)
	{
		libui->last_key = NULL;
		return (1);
	}
	else
		return (0);
}
