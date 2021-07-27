/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_edit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 14:04:42 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 14:04:43 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

void	bui_element_resize(t_bui_element *element, t_xywh coord)
{
	if (element->active_surface)
		SDL_FreeSurface(element->active_surface);
	if (element->surface[0])
		SDL_FreeSurface(element->surface[0]);
	if (element->surface[1])
		SDL_FreeSurface(element->surface[1]);
	if (element->surface[2])
		SDL_FreeSurface(element->surface[2]);
	element->surface[0] = create_surface(coord.w, coord.h);
	element->surface[1] = create_surface(coord.w, coord.h);
	element->surface[2] = create_surface(coord.w, coord.h);
	bui_set_element_color(element, element->color);
	element->active_surface = create_surface(coord.w, coord.h);
	element->position = coord;
}

void	bui_set_element_list_color(t_list *list, Uint32 color)
{
	t_list	*curr;

	curr = list;
	while (curr)
	{
		bui_set_element_color(curr->content, color);
		curr = curr->next;
	}
}
