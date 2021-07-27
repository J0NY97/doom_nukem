/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:59:26 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 12:02:41 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

void	render_all_children(t_bui_element *elem)
{
	t_list	*curr;
	t_list	*prev;

	curr = elem->children;
	if (curr && ((t_bui_element *)curr->content)->remove)
	{
		elem->children = curr->next;
		ft_lstdelone(&curr, &bui_element_free);
		curr = elem->children;
	}
	while (curr)
	{
		if (((t_bui_element *)curr->content)->remove)
		{
			prev->next = curr->next;
			ft_lstdelone(&curr, &bui_element_free);
		}
		else
		{
			bui_render_to_screen(curr->content);
			prev = curr;
		}
		curr = prev->next;
	}
}

void	bui_render_to_screen(t_bui_element *elem)
{
	if (!elem)
		return ;
	render_all_children(elem);
	if (elem->show != 0)
	{
		if (elem->type == TYPE_ELEMENT)
		{
			SDL_BlitSurface(elem->active_surface,
				&(SDL_Rect){0, 0,
				elem->position.w, elem->position.h},
				((t_bui_element *)elem->parent)->active_surface,
				&(SDL_Rect){elem->position.x, elem->position.y,
				elem->position.w, elem->position.h});
		}
		else
		{
			SDL_BlitSurface(elem->active_surface,
				&(SDL_Rect){0, 0,
				elem->position.w, elem->position.h},
				((t_bui_window *)elem->parent)->active_surface,
				&(SDL_Rect){elem->position.x, elem->position.y,
				elem->position.w, elem->position.h});
		}
	}
}

void	render_menu(t_bui_window *win)
{
	t_list	*curr_menu;
	t_list	*prev;

	curr_menu = win->menus;
	if (curr_menu && ((t_bui_element *)curr_menu->content)->remove)
	{
		win->menus = curr_menu->next;
		ft_lstdelone(&curr_menu, &bui_element_free);
		curr_menu = win->menus;
	}
	while (curr_menu)
	{
		if (((t_bui_element *)curr_menu->content)->remove)
		{
			prev->next = curr_menu->next;
			ft_lstdelone(&curr_menu, &bui_element_free);
		}
		else
		{
			bui_render_to_screen(curr_menu->content);
			prev = curr_menu;
		}
		curr_menu = prev->next;
	}
}

void	bui_render(t_bui_libui *libui)
{
	t_bui_window	*win;
	t_list			*curr_win;

	curr_win = libui->windows;
	while (curr_win)
	{
		win = curr_win->content;
		render_menu(win);
		curr_win = curr_win->next;
		if (win->update)
			SDL_UpdateWindowSurface(win->win);
	}
}
