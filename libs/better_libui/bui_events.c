/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 14:04:48 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 14:04:50 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

int	bui_button(t_bui_element *button)
{
	if (button->state == ELEMENT_CLICK
		&& button->was_clicked_last_frame
		&& !button->already_clicked)
	{
		button->already_clicked = 1;
		return (1);
	}
	else
		return (0);
}

int	bui_button_allow_duplicate(t_bui_element *button)
{
	if (button->state == ELEMENT_CLICK)
		return (1);
	else
		return (0);
}

int	bui_button_toggle(t_bui_element *button)
{
	if (button->state == ELEMENT_CLICK)
	{
		if (button->was_clicked_last_frame == 1
			&& button->already_clicked == 0)
		{
			button->already_clicked = 1;
			button->toggle = button->toggle == 0;
		}
		return (1);
	}
	else
		return (0);
}
