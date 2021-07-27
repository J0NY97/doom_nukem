/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 14:06:22 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 14:06:23 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

// Remove this if you dont come up with something better.
void	bui_set_element_flags(t_bui_element *elem, Uint32 flags)
{
	if (flags & BUI_ELEMENT_DONT_UPDATE)
		elem->update = 0;
	else if (flags & BUI_ELEMENT_UPDATE)
		elem->update = 1;
	if (flags & BUI_ELEMENT_DONT_UPDATE_STATE)
		elem->update_state = 0;
	else if (flags & BUI_ELEMENT_UPDATE_STATE)
		elem->update_state = 1;
	if (flags & BUI_ELEMENT_DONT_SHOW)
		elem->show = 0;
	else if (flags & BUI_ELEMENT_SHOW)
		elem->show = 1;
	if (flags & BUI_ELEMENT_DONT_CLEAR)
		elem->clear = 0;
	else if (flags & BUI_ELEMENT_CLEAR)
		elem->clear = 1;
}

void	bui_set_element_color(t_bui_element *elem, Uint32 color)
{
	elem->color = color;
	elem->color_rgba = hex_to_rgba(elem->color);
	elem->color_light = rgba_to_hex((t_rgba){
			elem->color_rgba.a,
			elem->color_rgba.r * 0.8f,
			elem->color_rgba.g * 0.8f,
			elem->color_rgba.b * 0.8f});
	elem->color_dark = rgba_to_hex((t_rgba){
			elem->color_rgba.a,
			elem->color_rgba.r * 1.5f,
			elem->color_rgba.g * 1.5f,
			elem->color_rgba.b * 1.5f});
	SDL_FillRect(elem->surface[0], NULL, elem->color);
	SDL_FillRect(elem->surface[1], NULL, elem->color_light);
	SDL_FillRect(elem->surface[2], NULL, elem->color_dark);
}

void	bui_set_element_state_border(
		t_bui_element *elem, int thiccness, Uint32 color, int state)
{
	if (state == ELEMENT_ALL)
		draw_surface_border(elem->active_surface, color, thiccness);
	if (state == ELEMENT_ALL || state == ELEMENT_DEFAULT)
		draw_surface_border(elem->surface[0], color, thiccness);
	if (state == ELEMENT_ALL || state == ELEMENT_HOVER)
		draw_surface_border(elem->surface[1], color, thiccness);
	if (state == ELEMENT_ALL || state == ELEMENT_CLICK)
		draw_surface_border(elem->surface[2], color, thiccness);
}

void	bui_set_element_border(t_bui_element *elem, int thiccness, Uint32 color)
{
	bui_set_element_state_border(elem, thiccness, color, ELEMENT_ALL);
}

void	bui_set_element_state(t_bui_element *elem, int state)
{
	if (state > 2 || state < 0)
	{
		ft_putstr("[bui_set_element_state] no state with that number.\n");
		return ;
	}
	elem->state = state;
	if (state == ELEMENT_CLICK)
		elem->was_clicked_last_frame = 1;
}
