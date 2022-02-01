/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/16 15:01:20 by nneronin          #+#    #+#             */
/*   Updated: 2022/02/01 13:42:00 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	fill_elem_pos(int *elem_pos, char *str, char delim)
{
	int	first;
	int	second;
	int	i;

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

int	get_elem_amount(const char *str, char delim)
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
	fill_elem_pos(elem_pos, (char *)str, delim);
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
