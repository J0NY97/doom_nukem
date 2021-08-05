/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/11 13:40:11 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 16:41:15 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	read_event_helper(t_editor *editor, t_event *event, char **arr)
{
	event->type = ft_strdup(arr[1]);
	event->action = ft_strdup(arr[2]);
	event->id = ft_atoi(arr[3]);
	if (!ft_strequ(arr[4], "-"))
		event->sector = get_sector_with_id(editor->grid.sectors,
				atoi(arr[4]));
	if (!ft_strequ(arr[5], "-"))
		event->min = ft_atoi(arr[5]);
	if (!ft_strequ(arr[6], "-"))
		event->max = ft_atoi(arr[6]);
	if (!ft_strequ(arr[7], "-"))
		event->speed = ft_atoi(arr[7]);
}

/*
 * NOTE: read_events needs to be called after we have gotten all the sectors.
*/
void	read_events(t_editor *editor, int fd)
{
	char	*line;
	char	**arr;
	t_event	*event;

	while (get_next_line(fd, &line))
	{
		if (line[0] == '-')
			break ;
		arr = ft_strsplit(line, '\t');
		event = new_event();
		read_event_helper(editor, event, arr);
		add_to_list(&editor->grid.events, event, sizeof(t_event));
		free_array(arr);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}
