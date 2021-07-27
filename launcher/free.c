/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:57:03 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 11:57:03 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

void	free_settings_menu(t_ui_menu_settings *menu)
{
	free(menu->fov_slider);
	free(menu->mouse_x_slider);
	free(menu->mouse_y_slider);
	free(menu->texture_scale_slider);
	dropdown_preset_free(menu->resolution_drop, 0);
	free(menu);
}

void	free_char_pointer(void *content, size_t size)
{
	char	*str;

	str = content;
	(void)size;
	ft_strdel(&str);
}

void	free_play_menu(t_ui_menu_play *menu)
{
	tab_preset_free(menu->play_tabs, 0);
	dropdown_preset_free(menu->difficulty_dropdown, 0);
	ft_lstdel(&menu->story_maps, &dummy_free_er);
	ft_lstdel(&menu->endless_maps, &dummy_free_er);
	free(menu);
}

void	free_editor_menu(t_ui_menu_editor *menu)
{
	ft_lstdel(&menu->map_buttons, &dummy_free_er);
	free(menu);
}

void	free_launcher(t_launcher *launcher)
{
	free(launcher->fps);
	free_settings_menu(launcher->settings_menu);
	free_play_menu(launcher->play_menu);
	free_editor_menu(launcher->editor_menu);
	ft_lstdel(&launcher->main_menu_buttons, &dummy_free_er);
	ft_strdel(&launcher->map_dir);
	ft_lstdel(&launcher->map_list, &free_char_pointer);
	free(launcher);
}
