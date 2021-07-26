#include "better_libui.h"

t_bui_window	*bui_new_window(
		t_bui_libui *libui, char *name, t_xywh pos, Uint32 flags)
{
	t_bui_window	*win;
	SDL_Window		*window;

	window = SDL_CreateWindow(name, pos.x, pos.y, pos.w, pos.h, flags);
	win = bui_new_window_from_window(libui, window, NULL);
	win->position = pos;
	win->free_window = 1;
	win->free_window_surface = 1;
	return (win);
}

// Creates a bui window from a sdl window.
t_bui_window	*bui_new_window_from_window(
		t_bui_libui *libui, SDL_Window *window, SDL_Surface *surface)
{
	t_bui_window	*win;

	win = malloc(sizeof(t_bui_window));
	win->win = window;
	SDL_GetWindowPosition(win->win, &win->position.x, &win->position.y);
	SDL_GetWindowSize(win->win, &win->position.w, &win->position.h);
	win->free_window_surface = 0;
	win->free_window = 0;
	if (!surface)
	{
		win->free_window_surface = 1;
		win->active_surface = SDL_GetWindowSurface(win->win);
	}
	else
		win->active_surface = surface;
	win->menus = NULL;
	win->libui = libui;
	win->update = 1;
	win->color = 0xff000000;
	add_to_list(&libui->windows, win, sizeof(t_bui_window));
	return (win);
}

void	bui_set_window_flags(t_bui_window *win, Uint32 flags)
{
	if (flags & BUI_WINDOW_DONT_UPDATE)
		win->update = 0;
	else if (flags & BUI_WINDOW_UPDATE)
		win->update = 1;
}

void	bui_set_window_color(t_bui_window *win, Uint32 color)
{
	win->color = color;
	SDL_FillRect(win->active_surface,
		&(SDL_Rect){0, 0, win->position.w, win->position.h},
		win->color);
}

void	bui_set_window_icon(t_bui_window *window, char *image_path)
{
	SDL_Surface	*icon;

	icon = load_image(image_path);
	if (!icon)
	{
		ft_printf("[bui_set_window_icon] Window icon %s"\
			" couldn\'t be set.\n", image_path);
		return ;
	}
	SDL_SetWindowIcon(window->win, icon);
	SDL_FreeSurface(icon);
}
