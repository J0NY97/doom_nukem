/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_preset_scrollbar.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 14:05:51 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 14:05:52 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

t_preset_scrollbar	*bui_new_scrollbar_preset(
		t_bui_element *menu, t_xywh pos, t_bui_element *target)
{
	t_preset_scrollbar	*scrollbar;

	scrollbar = ft_memalloc(sizeof(t_preset_scrollbar));
	scrollbar->scrollbar = bui_new_element(menu, "scrollbar", pos);
	scrollbar->button = bui_new_element(scrollbar->scrollbar, NULL,
			(t_xywh){1, 1, pos.w - 2, 20});
	bui_set_element_color(scrollbar->button, 0xffef476f);
	scrollbar->button->position.y
		= bui_set_slider_value(0, 0, target->position.h,
			scrollbar->scrollbar->position.h);
	scrollbar->target = target;
	scrollbar->min = target->position.y;
	scrollbar->max = target->position.y + target->position.h;
	scrollbar->value = target->position.y;
	return (scrollbar);
}

void	preset_scrollbar_events(t_preset_scrollbar *scrollbar)
{
	t_bui_libui	*libui;
	t_list		*curr;
	int			half_butt;

	libui = scrollbar->scrollbar->libui;
	if (bui_button_allow_duplicate(scrollbar->button)
		|| bui_button(scrollbar->scrollbar))
	{
		half_butt = scrollbar->button->position.h * 0.5f;
		scrollbar->button->position.y = ft_clamp(libui->mouse_y
				- scrollbar->scrollbar->screen_pos.y - half_butt, 0 - half_butt,
				scrollbar->scrollbar->position.h);
		scrollbar->value = bui_get_slider_value(
				scrollbar->min, scrollbar->max,
				scrollbar->button->position.y + half_butt,
				scrollbar->scrollbar->position.h);
		ft_printf("Value: %d\n", scrollbar->value);
	}
	else if (scrollbar->target->was_hovered_last_frame
		&& libui->mouse_wheel_y != 0)
		scrollbar->value += libui->mouse_wheel_y * 15;
	scrollbar->target->position.y = scrollbar->value;
}
