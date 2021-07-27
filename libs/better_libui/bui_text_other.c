/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_text_other.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 14:27:13 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 14:27:15 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

void	bui_set_element_text_color(t_bui_element *elem, Uint32 color)
{
	char	*font_name;

	font_name = ft_strdup(elem->font_name);
	bui_set_element_text_font(elem, font_name, elem->font_size, color);
	ft_strdel(&font_name);
}

void	bui_set_element_text_size(t_bui_element *elem, Uint32 size)
{
	char	*font_name;

	font_name = ft_strdup(elem->font_name);
	bui_set_element_text_font(elem, font_name, size, elem->text_color);
	ft_strdel(&font_name);
}

void	bui_element_set_text_font(t_bui_element *elem, char *font_name)
{
	bui_set_element_text_font(elem, font_name,
		elem->font_size, elem->text_color);
}

void	bui_element_set_text_style(t_bui_element *elem, int style)
{
	char	*font_name;

	elem->font_style = style;
	font_name = ft_strdup(elem->font_name);
	bui_set_element_text_font(elem, font_name,
		elem->font_size, elem->text_color);
	ft_strdel(&font_name);
}
