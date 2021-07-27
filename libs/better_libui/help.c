/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:58:44 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 11:58:45 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

SDL_Surface	*create_surface(int w, int h)
{
	SDL_Surface	*surface;

	surface = SDL_CreateRGBSurface(0, w, h, 32,
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	if (!surface)
		ft_printf("CreateSurface: %s\n", SDL_GetError());
	return (surface);
}

int	hitbox_rect(int x, int y, t_xywh rect)
{
	if (x >= rect.x && x < rect.x + rect.w
		&& y >= rect.y && y < rect.y + rect.h)
		return (1);
	return (0);
}

t_xywh	new_xywh(int x, int y, int w, int h)
{
	t_xywh	coords;

	coords.x = x;
	coords.y = y;
	coords.w = w;
	coords.h = h;
	return (coords);
}

void	xywh_print(t_xywh c)
{
	ft_printf("{ .x = %d, .y = %d, .w = %d, .h = %d}\n",
		c.x, c.y, c.w, c.h);
}

void	clear_surface(SDL_Surface *surface)
{
	SDL_memset(surface->pixels, 0, surface->h * surface->pitch);
}
