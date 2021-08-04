/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_bxpm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/05 13:20:28 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/02 12:29:57 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bxpm.h"

int	read_bxpm_header(FILE *fd, t_bxpm *bxpm)
{
	unsigned char	header[20];

	if (fread(header, 20, 1, fd) != 1)
		return (0);
	bxpm->w = read_int32(header, 0);
	bxpm->h = read_int32(header, 4);
	bxpm->clr_nb = read_int32(header, 8);
	bxpm->pix_nb = read_int32(header, 12);
	bxpm->bpp = read_int32(header, 16);
	bxpm->clr = malloc(sizeof(uint32_t) * bxpm->clr_nb);
	if (!bxpm->clr)
		return (0);
	bxpm->pix = malloc(sizeof(unsigned short) * bxpm->pix_nb);
	if (!bxpm->pix)
		return (0);
	return (1);
}

int	read_bxpm_clr(FILE *fd, t_bxpm *bxpm)
{
	int				i;
	int				x;
	unsigned char	*clr;

	i = 0;
	x = 0;
	clr = malloc(sizeof(unsigned char) * (bxpm->clr_nb * 4));
	if (!clr)
		return (0);
	if (fread(clr, bxpm->clr_nb, 4, fd) != 4)
		return (0);
	while (i < bxpm->clr_nb * 4)
	{
		bxpm->clr[x] = read_color32(clr, i);
		i += 4;
		x++;
	}
	free(clr);
	return (1);
}

int	read_bxpm_pix(FILE *fd, t_bxpm *bxpm)
{
	int				i;
	int				x;
	unsigned char	*pix;

	i = 0;
	x = 0;
	pix = malloc(sizeof(unsigned char) * (bxpm->pix_nb * 2));
	if (!pix)
		return (0);
	if (fread(pix, bxpm->pix_nb, 2, fd) != 2)
		return (0);
	while (i < bxpm->pix_nb * 2)
	{
		bxpm->pix[x] = read_int16(pix, i);
		i += 2;
		x++;
	}
	free(pix);
	return (1);
}

int	read_bxpm(t_bxpm *bxpm, char *file)
{
	FILE	*fd;

	fd = fopen(file, "rb");
	if (fd == NULL)
		return (0);
	if (!read_bxpm_header(fd, bxpm))
		return (0);
	if (!read_bxpm_clr(fd, bxpm))
		return (0);
	if (!read_bxpm_pix(fd, bxpm))
		return (0);
	fclose(fd);
	return (1);
}

int	multithread_read_bxpm(void *arg)
{
	int	res;

	res = read_bxpm(((t_thread_bxpm *)arg)->bxpm, ((t_thread_bxpm *)arg)->path);
	free(((t_thread_bxpm *)arg)->path);
	if (res)
		return (1);
	ft_printf("Could not read %s\n", ((t_thread_bxpm *)arg)->path);
	return (0);
}
