#include "better_libui.h"

t_bui_libui	*bui_new_libui(void)
{
	t_bui_libui	*libui;

	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
	libui = ft_memalloc(sizeof(t_bui_libui));
	libui_events_clear(libui);
	libui->last_key = NULL;
	libui->run = 1;
	libui->windows = NULL;
	libui->elements = NULL;
	return (libui);
}

void	bui_set_icon(SDL_Window *window, char *dir)
{
	SDL_Surface	*icon;

	icon = load_image(dir);
	if (!icon)
	{
		ft_putstr("[bui_set_icon] window icon couldnt be set.\n");
		return ;
	}
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);
}
