/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_draw_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/24 18:10:35 by nneronin          #+#    #+#             */
/*   Updated: 2021/07/24 18:15:12 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

static inline int	sign(int a, int b)
{
	if (a < b)
		return (1);
	return (-1);
}

static inline void	line_calc(t_shapes *l, t_shapes *c,
		int *cath_x, int *cath_y)
{
	c->fill = 0;
	c->size = l->size;
	if ((l->y1 - l->y2) < 0)
		*cath_x = (l->y1 - l->y2) * -1;
	else
		*cath_x = (l->y1 - l->y2);
	if ((l->x1 - l->x2) < 0)
		*cath_y = (l->x1 - l->x2) * -1;
	else
		*cath_y = (l->x1 - l->x2);
}

void	ft_create_line(SDL_Surface *surf, Uint32 color, t_shapes l)
{
	t_shapes	c;
	int			cath_x;
	int			cath_y;
	int			overflow_x;
	int			overflow_y;

	line_calc(&l, &c, &cath_x, &cath_y);
	overflow_y = cath_y - cath_x;
	while (l.x2 != l.x1 || l.y2 != l.y1)
	{
		c.x1 = l.x2;
		c.y1 = l.y2;
		ft_create_circle(surf, color, c);
		overflow_x = overflow_y * 2;
		if (overflow_x > -(cath_x))
		{
			overflow_y -= cath_x;
			l.x2 += sign(l.x2, l.x1);
		}
		else if (overflow_x < cath_x)
		{
			overflow_y += cath_y;
			l.y2 += sign(l.y2, l.y1);
		}
	}
}
