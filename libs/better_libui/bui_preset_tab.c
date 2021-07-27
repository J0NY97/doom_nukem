/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_preset_tab.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 14:06:06 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 14:06:07 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

t_preset_tab	*bui_new_tab_preset(t_bui_element *menu, char *text, t_xywh pos)
{
	t_preset_tab	*tab;

	tab = ft_memalloc(sizeof(t_preset_tab));
	tab->tabsystem = bui_new_element(menu, text, pos);
	tab->tabsystem->update_state = 0;
	tab->active = NULL;
	tab->tabs = NULL;
	tab->views = NULL;
	tab->tab_count = 0;
	return (tab);
}

void	tab_preset_free(void *preset, size_t size)
{
	t_preset_tab	*tab;
	t_list			*curr;
	t_list			*rruc;

	tab = preset;
	curr = tab->views;
	while (curr)
	{
		rruc = curr->next;
		ft_lstdelone(&curr, &dummy_free_er);
		curr = rruc;
	}
	curr = tab->tabs;
	while (curr)
	{
		rruc = curr->next;
		ft_lstdelone(&curr, &dummy_free_er);
		curr = rruc;
	}
	ft_memdel((void **)&tab);
}

t_bui_element	**preset_tab_add(t_preset_tab *tabsystem, char *text)
{
	t_bui_element	**tab;
	char			*view_text;
	int				small_gap;

	tab = ft_memalloc(sizeof(t_bui_element *) * 2);
	small_gap = tabsystem->tab_count * 5;
	tab[0] = bui_new_element(tabsystem->tabsystem, text, (t_xywh)
		{tabsystem->tab_count * 50 + small_gap, 0, 50, 20});
	view_text = ft_sprintf("%s view", text);
	tab[1] = bui_new_element(tabsystem->tabsystem, view_text, (t_xywh)
		{0, 20, tabsystem->tabsystem->position.w,
			tabsystem->tabsystem->position.h - 21});
	ft_strdel(&view_text);
	tabsystem->tab_count++;
	add_to_list(&tabsystem->tabs, tab[0], sizeof(t_bui_element));
	add_to_list(&tabsystem->views, tab[1], sizeof(t_bui_element));
	return (tab);
}

void	preset_tab_events(t_preset_tab *tabsystem)
{
	t_list		*tab;
	t_list		*view;
	t_bui_libui	*libui;
	int			found_this_frame;

	found_this_frame = 0;
	libui = tabsystem->tabsystem->libui;
	tab = tabsystem->tabs;
	view = tabsystem->views;
	while (tab && view)
	{
		if (bui_button(tab->content))
		{
			((t_bui_element *)view->content)->show = 1;
			found_this_frame = 1;
		}
		else if (found_this_frame == 1)
		{
			((t_bui_element *)view->content)->show = 0;
		}
		tab = tab->next;
		view = view->next;
	}
}
