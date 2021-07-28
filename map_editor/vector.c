/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:44:45 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:44:46 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

int	vector_compare(t_vector v1, t_vector v2)
{
	if ((int)v1.x == (int)v2.x
		&& (int)v1.y == (int)v2.y
		&& (int)v1.z == (int)v2.z)
		return (1);
	return (0);
}

int	vector_is_empty(t_vector v)
{
	return (vector_compare(v, (t_vector){0, 0, 0}));
}

int	vector_in_wall(t_vector v, t_wall *vec)
{
	if (vector_compare(v, vec->orig->pos)
		|| vector_compare(v, vec->dest->pos))
		return (1);
	return (0);
}
