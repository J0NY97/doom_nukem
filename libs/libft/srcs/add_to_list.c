/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 15:31:50 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/03 08:59:08 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

void	add_to_list(t_list **list, void *new_v, size_t size)
{
	t_list	*lst;

	lst = ft_lstnew(0, 0);
	lst->content = new_v;
	lst->content_size = size;
	if (*list == NULL)
		*list = lst;
	else
		ft_lstadd(list, lst);
}
