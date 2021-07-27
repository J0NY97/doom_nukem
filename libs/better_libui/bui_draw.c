#include "better_libui.h"

void	set_pixel(SDL_Surface *surface, int x, int y, Uint32 color)
{
	Uint32	*pixels;

	pixels = surface->pixels;
	if (x >= 0 && x < surface->w
		&& y >= 0 && y < surface->h)
	{
		pixels[y * surface->w + x] = color;
	}
}

/*
 * WHY IS THIS CALLED "FILLED", WHEN IT WONT DRAW A FILLED RECT?
*/
void	draw_rect_filled(SDL_Surface *surface, t_xywh c, Uint32 color)
{
	int	i;
	int	j;

	i = c.x;
	j = c.y;
	while (i < c.x + c.w)
	{
		set_pixel(surface, i, c.y, color);
		set_pixel(surface, i, c.y + c.h - 1, color);
		i++;
	}
	while (j < c.y + c.h)
	{
		set_pixel(surface, c.x, j, color);
		set_pixel(surface, c.x + c.w - 1, j, color);
		j++;
	}
}

void	draw_rect(SDL_Surface *surface, t_xywh c, Uint32 color, int fill)
{
	int	i;
	int	j;

	i = c.x;
	j = c.y;
	if (fill)
	{
		while (i < c.x + c.w)
		{
			while (j < c.y + c.h)
			{
				memcpy(surface->pixels + ((j * surface->w + i)
						* sizeof(Uint32)), &color,
					sizeof(Uint32));
				j++;
			}
			i++;
		}
	}
	else
		draw_rect_filled(surface, c, color);
}

void	draw_rect_border(
		SDL_Surface *surface, t_xywh c, Uint32 color, unsigned int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		draw_rect(surface, (t_xywh){c.x + i, c.y + i, c.w - (i * 2),
			c.h - (i * 2)}, color, 0);
		i++;
	}
}

void	draw_surface_border(SDL_Surface *surface, Uint32 color, int thiccness)
{
	int		i;
	t_xywh	c;

	c.x = 0;
	c.y = 0;
	c.w = surface->w;
	c.h = surface->h;
	i = 0;
	if (!surface)
		ft_printf("[draw_surface_border] surface == NULL.\n");
	while (i < thiccness)
	{
		draw_rect(surface, (t_xywh){c.x + i, c.y + i,
			c.w - (i * 2), c.h - (i * 2)}, color, 0);
		i++;
	}
}
