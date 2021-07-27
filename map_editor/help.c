/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/26 15:54:48 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

Uint32	random_color(void)
{
	Uint32		color;

	color = rgba_to_hex((t_rgba){
			.a = 255, .r = rand() % 255,
			.g = rand() % 255, .b = 245});
	return (color);
}

int	pointer_compare(void *p1, void *p2)
{
	return (p1 == p2);
}

int	in_list(void *pointer, t_list *list)
{
	t_list	*curr;

	curr = list;
	while (curr)
	{
		if (pointer == curr->content)
			return (1);
		curr = curr->next;
	}
	return (0);
}

t_list	*get_nth_from_list(t_list **list, int index)
{
	int		count;
	t_list	*curr;

	curr = *list;
	count = 0;
	while (curr)
	{
		if (count == index)
			return (curr);
		count++;
		curr = curr->next;
	}
	return (NULL);
}

void	remove_from_list_if_with(t_list **list, void *s,
		int (*cmp)(void *, void *), void (*del)(void *, size_t))
{
	t_list	*curr;
	t_list	*prev;

	curr = *list;
	if (curr == NULL)
		return ;
	if (cmp(curr->content, s))
	{
		*list = curr->next;
		ft_lstdelone(&curr, del);
	}
	else
	{
		while (curr)
		{
			if (cmp(curr->content, s))
			{
				prev->next = curr->next;
				ft_lstdelone(&curr, del);
			}
			else
				prev = curr;
			curr = prev->next;
		}
	}
}
