/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_bxpm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/25 14:42:43 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/27 12:47:35 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bxpm.h"
#include <stdio.h>

void	write_bxpm_header(t_bxpm *bxpm, FILE *fd)
{
	fwrite((const void *)&bxpm->w, 1, sizeof(uint32_t), fd);
	fwrite((const void *)&bxpm->h, 1, sizeof(uint32_t), fd);
	fwrite((const void *)&bxpm->clr_nb, 1, sizeof(uint32_t), fd);
	fwrite((const void *)&bxpm->pix_nb, 1, sizeof(uint32_t), fd);
	fwrite((const void *)&bxpm->bpp, 1, sizeof(uint32_t), fd);
}

void	write_bxpm_colors(t_bxpm *bxpm, FILE *fd)
{
	unsigned char	*clr;
	
	clr = (unsigned char *)bxpm->clr;
	fwrite((const void *)clr, bxpm->clr_nb, 4, fd);
}

void	write_bxpm_pixels(t_bxpm *bxpm, FILE *fd)
{
	unsigned char	*pix;
	
	pix = (unsigned char *)bxpm->pix;
	fwrite((const void *)pix, bxpm->pix_nb, 2, fd);
}

void	write_bxpm(t_bxpm *bxpm, char *path)
{
	FILE	*fd;
	char	*full;

	full = ft_strjoin(path, ".bxpm");
	fd = fopen(full, "wb");
	write_bxpm_header(bxpm, fd);
	write_bxpm_colors(bxpm, fd);
	write_bxpm_pixels(bxpm, fd);
	free(full);
	fclose(fd);
}
