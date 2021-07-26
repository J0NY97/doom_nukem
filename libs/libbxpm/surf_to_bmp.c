/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surf_to_bmp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/09 12:48:53 by nneronin          #+#    #+#             */
/*   Updated: 2021/05/17 17:29:12 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bxpm.h"

void	get_bmp_header(t_header *header, int w, int h, int bpp)
{
	header->type = 0x4D42;
	header->size = w * h * bpp + 54;
	header->reserved1 = 0;
	header->reserved2 = 0;
	header->offset = 54;
	header->header_size = 40;
	header->width_px = w;
	header->height_px = h;
	header->num_planes = 1;
	header->bits_per_pixel = bpp * 8;
	header->compression = 0;
	header->image_bytes = w * h * bpp;
	header->x_pix_per_m = 0;
	header->y_pix_per_m = 0;
	header->num_colors = 1;
	header->import_colors = 0;
}

void	pixel_clr(uint32_t clr, int i, unsigned char *data, int bpp)
{
	if (bpp == 4)
	{
		data[i + 0] = clr;
		data[i + 1] = clr >> 8;
		data[i + 2] = clr >> 16;
		data[i + 3] = clr >> 24;
	}
	else if (bpp == 3)
	{
		data[i + 0] = clr;
		data[i + 1] = clr >> 8;
		data[i + 2] = clr >> 16;
	}
}

void	get_bmp_pixel_data(t_bmp *bmp, void *pixels, int bpp)
{
	int	x;
	int	y;
	int	i;

	bmp->data = malloc(bmp->header.image_bytes + 1);
	y = bmp->header.height_px;
	i = 0;
	while (y--)
	{
		x = -1;
		while (++x < bmp->header.width_px)
		{
			pixel_clr(((uint32_t *)pixels)[y * bmp->header.width_px + x],
				i, bmp->data, bpp);
			i += bpp;
		}
	}
}

t_bmp	*surface_to_bmp(int w, int h, int bpp, void *pixels)
{
	t_bmp	*bmp;

	if (bpp == 3 || bpp == 4)
	{
		bmp = malloc(sizeof(t_bmp));
		get_bmp_header(&bmp->header, w, h, bpp);
		get_bmp_pixel_data(bmp, pixels, bpp);
		return (bmp);
	}
	write(1, "[ERROR] bpp\n", 12);
	return (NULL);
}
