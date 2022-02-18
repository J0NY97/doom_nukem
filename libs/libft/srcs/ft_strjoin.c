/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@stuent.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/21 15:02:14 by nneronin          #+#    #+#             */
/*   Updated: 2021/05/08 14:37:10 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char			*dest;
	unsigned int	i;

	i = 0;
	if (!s2)
		return (ft_strdup(s1));
	if (!s1)
		return (ft_strdup(s2));
	dest = (char *)malloc(sizeof(*dest) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (dest == NULL)
		return (NULL);
	while (*s1 != '\0')
		dest[i++] = *s1++;
	while (*s2 != '\0')
		dest[i++] = *s2++;
	dest[i] = '\0';
	return (dest);
}

/*
 * We join s1 and s2 into dest, dest should already be allocated, either heap
 *	or stack;
 * 'dest' should be atleast ft_strlen(s1) + ft_strlen(s2);
 */
char	*ft_b_strjoin(char *dest, const char *s1, const char *s2)
{
	int	s1_len;
	int	s2_len;

	s1_len = 0;
	while (s1[s1_len])
	{
		dest[s1_len] = s1[s1_len];
		s1_len++;
	}
	s2_len = 0;
	while (s2[s2_len])
	{
		dest[s1_len + s2_len] = s2[s2_len];
		s2_len++;
	}
	dest[s1_len + s2_len] = 0;
	return (dest);
}
