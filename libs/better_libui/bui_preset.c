/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_preset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:59:15 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 11:59:16 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

t_preset_menu	*bui_new_menu_preset(
		t_bui_window *parent, char *title, t_xywh pos)
{
	t_preset_menu	*preset;

	preset = malloc(sizeof(t_preset_menu));
	memset(preset, 0, sizeof(t_preset_menu));
	preset->menu = bui_new_menu(parent, title, pos);
	preset->quit = bui_new_element(preset->menu, "x",
			(t_xywh){preset->menu->position.w - 20, 0, 20, 20});
	preset->title_bar = bui_new_element(preset->menu, title,
			(t_xywh){0, 0, preset->menu->position.w
			- preset->quit->position.w, preset->quit->position.h});
	return (preset);
}

void	preset_menu_events(t_preset_menu *menu)
{
	if (bui_button(menu->quit))
		menu->menu->show = 0;
	if (menu->menu->libui->mouse_drag
		&& bui_button_allow_duplicate(menu->title_bar))
	{
		menu->menu->position.x += menu->menu->libui->mouse_x
			- menu->menu->libui->last_mouse_x;
		menu->menu->position.x
			= ft_clamp(menu->menu->position.x,
				-menu->title_bar->position.w
				+ menu->title_bar->position.h,
				((t_bui_window *)menu->menu->parent)->position.w
				- menu->title_bar->position.h);
		menu->menu->position.y
			+= menu->menu->libui->mouse_y
			- menu->menu->libui->last_mouse_y;
		menu->menu->position.y
			= ft_clamp(menu->menu->position.y, 0,
				((t_bui_window *)menu->menu->parent)->position.h
				- menu->title_bar->position.h);
	}
}

t_preset_console	*bui_new_console_preset(t_bui_window *win, t_xywh pos)
{
	t_preset_console	*console;
	t_bui_element		*menu;
	t_xywh				view_pos;

	console = (t_preset_console *)ft_memalloc(sizeof(t_preset_console));
	console->menu = bui_new_menu_preset(win, "Console", pos);
	menu = console->menu->menu;
	console->carrot = bui_new_element(menu, " >:", (t_xywh){0,
			menu->position.h - 20, 20, 20});
	console->send = bui_new_element(menu, "send", (t_xywh){
			menu->position.w - 50, menu->position.h - 20, 50, 20});
	console->input = bui_new_element(menu, " ", (t_xywh){
			console->carrot->position.w, console->carrot->position.y,
			menu->position.w - (console->send->position.w
				+ console->carrot->position.w), console->carrot->position.h});
	view_pos.x = 1;
	view_pos.y = console->menu->title_bar->position.y
		+ console->menu->title_bar->position.h + 1;
	view_pos.w = menu->position.w - (view_pos.x * 2);
	view_pos.h = menu->position.h - view_pos.y - console->input->position.h - 1;
	console->view = bui_new_element(menu, "", view_pos);
	console_log(console, "4 8 15 16 23 42");
	return (console);
}

void	preset_console_events(t_preset_console *console)
{
	t_bui_libui	*libui;

	libui = console->menu->menu->libui;
	preset_menu_events(console->menu);
	if (bui_button(console->send)
		|| (console->input->toggle && key_pressed(libui, KEY_RETURN)))
	{
		console_log(console, console->input->text);
		if (console->latest_command)
			ft_strdel(&console->latest_command);
		console->latest_command = ft_strdup(console->input->text);
		bui_change_element_text(console->input, " ");
	}
	bui_input(console->input);
	if (libui->mouse_wheel_y != 0)
		console->view->text_y += libui->mouse_wheel_y
			* TTF_FontLineSkip(console->view->font);
}

t_preset_slider	*bui_new_slider_preset(t_bui_element *menu, t_xywh pos)
{
	t_preset_slider	*slider;

	slider = ft_memalloc(sizeof(t_preset_slider));
	slider->slider = bui_new_element(menu, " ", pos);
	slider->button = bui_new_element(slider->slider, " ",
			(t_xywh){0, 0, 20, 20});
	bui_set_element_color(slider->button, 0xffef476f);
	bui_set_slider_values(slider, 50, 0, 100);
	return (slider);
}

void	bui_set_slider_values(
		t_preset_slider *slider, int value, int min, int max)
{
	char	*slide_str;

	slider->value = value;
	slider->min = min;
	slider->max = max;
	slider->button->position.x
		= bui_set_slider_value(value, min,
			max, slider->slider->position.w);
	slide_str = ft_itoa(slider->value);
	bui_set_element_text(slider->button, slide_str,
		slider->button->text_x, slider->button->text_y);
	bui_center_element_text(slider->button);
	ft_strdel(&slide_str);
}

void	preset_slider_events(t_preset_slider *slider)
{
	t_bui_libui	*libui;
	char		*slide_str;
	int			half_butt;

	libui = slider->slider->libui;
	if (bui_button_allow_duplicate(slider->button)
		|| bui_button(slider->slider))
	{
		half_butt = slider->button->position.w * 0.5f;
		slider->button->position.x = ft_clamp(libui->mouse_x
				- slider->slider->screen_pos.x - half_butt, 0,
				slider->slider->position.w - slider->button->position.w);
		slider->value = bui_get_slider_value(slider->min, slider->max,
				slider->button->position.x, slider->slider->position.w
				- slider->button->position.w);
		slide_str = ft_itoa(slider->value);
		bui_set_element_text(slider->button, slide_str,
			slider->button->text_x, slider->button->text_y);
		bui_center_element_text(slider->button);
		ft_strdel(&slide_str);
	}
}

t_preset_scrollbar	*bui_new_scrollbar_preset(
		t_bui_element *menu, t_xywh pos, t_bui_element *target)
{
	t_preset_scrollbar	*scrollbar;

	scrollbar = ft_memalloc(sizeof(t_preset_scrollbar));
	scrollbar->scrollbar = bui_new_element(menu, "scrollbar", pos);
	scrollbar->button = bui_new_element(scrollbar->scrollbar, NULL,
			(t_xywh){1, 1, pos.w - 2, 20});
	bui_set_element_color(scrollbar->button, 0xffef476f);
	scrollbar->button->position.y
		= bui_set_slider_value(0, 0, target->position.h,
			scrollbar->scrollbar->position.h);
	scrollbar->target = target;
	scrollbar->min = target->position.y;
	scrollbar->max = target->position.y + target->position.h;
	scrollbar->value = target->position.y;
	return (scrollbar);
}

void	preset_scrollbar_events(t_preset_scrollbar *scrollbar)
{
	t_bui_libui	*libui;
	t_list		*curr;
	int			half_butt;

	libui = scrollbar->scrollbar->libui;
	if (bui_button_allow_duplicate(scrollbar->button)
		|| bui_button(scrollbar->scrollbar))
	{
		half_butt = scrollbar->button->position.h * 0.5f;
		scrollbar->button->position.y = ft_clamp(libui->mouse_y
				- scrollbar->scrollbar->screen_pos.y - half_butt, 0 - half_butt,
				scrollbar->scrollbar->position.h);
		scrollbar->value = bui_get_slider_value(
				scrollbar->min, scrollbar->max,
				scrollbar->button->position.y + half_butt,
				scrollbar->scrollbar->position.h);
		ft_printf("Value: %d\n", scrollbar->value);
	}
	else if (scrollbar->target->was_hovered_last_frame
		&& libui->mouse_wheel_y != 0)
		scrollbar->value += libui->mouse_wheel_y * 15;
	scrollbar->target->position.y = scrollbar->value;
}

void	console_log(t_preset_console *console, char *text)
{
	char	*str;

	str = ft_sprintf("%s>: %s\n", console->view->text, text);
	bui_change_element_text(console->view, str);
	ft_strdel(&str);
	console->lines++;
	console->view->text_y = console->view->position.h
		- (console->lines * TTF_FontLineSkip(console->view->font));
}

t_preset_dropdown	*bui_new_dropdown_preset(
		t_bui_element *menu, char *text, t_xywh pos)
{
	t_preset_dropdown	*drop;

	drop = ft_memalloc(sizeof(t_preset_dropdown));
	drop->drop = bui_new_element(menu, text, pos);
	drop->arrow = bui_new_element(drop->drop, text,
			(t_xywh){pos.w - 20, 0, pos.h, 20});
	bui_set_element_text(drop->arrow, "<", 5, 2);
	drop->bass = bui_new_element(menu, "view", (t_xywh){
			drop->drop->position.x,
			drop->drop->position.y + drop->drop->position.h,
			drop->drop->position.w,
			drop->drop->position.h});
	drop->bass->show = 0;
	drop->elements = NULL;
	drop->active = NULL;
	drop->count = 0;
	return (drop);
}

void	dropdown_preset_free(void *preset, size_t size)
{
	t_preset_dropdown	*drop;

	drop = preset;
	ft_lstdel(&drop->elements, &dummy_free_er);
	ft_memdel((void **)&drop);
}

t_bui_element	*preset_dropdown_add_element(
		t_preset_dropdown *dropdown, char *text)
{
	t_bui_element	*elem;
	t_xywh			new_pos;

	elem = bui_new_element(dropdown->bass, text, (t_xywh){
			0, dropdown->drop->position.h * (dropdown->count),
			dropdown->bass->position.w,
			dropdown->drop->position.h});
	dropdown->count++;
	new_pos = dropdown->bass->position;
	new_pos.h = dropdown->drop->position.h * (dropdown->count);
	bui_element_resize(dropdown->bass, new_pos);
	add_to_list(&dropdown->elements, elem, sizeof(t_bui_element));
	return (elem);
}

int	preset_dropdown_events(t_preset_dropdown *dropdown)
{
	bui_button_toggle(dropdown->drop);
	dropdown->bass->show = dropdown->drop->toggle;
	bui_change_element_text(dropdown->arrow, "<");
	if (dropdown->bass->show)
	{
		bui_change_element_text(dropdown->arrow, "^");
		if (only_one_button_toggled_at_a_time(dropdown->elements,
				&dropdown->active))
		{
			dropdown->bass->show = 0;
			dropdown->drop->toggle = 0;
		}
	}
	if (dropdown->active != NULL)
	{
		bui_change_element_text(dropdown->drop, dropdown->active->text);
		SDL_BlitScaled(dropdown->active->surface[0], NULL,
			dropdown->drop->surface[0], NULL);
		SDL_BlitScaled(dropdown->active->surface[1], NULL,
			dropdown->drop->surface[1], NULL);
		SDL_BlitScaled(dropdown->active->surface[2], NULL,
			dropdown->drop->surface[2], NULL);
		return (1);
	}
	return (0);
}

void	bui_set_element_list_color(t_list *list, Uint32 color)
{
	t_list	*curr;

	curr = list;
	while (curr)
	{
		bui_set_element_color(curr->content, color);
		curr = curr->next;
	}
}

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

int	only_one_button_toggled_at_a_time(t_list *list, t_bui_element **active)
{
	t_list	*curr;
	int		found;
	int		nth;

	found = 0;
	nth = 0;
	curr = list;
	while (curr)
	{
		if (((t_bui_element *)curr->content)->was_clicked_last_frame)
		{
			*active = curr->content;
			found = nth + 1;
		}
		else
			((t_bui_element *)curr->content)->toggle = 0;
		nth++;
		curr = curr->next;
	}
	if (*active != NULL)
		(*active)->toggle = 1;
	return (found);
}

int	only_one_button_toggled_at_a_time_can_be_untoggled(
		t_list *list, t_bui_element **active)
{
	t_list	*curr;
	int		found;

	found = 0;
	curr = list;
	while (curr)
	{
		if (((t_bui_element *)curr->content)->was_clicked_last_frame)
		{
			*active = curr->content;
			found = 1;
		}
		else
			((t_bui_element *)curr->content)->toggle = 0;
		curr = curr->next;
	}
	if (*active != NULL)
		(*active)->toggle = 1;
	return (found);
}

void	toggle_on_element_with_text(
		t_list *elements, t_bui_element **active, char *text)
{
	t_bui_element	*elem;
	t_list			*curr;

	curr = elements;
	while (curr)
	{
		elem = curr->content;
		if (ft_strcmp(elem->text, text) == 0)
		{
			elem->was_clicked_last_frame = 1;
			*active = elem;
		}
		else
			elem->was_clicked_last_frame = 0;
		curr = curr->next;
	}
}
