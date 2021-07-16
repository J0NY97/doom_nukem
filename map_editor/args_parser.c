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
			ft_putstr("[ERROR] Invalid map name!\n");
		return (map);
	}
	return (NULL);
}
