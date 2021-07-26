/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 12:27:49 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/26 13:30:24 by jsalmi           ###   ########.fr       */
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
	system("leaks doom_editor");
	if (ft_strequ(argv[2], "-l")
		|| ft_strequ(argv[2], "-launcher"))
	{
		arr[0] = ft_strdup(ROOT_PATH"wolf3d");
		arr[1] = NULL;
		execv(arr[0], arr);
	}
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
	return (0);
}
