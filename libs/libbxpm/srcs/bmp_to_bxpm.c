/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_bxpm.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/25 16:45:27 by nneronin          #+#    #+#             */
/*   Updated: 2021/05/20 16:53:48 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bxpm.h"

void	fix_byte_alignment(int *x, int *y, double *byte, t_bmp *bmp)
{
	double	trash;

	if (*x >= bmp->header.width_px)
	{
		while (modf(*byte, &trash) != 0)
			(*byte) += bmp->header.bits_per_pixel / 8.0;
		while ((int)*byte % 4 != 0)
			(*byte)++;
		*x = 0;
		(*y)--;
	}
}

unsigned short	check_clr(t_bxpm *bxpm, uint32_t clr)
{
	unsigned short	i;

	i = -1;
	while (++i < bxpm->clr_nb)
	{
		if (bxpm->clr[i] == clr)
			return (i);
	}
	bxpm->clr_nb += 1;
	bxpm->clr = realloc(bxpm->clr, sizeof(uint32_t) * (bxpm->clr_nb));
	bxpm->clr[bxpm->clr_nb - 1] = clr;
	if (bxpm->clr_nb >= 65025)
		ft_printf("[ERROR] clr_nb reached max [%d]\n", bxpm->clr_nb);
	return (bxpm->clr_nb - 1);
}

void	bxpm_data(t_bxpm *bxpm, t_bmp *bmp)
{
	int		x;
	int		y;
	int		i;
	double	byte;

	x = 0;
	y = bmp->header.height_px - 1;
	byte = 0;
	if (bxpm->bpp == 32)
		byte = 16;
	while (byte + 4 < bmp->header.image_bytes)
	{
		i = check_clr(bxpm, bpp_clr(byte, bmp->data, bxpm->bpp));
		bxpm->pix[y * bxpm->w + x] = i;
		byte += bmp->header.bits_per_pixel / 8.0;
		x++;
		fix_byte_alignment(&x, &y, &byte, bmp);
	}
}

t_bxpm	*bmp_to_bxpm(t_bmp *bmp)
{
	t_bxpm	*bxpm;

	bxpm = malloc(sizeof(t_bxpm));
	bxpm->w = bmp->header.width_px;
	bxpm->h = bmp->header.height_px;
	bxpm->bpp = bmp->header.bits_per_pixel;
	bxpm->clr_nb = 0;
	bxpm->pix_nb = bxpm->w * bxpm->h;
	bxpm->clr = malloc(sizeof(uint32_t));
	bxpm->pix = malloc(sizeof(unsigned short) * bxpm->pix_nb);
	bxpm_data(bxpm, bmp);
	return (bxpm);
}
