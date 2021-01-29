/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 12:19:16 by jsalmi            #+#    #+#             */
/*   Updated: 2020/09/27 15:42:22 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core.h"

void	split_texture(t_texture *t)
{
	int				y;
	int				x;
	unsigned long	i;

	y = 0;
	x = 0;
	i = 0;
	t->position = (int **)malloc(sizeof(int *) * t->max_textures);
	while (t->max_textures > i)
	{
		t->position[i] = (int *)malloc(sizeof(int) * 2);
		t->position[i][0] = x;
		t->position[i][1] = y;
		x += t->x_size;
		i += 1;
		if ((i) % (t->max_textures / t->y_img) == 0)
		{
			y += t->y_size;
			x = 0;
		}
	}
}

void	load_texture(t_texture *t, char *dir, int x_size, int y_size)
{
	t->surface = load_image(dir);
	t->pixels = t->surface->pixels;
	t->x_img = t->surface->w / x_size;
	t->y_img = t->surface->h / y_size;
	t->max_textures = t->x_img * t->y_img;
	t->x_size = x_size;
	t->y_size = y_size;
}
