/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 10:09:19 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/11 10:09:20 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_arrjoin(char **to_this, char **from_this)
{
	char	**arr;
	int		total;
	int		i;
	int		j;

	i = 0;
	j = 0;
	arr = NULL;
	total = ft_arrlen(to_this) + ft_arrlen(from_this) + 1;
	if (total  == 1)
		return (NULL);
	arr = ft_memalloc(sizeof(char *) * total);
	while (to_this && to_this[i])
	{
		arr[i] = ft_strdup(to_this[i]);
		i++;
	}
	while (from_this && from_this[j])
	{
		arr[i + j] = ft_strdup(from_this[j]);
		j++;
	}
	return (arr);
}
