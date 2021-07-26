/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 10:17:22 by jsalmi            #+#    #+#             */
/*   Updated: 2019/10/23 10:24:09 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

void	ft_lstdelone(t_list **alst, void (*del)(void *, size_t))
{
	if (!*alst)
		return ;
	if ((*alst)->content)
		del((*alst)->content, (*alst)->content_size);
	(*alst)->content = NULL;
	free(*alst);
	*alst = NULL;
}
