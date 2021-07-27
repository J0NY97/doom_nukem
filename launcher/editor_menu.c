/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_menu.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:56:19 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 11:56:20 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

t_ui_menu_editor	*editor_menu_init(t_bui_element *main_menu)
{
	t_ui_menu_editor	*menu;

	menu = ft_memalloc(sizeof(t_ui_menu_editor));
	menu->menu = new_main_menu_menu(main_menu, "Editor");
	return (menu);
}

void	editor_menu_events(t_launcher *launcher, t_ui_menu_editor *menu)
{
	char	*map_pressed;
	char	*arr[4];

	map_pressed = get_clicked_map_button(menu->map_buttons);
	if (map_pressed != NULL)
	{
		arr[0] = ft_sprintf("%s", ROOT_PATH"map_editor/doom_editor");
		arr[1] = ft_sprintf("%s", map_pressed);
		arr[2] = ft_sprintf("-l");
		arr[3] = NULL;
		ft_printf("%s %s",
			arr[0], arr[1]);
		bui_libui_quit(menu->menu->libui);
		free_launcher(launcher);
		system("leaks wolf3d");
		execv(arr[0], arr);
	}
}

void	init_editor_menu_map_buttons(t_ui_menu_editor *menu, t_list *maps)
{
	t_list	*curr;
	t_xywh	c;
	int		i;

	i = 0;
	c.x = 30;
	c.y = 50;
	c.w = menu->menu->position.w + 25;
	c.h = menu->menu->position.h;
	curr = maps;
	add_to_list(&menu->map_buttons,
		new_map_button(menu->menu, "new.endless", i++, c),
		sizeof(t_bui_element));
	add_to_list(&menu->map_buttons,
		new_map_button(menu->menu, "new.story", i++, c),
		sizeof(t_bui_element));
	while (curr)
	{
		add_to_list(&menu->map_buttons,
			new_map_button(menu->menu, curr->content, i, c),
			sizeof(t_bui_element));
		i++;
		curr = curr->next;
	}
}
