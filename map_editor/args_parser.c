/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 12:37:03 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/13 13:40:41 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	print_usage(void)
{
	ft_printf("Usage: ./doom_editor [map]\n\n"\
		"Note:\n"\
		"Map must end in .endless or .story\n"\
		"Map will be taken from ./maps/\n"\
		"(a new map file with the [map] name will be made "\
		"if it\'s not in the \"maps\" directory)\n");
}

char	*args_parser(int ac, char **av)
{
	char	*map;

	map = NULL;
	if (ac == 1)
	{
		map = ft_strjoiner(ROOT_PATH"map_editor/maps/", "map_name", NULL);
		return (map);
	}
	else if (ac > 1)
	{
		if (ft_strendswith(av[1], ".endless") == 0
			|| ft_strendswith(av[1], ".story") == 0)
			map = ft_strjoiner(ROOT_PATH"map_editor/maps/", av[1], NULL);
		else
			print_usage();
		return (map);
	}
	return (NULL);
}
