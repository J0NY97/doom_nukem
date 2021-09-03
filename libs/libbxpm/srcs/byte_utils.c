/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_byte.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/25 13:37:46 by nneronin          #+#    #+#             */
/*   Updated: 2021/05/17 17:24:51 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bxpm.h"

int32_t	read_int32(unsigned char *str, int index)
{
	int32_t		res;

	res = str[index + 3] << 24
		| str[index + 2] << 16
		| str[index + 1] << 8
		| str[index] << 0;
	return (res);
}

uint32_t	read_color32(unsigned char *str, int index)
{
	uint32_t	res;

	res = str[index + 3] << 24
		| str[index + 2] << 16
		| str[index + 1] << 8
		| str[index + 0];
	return (res);
}

uint32_t	read_color24(unsigned char *str, int i)
{
	uint32_t	res;

	res = 255 << 24
		| str[i + 2] << 16
		| str[i + 1] << 8
		| str[i + 0] << 0;
	return (res);
}

int32_t	read_int16(unsigned char *str, int i)
{
	int32_t	res;

	res = str[i + 1] << 8
		| str[i];
	return (res);
}

unsigned int	bpp_clr(double byte, unsigned char *str, uint32_t bpp)
{
	if (bpp == 32)
		return (read_color32(str, byte));
	if (bpp == 24)
		return (read_color24(str, byte));
	return (0);
}
