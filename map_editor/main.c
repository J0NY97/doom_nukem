/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 12:27:49 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/19 15:35:48 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

int	main(int argc, char **argv)
{
	ft_printf("%s %s %s %s %s %s %s, %s\n", ft_itoa(-123), ft_itoa(-10), ft_itoa(-1), ft_itoa(0), ft_itoa(1), ft_itoa(10), ft_itoa(100), ft_itoa(123));
	char	*map;
	char	*arr[0];

	map = args_parser(argc, argv);
	if (!map)
		return (1);
	map_editor(map);
	system("leaks doom_editor");
	if (ft_strequ(argv[2], "-l")
		|| ft_strequ(argv[2], "-launcher"))
	{
		arr[0] = ft_strdup(ROOT_PATH"ui/menu");
		arr[1] = NULL;
		execv(arr[0], arr);
	}
	ft_strdel(&map);
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
	return (0);
}
