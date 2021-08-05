/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:44:27 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/29 13:04:32 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_event	*new_event(void)
{
	t_event	*event;

	event = ft_memalloc(sizeof(t_event));
	event->type = NULL;
	event->action = NULL;
	event->id = -1;
	event->sector = NULL;
	event->min = -1;
	event->max = -1;
	event->speed = -1;
	return (event);
}

void	free_event(void *content, size_t size)
{
	t_event	*event;

	(void)size;
	event = content;
	if (event == NULL)
		return ;
	ft_strdel(&event->type);
	ft_strdel(&event->action);
	event->sector = NULL;
	free(event);
}
