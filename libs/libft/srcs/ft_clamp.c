/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clamp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 14:10:50 by nneronin          #+#    #+#             */
/*   Updated: 2021/09/17 17:46:07 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static float	ft_fclamp2(float x, float min, float max)
{
	if (x < min)
		return (min);
	if (x > max)
		return (max);
	return (x);
}

float	ft_fclamp(float x, float a, float b)
{
	if (a > b)
		return (ft_fclamp2(x, b, a));
	else if (a < b)
		return (ft_fclamp2(x, a, b));
	else
		return (a);
}

static int	ft_clamp2(int x, int min, int max)
{
	if (x < min)
		return (min);
	if (x > max)
		return (max);
	return (x);
}

int	ft_clamp(int x, int a, int b)
{
	if (a > b)
		return (ft_clamp2(x, b, a));
	else if (a < b)
		return (ft_clamp2(x, a, b));
	else
		return (a);
}
