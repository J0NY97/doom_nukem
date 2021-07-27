/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   play_menu2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 16:41:48 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 16:41:50 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

void	map_button(t_map_button_data data)
{
	char	*text;

	text = ft_strdup(data.str);
	ft_strremove(text, ".endless");
	add_to_list(data.list, new_map_button(data.parent, text, data.i, data.cap),
		sizeof(t_bui_element));
	ft_strdel(&text);
}

void	init_play_menu_map_buttons(t_ui_menu_play *menu, t_list *maps)
{
	t_list	*curr;
	int		endless_x;
	int		story_x;

	endless_x = 0;
	story_x = 0;
	curr = maps;
	while (curr)
	{
		if (ft_strendswith(curr->content, ".endless") == 0)
		{
			map_button((t_map_button_data){&menu->endless_maps,
				menu->endless_maps_view, curr->content,
				new_xywh(10, 10, menu->endless_maps_view->position.w,
					menu->endless_maps_view->position.h), endless_x++});
		}
		else if (ft_strendswith(curr->content, ".story") == 0)
		{
			map_button((t_map_button_data){&menu->story_maps, menu->story_view,
				curr->content, new_xywh(30, 30, menu->story_view->position.w,
					menu->story_view->position.h), story_x++});
		}
		curr = curr->next;
	}
}

void	endless_tab_init(t_ui_menu_play *play_menu)
{
	t_bui_element	**temp;
	t_xywh			c;

	temp = preset_tab_add(play_menu->play_tabs, "Endless");
	play_menu->endless_button = temp[0];
	play_menu->endless_view = temp[1];
	bui_remove_element_text(play_menu->endless_view);
	bui_set_element_color(play_menu->endless_button, 0xff505168);
	bui_set_element_color(play_menu->endless_view, 0xff505168);
	play_menu->endless_view->update_state = 0;
	free(temp);
	// Difficulty text elem
	c = new_xywh(25, 25, 100, 20);
	difficulty_drop_init(play_menu, c);
	// Divider
	c = new_xywh(c.x + c.w + c.x, 5, 1,
			play_menu->endless_view->active_surface->h - 10);
	bui_new_element(play_menu->endless_view, NULL, c);
	// Maps view
	c.x = c.x + c.w + 5;
	c.y = 5;
	c.w = play_menu->endless_view->position.w - c.x - 15;
	c.h = play_menu->endless_view->position.h - 10;
	play_menu->endless_maps_view
		= bui_new_element(play_menu->endless_view, NULL, c);
	play_menu->endless_maps_view->update_state = 0;
	bui_set_element_color(play_menu->endless_maps_view, 0xff505168);
	//
	play_menu->endless_maps = NULL;
}

void	story_tab_init(t_ui_menu_play *play_menu)
{
	t_bui_element	**temp;

	temp = preset_tab_add(play_menu->play_tabs, "Story");
	play_menu->story_button = temp[0];
	play_menu->story_view = temp[1];
	bui_remove_element_text(play_menu->story_view);
	bui_set_element_color(play_menu->story_button, 0xffB3C0A4);
	bui_set_element_color(play_menu->story_view, 0xff505168);
	play_menu->story_view->update_state = 0;
	free(temp);
	play_menu->story_maps = NULL;
}
