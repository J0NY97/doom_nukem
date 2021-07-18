/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 12:27:49 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/13 13:38:57 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

int main(int argc, char **argv)
{
	char *map;

	if (!(map = args_parser(argc, argv)))
		return (1);
	map_editor(map);
	ft_strdel(&map);
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
//	while (1);
	return (0);
}
