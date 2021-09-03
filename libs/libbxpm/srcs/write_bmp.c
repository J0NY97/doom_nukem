/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_bmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/09 12:48:53 by nneronin          #+#    #+#             */
/*   Updated: 2021/07/10 13:50:35 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bxpm.h"

void	write_bpm_header(FILE *fd, t_header *header)
{
	fwrite((const void *)&header->type, 1, sizeof(uint16_t), fd);
	fwrite((const void *)&header->size, 1, sizeof(uint32_t), fd);
	fwrite((const void *)&header->reserved1, 1, sizeof(uint16_t), fd);
	fwrite((const void *)&header->reserved2, 1, sizeof(uint16_t), fd);
	fwrite((const void *)&header->offset, 1, sizeof(uint32_t), fd);
	fwrite((const void *)&header->header_size, 1, sizeof(uint32_t), fd);
	fwrite((const void *)&header->width_px, 1, sizeof(int32_t), fd);
	fwrite((const void *)&header->height_px, 1, sizeof(int32_t), fd);
	fwrite((const void *)&header->num_planes, 1, sizeof(uint16_t), fd);
	fwrite((const void *)&header->bits_per_pixel, 1, sizeof(uint16_t), fd);
	fwrite((const void *)&header->compression, 1, sizeof(uint32_t), fd);
	fwrite((const void *)&header->image_bytes, 1, sizeof(uint32_t), fd);
	fwrite((const void *)&header->x_pix_per_m, 1, sizeof(int32_t), fd);
	fwrite((const void *)&header->y_pix_per_m, 1, sizeof(int32_t), fd);
	fwrite((const void *)&header->num_colors, 1, sizeof(uint32_t), fd);
	fwrite((const void *)&header->import_colors, 1, sizeof(uint32_t), fd);
}

void	write_bpm_pixel_data(FILE *fd, t_bmp *bmp)
{
	fwrite(bmp->data, 1, bmp->header.image_bytes, fd);
}

void	write_bmp(char *name, t_bmp *bmp)
{
	FILE	*fd;

	fd = fopen(name, "wb");
	write_bpm_header(fd, &bmp->header);
	write_bpm_pixel_data(fd, bmp);
	fclose(fd);
}
