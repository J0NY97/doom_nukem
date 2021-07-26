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

void	fill_surface(SDL_Surface *surface, Uint32 color)
{
	int				i;
	Uint32			*pixels;
	unsigned int	num_pixels;

	i = 0;
	pixels = surface->pixels;
	num_pixels = surface->w * surface->h;
	while (i < num_pixels)
	{
		pixels[i] = color;
		i++;
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

SDL_Surface	*create_surface(int w, int h)
{
	SDL_Surface	*surface;

	surface = SDL_CreateRGBSurface(0, w, h, 32,
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	if (!surface)
		ft_printf("CreateSurface: %s\n", SDL_GetError());
	return (surface);
}

t_rgba	hex_to_rgba(Uint32 color_hex)
{
	t_rgba	rgba;

	rgba.a = (color_hex >> 24 & 0xff);
	rgba.r = (color_hex >> 16 & 0xff);
	rgba.g = (color_hex >> 8 & 0xff);
	rgba.b = (color_hex & 0xff);
	return (rgba);
}

Uint32	rgba_to_hex(t_rgba rgba)
{
	return ((rgba.a & 0xff) << 24
		| ((rgba.r & 0xff) << 16)
		| ((rgba.g & 0xff) << 8)
		| (rgba.b & 0xff));
}

int	hitbox_rect(int x, int y, t_xywh rect)
{
	if (x >= rect.x && x < rect.x + rect.w
		&& y >= rect.y && y < rect.y + rect.h)
		return (1);
	return (0);
}

t_xywh	ui_init_coords(int x, int y, int w, int h)
{
	t_xywh	coords;

	coords.x = x;
	coords.y = y;
	coords.w = w;
	coords.h = h;
	return (coords);
}

// Basically the same as ui_init_coords, but shorter name
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

void	bui_fps_func(t_fps *fps)
{
	fps->curr = SDL_GetTicks();
	fps->count++;
	if (fps->curr - fps->prev >= 1000)
	{
		fps->prev = fps->curr;
		fps->fps = fps->count;
		fps->count = 0;
	}
}

void	clear_surface(SDL_Surface *surface)
{
	SDL_memset(surface->pixels, 0, surface->h * surface->pitch);
}
