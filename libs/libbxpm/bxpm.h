/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bxpm.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 17:15:02 by nneronin          #+#    #+#             */
/*   Updated: 2021/09/05 06:45:16 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BXPM_H
# define BXPM_H

# include "libft.h"
# include "libpf.h"
# include <sys/stat.h>
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <string.h>

typedef struct s_bxpm
{
	int32_t				w;
	int32_t				h;
	int32_t				bpp;
	int32_t				clr_nb;
	int32_t				pix_nb;
	uint32_t			*clr;
	unsigned short		*pix;
	uint32_t			*shade[512];
}						t_bxpm;

typedef struct s_thread_bxpm
{
	int					active;
	char				*path;
	t_bxpm				*bxpm;
}						t_thread_bxpm;

typedef struct s_header
{
	uint16_t			type;
	uint32_t			size;
	uint16_t			reserved1;
	uint16_t			reserved2;
	uint32_t			offset;
	uint32_t			header_size;
	int32_t				width_px;
	int32_t				height_px;
	uint16_t			num_planes;
	uint16_t			bits_per_pixel;
	uint32_t			compression;
	uint32_t			image_bytes;
	int32_t				x_pix_per_m;
	int32_t				y_pix_per_m;
	uint32_t			num_colors;
	uint32_t			import_colors;
}						t_header;

typedef struct s_bmp
{
	t_header			header;
	unsigned char		*data;
}						t_bmp;

void					write_bmp(char *name, t_bmp *bmp);
void					write_bxpm(t_bxpm *bxpm, char *path);
void					write_bxpm2(t_bxpm *bxpm, char *path, char *name);
int						read_bmp(t_bmp *bmp, char *file);
int						read_bxpm(t_bxpm *bxpm, char *file);
t_bmp					*surface_to_bmp(int w, int h, int bpp, void *pixels);
t_bxpm					*surface_to_bxpm(int w, int h, int bpp, void *pixels);
t_bxpm					*bmp_to_bxpm(t_bmp *bmp);
int32_t					read_int32(unsigned char *str, int index);
int32_t					read_int16(unsigned char *str, int index);
uint32_t				read_color32(unsigned char *str, int index);
uint32_t				read_color24(unsigned char *str, int i);
unsigned int			bpp_clr(double byte, unsigned char *str, uint32_t bpp);
void					free_bxpm(t_bxpm *bxpm);
void					free_bmp(t_bmp *bmp);
int						multithread_read_bxpm(void *arg);

#endif
