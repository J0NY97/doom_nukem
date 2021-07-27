/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_text_pos.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 14:10:58 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 14:11:06 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

void	bui_set_element_text_position(t_bui_element *elem, int x, int y)
{
	elem->text_x = x;
	elem->text_y = y;
}

void	bui_center_element_text_x(t_bui_element *elem)
{
	int	text_w;
	int	text_h;

	if (!elem->font || !elem->text)
		return ;
	TTF_SizeText(elem->font, elem->text, &text_w, &text_h);
	elem->text_x = (elem->position.w / 2) - (text_w / 2);
}

void	bui_center_element_text_y(t_bui_element *elem)
{
	int	text_w;
	int	text_h;

	if (!elem->font || !elem->text)
		return ;
	TTF_SizeText(elem->font, elem->text, &text_w, &text_h);
	elem->text_y = (elem->position.h / 2) - (text_h / 2);
}

void	bui_center_element_text(t_bui_element *elem)
{
	int	text_w;
	int	text_h;

	if (!elem->font || !elem->text)
		return ;
	TTF_SizeText(elem->font, elem->text, &text_w, &text_h);
	elem->text_y = (elem->position.h / 2) - (text_h / 2);
	elem->text_x = (elem->position.w / 2) - (text_w / 2);
}
