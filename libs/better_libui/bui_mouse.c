/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_mouse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 16:28:00 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 16:28:02 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

int	mouse_pressed(t_bui_libui *libui, int key)
{
	if (key < 0 || key > 4)
		return (0);
	return (libui->mouse_buttons[key]);
}

int	mouse_hover(t_bui_libui *libui, t_xywh rect)
{
	return (hitbox_rect(libui->mouse_x, libui->mouse_y, rect));
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
