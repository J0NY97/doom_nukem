/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strdup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@stuent.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 11:58:56 by nneronin          #+#    #+#             */
/*   Updated: 2021/05/08 14:33:39 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*dest;
	int		x;
	int		y;

	y = 0;
	x = 0;
	while (src[y] != '\0')
		y++;
	dest = (char *)malloc(sizeof(char) * (y + 1));
	if (!dest)
		return (NULL);
	while (y != 0)
	{
		dest[x] = src[x];
		x++;
		y--;
	}
	dest[x] = '\0';
	return (dest);
}
