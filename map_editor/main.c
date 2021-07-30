/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:54:28 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/29 13:55:36 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

int	main(int argc, char **argv)
{
	char	*map;
	char	*arr[0];

	map = args_parser(argc, argv);
	if (!map)
		return (1);
	map_editor(map);
	ft_strdel(&map);
	if (argc == 3 && (ft_strequ(argv[2], "-l")
			|| ft_strequ(argv[2], "-launcher")))
	{
		arr[0] = ft_strdup(ROOT_PATH"wolf3d");
		arr[1] = NULL;
		execv(arr[0], arr);
	}
	system("leaks doom_editor");
	return (0);
}
