/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_element.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 12:00:44 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 12:12:29 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

t_bui_element	*bui_new_element_defaults(
	t_bui_libui *libui, char *text, t_xywh pos)
{
	t_bui_element	*elem;

	elem = ft_memalloc(sizeof(t_bui_element));
	elem->children = NULL;
	elem->libui = libui;
	elem->position = pos;
	elem->surface[0] = create_surface(pos.w, pos.h);
	elem->surface[1] = create_surface(pos.w, pos.h);
	elem->surface[2] = create_surface(pos.w, pos.h);
	bui_set_element_color(elem, 0xff809848);
	elem->active_surface = create_surface(pos.w, pos.h);
	elem->text_color = 0xff000000;
	elem->font_size = 12;
	elem->font_name = ft_strdup(BUI_PATH"DroidSans.ttf");
	elem->font_style = TTF_STYLE_NORMAL;
	bui_set_element_text_position(elem, 0, 0);
	bui_set_element_text(elem, text);
	elem->state = ELEMENT_DEFAULT;
	elem->show = 1;
	elem->type = TYPE_ELEMENT;
	elem->update = 1;
	elem->update_state = 1;
	add_to_list(&elem->libui->elements, elem, sizeof(t_bui_element));
	return (elem);
}

t_bui_element	*bui_new_element(t_bui_element *parent, char *text, t_xywh pos)
{
	t_bui_element	*elem;

	elem = bui_new_element_defaults(parent->libui, text, pos);
	elem->type = TYPE_ELEMENT;
	elem->parent = parent;
	add_to_list(&parent->children, elem, sizeof(t_bui_element));
	return (elem);
}

t_bui_element	*bui_new_menu(t_bui_window *parent, char *name, t_xywh pos)
{
	t_bui_element	*menu;

	menu = bui_new_element_defaults(parent->libui, name, pos);
	menu->type = TYPE_MENU;
	menu->parent = parent;
	add_to_list(&parent->menus, menu, sizeof(t_bui_element));
	return (menu);
}
