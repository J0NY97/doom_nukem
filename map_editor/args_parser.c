/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 12:37:03 by jsalmi            #+#    #+#             */
/*   Updated: 2021/05/06 14:03:03 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

// !!!!!!!!!!!!!!!!!!!IMPORTANT!!!!!!!!!!!!!!!!!!
// TODO: remember to norminette ft_strendswith(); in libft
int	args_parser(t_editor *editor, int ac, char **av)
{
	ft_putstr("Parsing args\n");
	if (ac == 1)
	{
		editor->fullpath = ft_strjoiner(ROOT_PATH"map_editor/maps/", "default_name", NULL);
		editor->mapname = ft_strdup("default_name");
		ft_printf("Map name: %s, fullpath: %s\n", editor->mapname, editor->fullpath);
		return (1);
	}
	if (ac < 2)
	{
		ft_putstr("Usage: ./doom_editor [map_name]\n");
		return (0);
	}
	else if (ac > 1)
	{
		if (ft_strendswith(av[1], ".endless") == 0)
		{
			editor->fullpath = ft_strjoiner(ROOT_PATH"map_editor/maps/", av[1], NULL);
			editor->mapname = ft_strdup(ft_strremove(av[1], ".endless"));
		}
		else if (ft_strendswith(av[1], ".story") == 0)
		{
			editor->fullpath = ft_strjoiner(ROOT_PATH"map_editor/maps/", av[1], NULL);
			editor->mapname = ft_strdup(ft_strremove(av[1], ".story"));
		}
		else if (ft_strendswith(av[1], ".doom") == 0)
		{
			editor->fullpath = ft_strjoiner(ROOT_PATH"map_editor/maps/", av[1], NULL);
			editor->mapname = ft_strdup(ft_strremove(av[1], ".doom"));
		}	
		else
		{
			editor->mapname = ft_strdup(av[1]);
			editor->fullpath = ft_strjoiner(ROOT_PATH"map_editor/maps/", av[1], ".doom", NULL);
		}
		ft_printf("Map name: %s, fullpath: %s\n", editor->mapname, editor->fullpath);
		return (1);
	}
	return (0);
}
