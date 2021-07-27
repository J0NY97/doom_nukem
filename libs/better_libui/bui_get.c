/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 14:06:47 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 14:06:47 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

t_bui_element	*bui_get_element_with_text(t_bui_libui *libui, char *text)
{
	return (bui_get_element_with_text_from_list(libui->elements, text));
}

t_bui_element	*bui_get_element_with_text_from_list(t_list *list, char *text)
{
	t_list	*curr;

	if (!text)
		return (NULL);
	curr = list;
	while (curr)
	{
		if (ft_strequ(text, ((t_bui_element *)curr->content)->text))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}
