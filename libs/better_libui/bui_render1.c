/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_render1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 16:17:36 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 16:18:59 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

static void	update_element_state(t_bui_element *elem)
{
	if (elem->show && hitbox_rect(elem->libui->mouse_x,
			elem->libui->mouse_y, elem->screen_pos))
	{
		if (elem->libui->mouse_down)
		{
			elem->was_clicked_last_frame = 1;
			if (elem->update_state)
				elem->state = ELEMENT_CLICK;
		}
		else if (elem->toggle != 1)
		{
			if (elem->update_state)
				elem->state = ELEMENT_HOVER;
			elem->was_hovered_last_frame = 1;
		}
	}
}

static void	event_handle_element(SDL_Window *win, t_bui_element *elem)
{
	elem->was_clicked_last_frame = 0;
	elem->was_hovered_last_frame = 0;
	if (elem->toggle == 1)
		elem->state = ELEMENT_CLICK;
	else
		elem->state = ELEMENT_DEFAULT;
	if (elem->libui->mouse_down == 0)
		elem->already_clicked = 0;
	if ((SDL_GetMouseFocus() == win))
		update_element_state(elem);
}

static void	update_element_screen_pos(t_bui_element *elem)
{
	if (!elem)
		return ;
	if (elem->type == TYPE_ELEMENT)
	{
		elem->screen_pos = elem->position;
		elem->screen_pos.x
			= ((t_bui_element *)elem->parent)->screen_pos.x + elem->position.x;
		elem->screen_pos.y
			= ((t_bui_element *)elem->parent)->screen_pos.y + elem->position.y;
	}
	else if (elem->type == TYPE_MENU)
	{
		elem->screen_pos = elem->position;
		elem->screen_pos.x = elem->position.x;
		elem->screen_pos.y = elem->position.y;
	}
}

static void	redo_element(t_bui_element *elem)
{
	if (elem->show != 0)
	{
		if (elem->update)
		{
			if (elem->clear)
				SDL_memset(elem->active_surface->pixels,
					0, elem->active_surface->h
					* elem->active_surface->pitch);
			SDL_BlitSurface(elem->surface[elem->state], NULL,
				elem->active_surface, NULL);
		}
		if (elem->text_surface)
		{
			SDL_BlitSurface(elem->text_surface, NULL,
				elem->active_surface, &(SDL_Rect){elem->text_x,
				elem->text_y, elem->text_surface->w,
				elem->text_surface->h});
		}
	}
}

void	update_element(t_bui_element *elem, SDL_Window *win)
{
	t_list	*child;

	if (elem == NULL)
		return ;
	update_element_screen_pos(elem);
	event_handle_element(win, elem);
	redo_element(elem);
	child = elem->children;
	while (child)
	{
		update_element(child->content, win);
		child = child->next;
	}
}
