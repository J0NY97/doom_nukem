/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:56:39 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/29 14:11:41 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "launcher.h"

void	update_title_fps(SDL_Window *win, t_fps *fps)
{
	char	*str;

	str = ft_sprintf("Launcher [FPS: %d]", fps->fps);
	SDL_SetWindowTitle(win, str);
	ft_strdel(&str);
}

t_bui_window	*window_init(t_bui_libui *libui)
{
	t_bui_window	*win;

	win = bui_new_window(libui, "LAUNCHER",
			(t_xywh){100, 100, 1280, 720}, 0);
	bui_set_window_icon(win, ICON_PATH"DND.bmp");
	SDL_RaiseWindow(win->win);
	return (win);
}

int	main(void)
{
	t_bui_libui		*libui;
	t_bui_window	*win;
	t_launcher		*launcher;

	libui = bui_new_libui();
	win = window_init(libui);
	launcher = launcher_init(win);
	while (libui->run)
	{
		bui_event_handler(libui);
		main_menu_events(launcher, libui);
		bui_render(libui);
		bui_fps_func(launcher->fps);
		update_title_fps(win->win, launcher->fps);
	}
	bui_libui_quit(libui);
	free_launcher(launcher);
	system("leaks wolf3d");
	return (0);
}
