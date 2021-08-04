/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:44:43 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 13:44:44 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

char	*set_event(t_editor *editor)
{
	char	*str;
	char	*temp;
	t_list	*curr;
	t_event	*event;
	int		i;

	i = -1;
	str = ft_sprintf("type:event\tTYPE\tACTION\tID\tSECTOR\tMIN\tMAX\tSPEED\n");
	curr = editor->grid.events;
	while (curr)
	{
		event = curr->content;
		temp = ft_sprintf("%d\t%s\t%s\t%d\t%d\t%d\t%d\t%d\n",
				++i,
				event->type,
				event->action,
				event->id,
				event->sector->id,
				event->min,
				event->max,
				event->speed);
		ft_stradd(&str, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (str);
}
