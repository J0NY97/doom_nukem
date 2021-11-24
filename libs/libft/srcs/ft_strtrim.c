/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 12:40:54 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/03 08:24:30 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

/*
char	*ft_strtrim(char const *s)
{
	int		x;
	int		y;
	int		temp;
	char	*asd;

	x = 0;
	y = 0;
	while (s[x] == ' ' || s[x] == '\n' || s[x] == '\t')
		x++;
	temp = x;
	if (s[x] != '\0')
	{
		x = ft_strlen(s);
		y = x - temp;
		x--;
	}
	while (s[x] == ' ' || s[x] == '\n' || s[x] == '\t')
	{
		y--;
		x--;
	}
	asd = ft_strnew(y);
	ft_strncpy(asd, (s + temp), y);
	return (asd);
}
*/

char	*ft_strtrim(char const *str)
{
	int		i;
	int		j;
	int		total;
	char	*final;

	i = -1;
	while (str[++i] && ft_isspace(str[i]));
	j = ft_strlen(str);
	while (str[--j] && ft_isspace(str[j]));
	total = j - i + 1;
	if (total <= 0)
		return (NULL);
	final = ft_memalloc(sizeof(char) * (total + 1));
	ft_strncpy(final, str + i, total);
	final[total] = '\0';
	return (final);
}
