/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_to_bxpm.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/01 14:20:47 by nneronin          #+#    #+#             */
/*   Updated: 2021/05/17 17:29:45 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bxpm.h"

void	get_bxpm_header(t_bxpm *bxpm, int w, int h, int bpp)
{
	bxpm->w = w;
	bxpm->h = h;
	bxpm->bpp = bpp;
	bxpm->clr_nb = 0;
	bxpm->pix_nb = w * h;
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
	return (bxpm->clr_nb - 1);
}

void	get_bxpm_pixel_data(t_bxpm *bxpm, void *pixels)
{
	int	i;

	i = -1;
	bxpm->clr = malloc(sizeof(uint32_t));
	bxpm->pix = malloc(sizeof(unsigned short) * bxpm->pix_nb);
	while (++i < bxpm->pix_nb)
	{
		bxpm->pix[i] = check_clr(bxpm, ((uint32_t *)pixels)[i]);
	}
}

t_bxpm	*surface_to_bxpm(int w, int h, int bpp, void *pixels)
{
	t_bxpm	*bxpm;

	bxpm = ft_memalloc(sizeof(t_bxpm));
	get_bxpm_header(bxpm, w, h, bpp);
	get_bxpm_pixel_data(bxpm, pixels);
	return (bxpm);
}
