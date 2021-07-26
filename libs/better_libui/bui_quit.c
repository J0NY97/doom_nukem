#include "better_libui.h"

void	dummy_free_er(void *dum, size_t my)
{
	(void)dum;
	(void)my;
}

void	bui_element_free(void *element, size_t size)
{
	t_bui_element	*elem;

	elem = element;
	if (!elem)
		return ;
	ft_lstdel(&elem->children, &bui_element_free);
	SDL_FreeSurface(elem->active_surface);
	SDL_FreeSurface(elem->surface[0]);
	SDL_FreeSurface(elem->surface[1]);
	SDL_FreeSurface(elem->surface[2]);
	if (elem->text)
		ft_strdel(&elem->text);
	if (elem->text_surface)
		SDL_FreeSurface(elem->text_surface);
	if (elem->id)
		ft_strdel(&elem->id);
	if (elem->font)
		TTF_CloseFont(elem->font);
	if (elem->font_name)
		ft_strdel(&elem->font_name);
	elem->libui = NULL;
	elem->parent = NULL;
	elem->children = NULL;
	ft_memdel(&element);
}

void	bui_window_free(void *window, size_t size)
{
	t_bui_window	*win;

	win = window;
	ft_lstdel(&win->menus, &bui_element_free);
	if (win->free_window_surface)
		SDL_FreeSurface(win->active_surface);
	if (win->free_window)
		SDL_DestroyWindow(win->win);
	win->libui = NULL;
	win->menus = NULL;
	ft_memdel((void **)&win);
}

void	bui_libui_free(t_bui_libui *libui)
{
	libui->windows = NULL;
	libui->elements = NULL;
	ft_strdel(&libui->last_key);
	free(libui);
}

void	bui_libui_quit(t_bui_libui *libui)
{
	ft_printf("ELements: %d\n", ft_lstlen(libui->elements));
	ft_lstdel(&libui->windows, &bui_window_free);
	ft_lstdel(&libui->elements, &dummy_free_er);
	libui->last_key = NULL;
	ft_memdel((void **)&libui);
}
