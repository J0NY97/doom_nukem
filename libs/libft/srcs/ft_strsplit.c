/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 15:01:20 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/08 15:48:39 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_strwordcount(const char *str, char c)
{
	int	i;
	int	count;

	i = -1;
	count = 1;
	while (str[++i])
	{
		if (str[i] == c)
		{
			count++;
			while (str[++i] == c)
				;
		}
	}
	return (count);
}

char	**ft_strsplit_old(const char *str, char c)
{
	char	**arr;
	int		i;
	int		prev_i;
	int		wc;

	if (!str)
		return (NULL);
	arr = malloc(sizeof(char *) * (ft_strwordcount(str, c) + 1));
	prev_i = 0;
	i = -1;
	wc = -1;
	while (str[++i])
	{
		if (str[i] == c)
		{
			arr[++wc] = ft_strsub(str, prev_i, i - prev_i);
			while (str[++i] == c)
				;
			prev_i = i;
		}
	}
	if (i - prev_i > 0)
		arr[++wc] = ft_strsub(str, prev_i, i - prev_i);
	arr[++wc] = NULL;
	return (arr);
}
