/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_new.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/30 13:41:55 by nneronin          #+#    #+#             */
/*   Updated: 2021/07/06 09:34:36 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstadd_new(t_list **list, void *content, size_t content_size)
{
	t_list	*new;

	if (content == NULL)
		return (0);
	new = (t_list *)malloc(sizeof(*new));
	if (!new)
		return (0);
	new->content = content;
	new->content_size = content_size;
	ft_lstadd(list, new);
	return (1);
}
