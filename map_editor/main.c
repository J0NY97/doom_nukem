/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 12:27:49 by jsalmi            #+#    #+#             */
/*   Updated: 2021/05/13 14:15:44 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	fps_func(t_fps *fps)
{
	fps->curr = SDL_GetTicks();
	fps->count++;
	if (fps->curr - fps->prev >= 1000)
	{
		fps->prev = fps->curr;
		fps->fps = fps->count;
		fps->count = 0;
		ft_putnbr(fps->fps);
		ft_putchar('\n');
	}
}

int main(int argc, char **argv)
{
	char *map;

	if (!(map = args_parser_v2(argc, argv)))
		return (1);
	map_editor(map);
	ft_strdel(&map);
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
//	while (1);
	return (0);
}
