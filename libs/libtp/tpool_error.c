/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tpool_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/08 14:55:22 by nneronin          #+#    #+#             */
/*   Updated: 2021/07/30 16:07:53 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tpool.h"

int	tpool_error(int i)
{
	write(1, "ERROR tpool: ", 14);
	if (i == -1)
		write(1, "passed function errors, (0 == error).\n", 39);
	else if (i == -2)
		write(1, "pthread_create.\n", 17);
	else if (i == -3)
		write(1, "malloc tpool->thread.\n", 23);
	else if (i == -4)
		write(1, "create_task.\n", 14);
	else if (i == -5)
		write(1, "no function passed to tpool_add.\n", 34);
	return (0);
}
