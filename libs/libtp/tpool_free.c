/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tpool_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 10:36:46 by nneronin          #+#    #+#             */
/*   Updated: 2021/05/08 14:56:24 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tpool.h"

void	tpool_memdel(void **ap)
{
	if (ap == NULL)
		return ;
	free(*ap);
	*ap = NULL;
}

void	free_task_list(t_task *tasks)
{
	t_task	*tmp;

	while (tasks)
	{
		tmp = tasks->next;
		if (tasks)
		{
			free(tasks);
			tasks = NULL;
		}
		tasks = tmp;
	}
}

int	free_tpool(t_tpool *tpool)
{
	int		i;
	t_task	*task;

	i = -1;
	if (!tpool->threads)
		return (0);
	pthread_mutex_lock(&tpool->mutex);
	task = tpool->tasks;
	free_task_list(tpool->tasks);
	tpool->stop = 1;
	pthread_cond_broadcast(&tpool->task_cond);
	pthread_mutex_unlock(&tpool->mutex);
	while (++i < tpool->nb_threads)
		if (pthread_join(tpool->threads[i], NULL))
			write(1, "Pthread_join error!\n", 20);
	tpool_memdel((void **)&tpool->threads);
	pthread_mutex_destroy(&tpool->mutex);
	pthread_cond_destroy(&tpool->task_cond);
	pthread_cond_destroy(&tpool->main_cond);
	return (0);
}
