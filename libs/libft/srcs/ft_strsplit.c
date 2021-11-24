/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 15:01:20 by nneronin          #+#    #+#             */
/*   Updated: 2021/07/06 09:33:24 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdio.h>

static void	wc_and_cc(char *str, char c, int *wc, int *cc)
{
	*cc = 0;
	*wc = 0;
	while (*str)
	{
		if (*str && *str != c)
		{
			*wc += 1;
			while (*str && *str != c)
			{
				str++;
				*cc += 1;
			}
		}
		else
			str++;
	}
}

static char	**split(char *str, char c, int wc, int cc)
{
	void		*arr;
	const char	**ptrs;
	char		*data;

	arr = malloc(8 * (wc + 1) + (cc + wc));
	if (!arr)
		return (NULL);
	ptrs = arr;
	data = arr + 8 * (wc + 1);
	while (*str)
	{
		if (*str != c)
		{
			*ptrs = data;
			while (*str && *str != c)
				*data++ = *str++;
			*data = '\0';
			data++;
			*++ptrs = NULL;
		}
		else
			str++;
	}
	return (arr);
}

char	**ft_strsplit_nik(const char *s, char c)
{
	int		wc;
	int		cc;
	char	*str;

	str = (char *)s;
	wc_and_cc(str, c, &wc, &cc);
	return (split(str, c, wc, cc));
}

// HERE STARTS MINE

int		ft_strwordcount(const char *str, char c)
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
			while (str[++i] == c);
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
	i = -1;
	arr = malloc(sizeof(char *) * (ft_strwordcount(str, c) + 1));
	prev_i = 0;
	i = -1;
	wc = -1;
	while (str[++i])
	{
		if (str[i] == c)
		{
			arr[++wc] = ft_strsub(str, prev_i, i - prev_i);
			while (str[++i] == c);
			prev_i = i;
		}
	}
	if (i - prev_i > 0)
		arr[++wc] = ft_strsub(str, prev_i, i - prev_i);
	arr[++wc] = NULL;
	return (arr);
}

/////// OLD START HERE

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
