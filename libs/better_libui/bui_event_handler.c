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
	memset(libui->events, 0, sizeof(char) * EVENT_LAST);
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

// Prints out all the keys you have currently pressed.
void	libui_events_check(t_bui_libui *libui)
{
	int	total;
	int	i;

	total = 0;
	i = EVENT_FIRST + 1;
	ft_putstr("\n[Events Checker]\n");
	while (i < EVENT_LAST)
	{
		if (libui->events[i] == 1)
			total++;
		i++;
	}
	ft_printf("Total Keys Pressed: %d\n", total);
}

void	key_was_alpha0(t_bui_libui *libui, SDL_Keycode k, int type)
{
	if (k == SDLK_a)
		libui->events[KEY_A] = type;
	else if (k == SDLK_b)
		libui->events[KEY_B] = type;
	else if (k == SDLK_c)
		libui->events[KEY_C] = type;
	else if (k == SDLK_d)
		libui->events[KEY_D] = type;
	else if (k == SDLK_e)
		libui->events[KEY_E] = type;
	else if (k == SDLK_f)
		libui->events[KEY_F] = type;
	else if (k == SDLK_g)
		libui->events[KEY_G] = type;
	else if (k == SDLK_h)
		libui->events[KEY_H] = type;
	else if (k == SDLK_i)
		libui->events[KEY_I] = type;
	else if (k == SDLK_j)
		libui->events[KEY_J] = type;
	else if (k == SDLK_k)
		libui->events[KEY_K] = type;
	else if (k == SDLK_l)
		libui->events[KEY_L] = type;
}

void	key_was_alpha1(t_bui_libui *libui, SDL_Keycode k, int type)
{
	if (k == SDLK_m)
		libui->events[KEY_M] = type;
	else if (k == SDLK_n)
		libui->events[KEY_N] = type;
	else if (k == SDLK_o)
		libui->events[KEY_O] = type;
	else if (k == SDLK_p)
		libui->events[KEY_P] = type;
	else if (k == SDLK_q)
		libui->events[KEY_Q] = type;
	else if (k == SDLK_r)
		libui->events[KEY_R] = type;
	else if (k == SDLK_s)
		libui->events[KEY_S] = type;
	else if (k == SDLK_t)
		libui->events[KEY_T] = type;
	else if (k == SDLK_u)
		libui->events[KEY_U] = type;
	else if (k == SDLK_v)
		libui->events[KEY_V] = type;
	else if (k == SDLK_w)
		libui->events[KEY_W] = type;
	else if (k == SDLK_x)
		libui->events[KEY_X] = type;
}

void	key_was_alpha(t_bui_libui *libui, SDL_Keycode k, int type)
{
	key_was_alpha0(libui, k, type);
	key_was_alpha1(libui, k, type);
	if (k == SDLK_y)
		libui->events[KEY_Y] = type;
	else if (k == SDLK_z)
		libui->events[KEY_Z] = type;
}

void	key_was_kp_num(t_bui_libui *libui, SDL_Keycode k, int type)
{
	if (k == SDLK_KP_0)
		libui->events[KEYP_0] = type;
	else if (k == SDLK_KP_1)
		libui->events[KEYP_1] = type;
	else if (k == SDLK_KP_2)
		libui->events[KEYP_2] = type;
	else if (k == SDLK_KP_3)
		libui->events[KEYP_3] = type;
	else if (k == SDLK_KP_4)
		libui->events[KEYP_4] = type;
	else if (k == SDLK_KP_5)
		libui->events[KEYP_5] = type;
	else if (k == SDLK_KP_6)
		libui->events[KEYP_6] = type;
	else if (k == SDLK_KP_7)
		libui->events[KEYP_7] = type;
	else if (k == SDLK_KP_8)
		libui->events[KEYP_8] = type;
	else if (k == SDLK_KP_9)
		libui->events[KEYP_9] = type;
}

void	key_was_num(t_bui_libui *libui, SDL_Keycode k, int type)
{
	if (k == SDLK_0)
		libui->events[KEY_0] = type;
	else if (k == SDLK_1)
		libui->events[KEY_1] = type;
	else if (k == SDLK_2)
		libui->events[KEY_2] = type;
	else if (k == SDLK_3)
		libui->events[KEY_3] = type;
	else if (k == SDLK_4)
		libui->events[KEY_4] = type;
	else if (k == SDLK_5)
		libui->events[KEY_5] = type;
	else if (k == SDLK_6)
		libui->events[KEY_6] = type;
	else if (k == SDLK_7)
		libui->events[KEY_7] = type;
	else if (k == SDLK_8)
		libui->events[KEY_8] = type;
	else if (k == SDLK_9)
		libui->events[KEY_9] = type;
}

void	key_was_other(t_bui_libui *libui, SDL_Keycode k, int type)
{
	if (k == SDLK_SPACE)
		libui->events[KEY_SPACE] = type;
	else if (k == SDLK_LCTRL)
		libui->events[KEY_LCTRL] = type;
	else if (k == SDLK_LSHIFT)
		libui->events[KEY_LSHIFT] = type;
	else if (k == SDLK_RETURN)
		libui->events[KEY_RETURN] = type;
	else if (k == SDLK_ESCAPE)
		libui->events[KEY_ESCAPE] = type;
	else if (k == SDLK_BACKSPACE)
		libui->events[KEY_BACKSPACE] = type;
}

void	key_was(t_bui_libui *libui, SDL_Keysym key, int type)
{
	SDL_Keycode	k;

	k = key.sym;
	if (type == 1)
		libui->last_key = (char *)SDL_GetKeyName(k);
	else
		libui->last_key = NULL;
	key_was_alpha(libui, k, type);
	key_was_num(libui, k, type);
	key_was_kp_num(libui, k, type);
	key_was_other(libui, k, type);
	if (k == SDLK_LEFT)
		libui->events[KEY_LEFT] = type;
	else if (k == SDLK_RIGHT)
		libui->events[KEY_RIGHT] = type;
	else if (k == SDLK_UP)
		libui->events[KEY_UP] = type;
	else if (k == SDLK_DOWN)
		libui->events[KEY_DOWN] = type;
	else if (k == SDLK_KP_MINUS)
		libui->events[KPKEY_MINUS] = type;
	else if (k == SDLK_KP_PLUS)
		libui->events[KPKEY_PLUS] = type;
}

void	mouse_key_was(t_bui_libui *libui, Uint8 button, int type)
{
	if (button == SDL_BUTTON_LEFT)
		libui->events[MKEY_LEFT] = type;
	else if (button == SDL_BUTTON_RIGHT)
		libui->events[MKEY_RIGHT] = type;
	else if (button == SDL_BUTTON_MIDDLE)
		libui->events[MKEY_MIDDLE] = type;
}

int	mouse_moved(t_bui_libui *libui)
{
	if (libui->mouse_x != libui->last_mouse_x
		|| libui->mouse_y != libui->last_mouse_y)
		return (1);
	return (0);
}

int	mouse_wheel(t_bui_libui *libui)
{
	if (libui->mouse_wheel_x != 0
		|| libui->mouse_wheel_y != 0)
		return (1);
	return (0);
}

int	key_pressed(t_bui_libui *libui, int key)
{
	return (libui->events[key]);
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

int	mouse_pressed(t_bui_libui *libui, int key)
{
	if (libui->events[key] == 1)
		return (1);
	return (0);
}

int	mouse_hover(t_bui_libui *libui, t_xywh rect)
{
	return (hitbox_rect(libui->mouse_x, libui->mouse_y, rect));
}

int	alpha_pressed(t_bui_libui *libui)
{
	int	i;

	i = KEY_A;
	while (i <= KEY_Z)
	{
		if (libui->events[i])
			return (1);
		i++;
	}
	return (0);
}

int	number_pressed(t_bui_libui *libui)
{
	int	i;

	i = KEY_0;
	while (i <= KEY_9)
	{
		if (libui->events[i])
			return (1);
		i++;
	}
	return (0);
}
