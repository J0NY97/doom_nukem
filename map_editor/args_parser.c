/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 12:37:03 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 16:06:35 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	print_usage(void)
{
	ft_printf("Usage: ./doom_editor [map]\n\n"\
		"Note:\n"\
		"Map must end in .endless or .story\n"\
		"Map will be taken from %s\n"\
		"(a new map file with the [map] name will be made "\
		"if it\'s not in the \"maps\" directory)\n", MAP_PATH);
}

char	*args_parser(int ac, char **av)
{
	char	*map;

	map = NULL;
	if (ac == 1)
	{
		map = ft_strdup(MAP_PATH"map_name");
		return (map);
	}
	else if (ac > 1)
	{
		if (ft_strendswith(av[1], ".endless") == 0
			|| ft_strendswith(av[1], ".story") == 0)
			map = ft_strjoiner(MAP_PATH, av[1], NULL);
		else
			print_usage();
		return (map);
	}
	return (NULL);
}

char	*get_mapname_from_path(char *map)
{
	char	**mapname;
	char	*str;
	int		i;

	i = 0;
	mapname = ft_strsplit(map, '/');
	while (mapname[i + 1])
		i++;
	if (ft_strendswith(mapname[i], ".endless") == 0)
		ft_strremove(mapname[i], ".endless");
	else if (ft_strendswith(mapname[i], ".story") == 0)
		ft_strremove(mapname[i], ".story");
	str = ft_strdup(mapname[i]);
	free_array(mapname);
	return (str);
}
