/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_preset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:59:15 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 11:59:16 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

int	only_one_button_toggled_at_a_time(t_list *list, t_bui_element **active)
{
	t_list	*curr;
	int		found;
	int		nth;

	found = 0;
	nth = 0;
	curr = list;
	while (curr)
	{
		if (((t_bui_element *)curr->content)->was_clicked_last_frame)
		{
			*active = curr->content;
			found = nth + 1;
		}
		else
			((t_bui_element *)curr->content)->toggle = 0;
		nth++;
		curr = curr->next;
	}
	if (*active != NULL)
		(*active)->toggle = 1;
	return (found);
}

int	only_one_button_toggled_at_a_time_can_be_untoggled(
		t_list *list, t_bui_element **active)
{
	t_list	*curr;
	int		found;

	found = 0;
	curr = list;
	while (curr)
	{
		if (((t_bui_element *)curr->content)->was_clicked_last_frame)
		{
			*active = curr->content;
			found = 1;
		}
		else
			((t_bui_element *)curr->content)->toggle = 0;
		curr = curr->next;
	}
	if (*active != NULL)
		(*active)->toggle = 1;
	return (found);
}

void	toggle_on_element_with_text(
		t_list *elements, t_bui_element **active, char *text)
{
	t_bui_element	*elem;
	t_list			*curr;

	curr = elements;
	while (curr)
	{
		elem = curr->content;
		if (ft_strcmp(elem->text, text) == 0)
		{
			elem->was_clicked_last_frame = 1;
			*active = elem;
		}
		else
			elem->was_clicked_last_frame = 0;
		curr = curr->next;
	}
}
