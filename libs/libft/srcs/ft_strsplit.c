/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 15:01:20 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/09 12:38:55 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
static int	ft_wordlen(char const *str, char c, int x)
{
	int	i;

	i = 0;
	while (str[x] != c && str[x])
	{
		x++;
		i++;
	}
	return (i);
}

char	**ft_strsplit(char const *s, char c)
{
	char	**arr;
	int		x[3];

	x[1] = 0;
	x[0] = 0;
	if (!s)
		return (NULL);
	arr = ft_memalloc(sizeof(char *) * (ft_wordcount(s, c) + 1));
	while (s[x[0]])
	{
		x[2] = 0;
		if (s[x[0]] != c && s[x[0]])
		{
			arr[x[1]] = ft_memalloc(ft_wordlen(s, c, x[0]) + 1);
			ft_strncpy(arr[x[1]], (s + x[0]), ft_wordlen(s, c, x[0]));
			x[2] += ft_wordlen(s, c, x[0]);
			x[0] += ft_wordlen(s, c, x[0]);
			arr[x[1]][x[2]] = '\0';
			x[1]++;
		}
		else
			x[0]++;
	}
	arr[x[1]] = 0;
	return (arr);
}
*/

void	fill_elem_pos(int *elem_pos, char *str, char delim)
{
	int	first;
	int	second;
	int i;

	first = 0;
	second = 0;
	i = 0;
	while (1)
	{
		while (str[second] && str[second] == delim)
			second++;
		if (!str[second])
			break ;
		first = second;
		while (str[second] && str[second] != delim)
			second++;
		elem_pos[i + 0] = first;
		elem_pos[i + 1] = second - first;
		i += 2;
	}
}

int		get_elem_amount(const char *str, char delim)
{
	int	amount;
	int	second;

	if (!str)
		return (0);
	amount = 0;
	second = 0;
	while (1)
	{
		while (str[second] && str[second] == delim)
			second++;
		if (!str[second])
			break ;
		while (str[second] && str[second] != delim)
			second++;
		amount++;
	}
	return (amount);
}

char	**ft_strsplit(char const *str, char delim)
{
	char	**final;
	int		i;
	int		elem_count;
	int		*elem_pos;
	int		amount;

	i = 0;
	elem_count = -1;
	amount = get_elem_amount(str, delim) * 2;
	elem_pos = malloc(sizeof(int) * amount);
	fill_elem_pos(elem_pos, (char *)str, delim); // this is unnessecary. we could do everything, we do in this function, in the while loop down below.
	if (!elem_pos || amount == 0)
		return (NULL);
	final = malloc(sizeof(char *) * ((amount / 2) + 1));
	while (i < amount)
	{
		final[++elem_count] = ft_strsub(str, elem_pos[i], elem_pos[i + 1]);
		i += 2;
	}
	final[++elem_count] = NULL;
	free(elem_pos);
	return (final);
}