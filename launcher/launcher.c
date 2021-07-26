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
	t_fps			*fps;
	t_bui_libui		*libui;
	t_bui_window	*win;
	t_launcher		*launcher;

	fps = ft_memalloc(sizeof(t_fps));
	libui = bui_new_libui();
	win = window_init(libui);
	launcher = launcher_init(win);

	while (libui->run)
	{
		bui_event_handler_new(libui);
		main_menu_events(launcher, libui);
		bui_render_new(libui);
		bui_fps_func(fps);
		update_title_fps(win->win, fps);
	}
	// launcher_free(launcher);
	// free map_dir
	// free map_list
	bui_libui_quit(libui);
	return (0);
}
