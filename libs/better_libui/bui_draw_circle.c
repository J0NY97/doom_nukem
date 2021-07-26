/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_draw_circle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/24 18:10:44 by nneronin          #+#    #+#             */
/*   Updated: 2021/07/24 18:12:08 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

static inline void	full_or_empty(SDL_Surface *surf, Uint32 color, t_shapes l)
{
	int	w;
	int	h;

	w = surf->w;
	h = surf->h;
	l.x2 = ft_clamp(l.x2, 0, w - 1);
	l.y2 = ft_clamp(l.y2, 0, h - 1);
	l.x1 = ft_clamp(l.x1, 0, w - 1);
	l.y1 = ft_clamp(l.y1, 0, h - 1);
	if (l.fill == 1)
		ft_create_line(surf, color, l);
	else
	{
		set_pixel(surf, l.x2, l.y2, color);
		set_pixel(surf, l.x1, l.y1, color);
	}
}

static inline void	full_circle(SDL_Surface *surf, t_shapes c, int x, int y)
{
	t_shapes	l;

	l.size = 1;
	l.fill = c.fill;
	l.x1 = c.x1 - x;
	l.y1 = c.y1 - y;
	l.x2 = c.x1 + x;
	l.y2 = c.y1 - y;
	full_or_empty(surf, c.color, l);
	l.x1 = c.x1 - y;
	l.y1 = c.y1 - x;
	l.x2 = c.x1 + y;
	l.y2 = c.y1 - x;
	full_or_empty(surf, c.color, l);
	l.x1 = c.x1 - y;
	l.y1 = c.y1 + x;
	l.x2 = c.x1 + y;
	l.y2 = c.y1 + x;
	full_or_empty(surf, c.color, l);
	l.x1 = c.x1 - x;
	l.y1 = c.y1 + y;
	l.x2 = c.x1 + x;
	l.y2 = c.y1 + y;
	full_or_empty(surf, c.color, l);
}

void	ft_create_circle(SDL_Surface *surf, Uint32 color, t_shapes c)
{
	int	x;
	int	y;
	int	dist;

	x = 0;
	y = c.size;
	c.color = color;
	dist = 3 - 2 * c.size;
	full_circle(surf, c, x, y);
	while (y >= x)
	{
		x++;
		if (dist >= 0)
		{
			y--;
			dist = dist + 4 * (x - y) + 10;
		}
		else
			dist = dist + 4 * x + 6;
		full_circle(surf, c, x, y);
	}
}
