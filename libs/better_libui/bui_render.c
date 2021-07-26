#include "better_libui.h"

static void	event_handle_element(SDL_Window *win, t_bui_element *elem)
{
	elem->was_clicked_last_frame = 0;
	elem->was_hovered_last_frame = 0;
	if (elem->toggle == 1)
		elem->state = ELEMENT_CLICK;
	else
		elem->state = ELEMENT_DEFAULT;
	if (elem->libui->mouse_down == 0)
		elem->already_clicked = 0;
	if ((SDL_GetMouseFocus() == win))
	{
		if (elem->show && hitbox_rect(elem->libui->mouse_x,
				elem->libui->mouse_y, elem->screen_pos))
		{
			if (elem->libui->mouse_down)
			{
				elem->was_clicked_last_frame = 1;
				if (elem->update_state)
					elem->state = ELEMENT_CLICK;
			}
			else if (elem->toggle != 1)
			{
				if (elem->update_state)
					elem->state = ELEMENT_HOVER;
				elem->was_hovered_last_frame = 1;
			}
		}
	}
}

static void	update_element_screen_pos(t_bui_element *elem)
{
	if (!elem)
		return ;
	if (elem->type == TYPE_ELEMENT)
	{
		elem->screen_pos = elem->position;
		elem->screen_pos.x
			= ((t_bui_element *)elem->parent)->screen_pos.x + elem->position.x;
		elem->screen_pos.y
			= ((t_bui_element *)elem->parent)->screen_pos.y + elem->position.y;
	}
	else if (elem->type == TYPE_MENU)
	{
		elem->screen_pos = elem->position;
		elem->screen_pos.x = elem->position.x;
		elem->screen_pos.y = elem->position.y;
	}
}

static void	redo_element(t_bui_element *elem)
{
	if (elem->show != 0)
	{
		if (elem->update)
		{
			if (elem->clear)
				SDL_memset(elem->active_surface->pixels,
					0, elem->active_surface->h
					* elem->active_surface->pitch);
			SDL_BlitSurface(elem->surface[elem->state], NULL,
				elem->active_surface, NULL);
		}
		if (elem->text_surface)
		{
			SDL_BlitSurface(elem->text_surface, NULL,
				elem->active_surface, &(SDL_Rect){elem->text_x,
				elem->text_y, elem->text_surface->w,
				elem->text_surface->h});
		}
	}
}

static void	update_element(t_bui_element *elem, SDL_Window *win)
{
	t_list	*child;

	if (elem == NULL)
		return ;
	update_element_screen_pos(elem);
	event_handle_element(win, elem);
	redo_element(elem);
	child = elem->children;
	while (child)
	{
		update_element(child->content, win);
		child = child->next;
	}
}

// UP from here all in the same file.
/////////////////////

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

void	bui_update_elements(t_bui_libui *libui)
{
	t_bui_window	*win;
	t_list			*curr_win;
	t_list			*curr_menu;

	curr_win = libui->windows;
	while (curr_win)
	{
		win = curr_win->content;
		if (1) //(SDL_GetWindowFlags(win->win) & SDL_WINDOW_SHOWN))
		{
			curr_menu = win->menus;
			while (curr_menu)
			{
				update_element(curr_menu->content, win->win);
				curr_menu = curr_menu->next;
			}
		}
		curr_win = curr_win->next;
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

void	bui_render_new(t_bui_libui *libui)
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

void	get_mouse_states(t_bui_libui *libui)
{
	int	rx;
	int	ry;

	libui->last_global_mouse_x = libui->global_mouse_x;
	libui->last_global_mouse_y = libui->global_mouse_y;
	SDL_GetGlobalMouseState(&libui->global_mouse_x, &libui->global_mouse_y);
	libui->last_mouse_x = libui->mouse_x;
	libui->last_mouse_y = libui->mouse_y;
	SDL_GetMouseState(&libui->mouse_x, &libui->mouse_y);
	libui->last_relative_mouse_x = libui->relative_mouse_x;
	libui->last_relative_mouse_y = libui->relative_mouse_y;
	SDL_GetRelativeMouseState(&rx, &ry);
	libui->relative_mouse_x += rx;
	libui->relative_mouse_y += ry;
	if (libui->mouse_down
		&& (libui->mouse_x != libui->last_mouse_x
			|| libui->mouse_y != libui->last_mouse_y))
		libui->mouse_drag = 1;
	else
		libui->mouse_drag = 0;
}

void	mouse_update(t_bui_libui *libui, SDL_Event e)
{
	if (e.button.type == SDL_MOUSEBUTTONDOWN)
	{
		libui->mouse_down = 1;
		libui->mouse_down_last_frame = 1;
		libui->mouse_click_x = libui->mouse_x;
		libui->mouse_click_y = libui->mouse_y;
		mouse_key_was(libui, e.button.button, 1);
	}
	else if (e.button.type == SDL_MOUSEBUTTONUP)
	{
		libui->mouse_down = 0;
		mouse_key_was(libui, e.button.button, 0);
	}
	else if (e.type == SDL_MOUSEWHEEL)
	{
		libui->mouse_wheel_x = e.wheel.x;
		libui->mouse_wheel_y = e.wheel.y;
	}
}

void	bui_event_handler_new(t_bui_libui *libui)
{
	SDL_Event	e;

	libui->mouse_down_last_frame = 0;
	libui->last_key = NULL;
	libui->mouse_wheel_x = 0;
	libui->mouse_wheel_y = 0;
	e = libui->event;
	while (SDL_PollEvent(&e))
	{
		if ((e.type == SDL_WINDOWEVENT
				&& e.window.event == SDL_WINDOWEVENT_CLOSE)
			|| key_pressed(libui, KEY_ESCAPE))
			libui->run = 0;
		else if (e.button.type == SDL_MOUSEBUTTONDOWN)
			mouse_update(libui, e);
		else if (e.button.type == SDL_MOUSEBUTTONUP)
			mouse_update(libui, e);
		else if (e.key.type == SDL_KEYDOWN)
			key_was(libui, e.key.keysym, 1);
		else if (e.key.type == SDL_KEYUP)
			key_was(libui, e.key.keysym, 0);
		else if (e.type == SDL_MOUSEWHEEL)
			mouse_update(libui, e);
	}
	get_mouse_states(libui);
	bui_update_elements(libui);
}
