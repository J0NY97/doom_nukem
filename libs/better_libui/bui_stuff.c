#include "better_libui.h"

const char	*bui_element_get_state_string(t_bui_element *elem)
{
	if (elem->state == ELEMENT_DEFAULT)
		return ("DEFAULT");
	else if (elem->state == ELEMENT_HOVER)
		return ("HOVER");
	else if (elem->state == ELEMENT_CLICK)
		return ("CLICK");
	return ("UNKNOWN");
}

const char	*bui_element_get_type_string(t_bui_element *elem)
{
	if (elem->type == TYPE_ELEMENT)
		return ("ELEMENT");
	else if (elem->type == TYPE_MENU)
		return ("MENU");
	return ("UNKNOWN");
}

void	bui_element_print(t_bui_element *elem)
{
	ft_printf("[bui_element_print]\n{\n\ttext: %s\n", elem->text);
	ft_printf("\tposition: ");
	xywh_print(elem->position);
	ft_printf("\tscreen_pos: ");
	xywh_print(elem->screen_pos);
	ft_printf("\tcolor: %#x %#x %#x\n",
		elem->color, elem->color_light, elem->color_dark);
	ft_printf("\tstate: %s\n", bui_element_get_state_string(elem));
	ft_printf("\talready_clicked: %d\n", elem->already_clicked);
	ft_printf("\tshow: %d\n", elem->show);
	ft_printf("\twas_clicked_last_frame: %d\n", elem->was_clicked_last_frame);
	ft_printf("\twas_hovered_last_frame: %d\n", elem->was_hovered_last_frame);
	ft_printf("\ttoggle: %d\n", elem->toggle);
	ft_printf("\ttype: %s\n", bui_element_get_type_string(elem));
	ft_printf("\tupdate: %d\n", elem->update);
	ft_printf("\tupdate_state: %d\n", elem->update_state);
	ft_printf("\tclear: %d\n", elem->clear);
	ft_printf("\ttext pos: { %d %d %d %d }\n",
		elem->text_x, elem->text_y, elem->text_w, elem->text_h);
	ft_printf("\ttext_color: %#x\n", elem->text_color);
	ft_printf("\tfont_size: %d\n", elem->font_size);
	ft_printf("\tfont_name: %s\n", elem->font_name);
	ft_printf("\tfont: %d\n", elem->font != NULL);
	ft_printf("\tremove: %d\n", elem->remove);
	ft_putstr("{\n");
}

// Remove this if you dont come up with something better.
void	bui_set_element_flags(t_bui_element *elem, Uint32 flags)
{
	if (flags & BUI_ELEMENT_DONT_UPDATE)
		elem->update = 0;
	else if (flags & BUI_ELEMENT_UPDATE)
		elem->update = 1;
	if (flags & BUI_ELEMENT_DONT_UPDATE_STATE)
		elem->update_state = 0;
	else if (flags & BUI_ELEMENT_UPDATE_STATE)
		elem->update_state = 1;
	if (flags & BUI_ELEMENT_DONT_SHOW)
		elem->show = 0;
	else if (flags & BUI_ELEMENT_SHOW)
		elem->show = 1;
	if (flags & BUI_ELEMENT_DONT_CLEAR)
		elem->clear = 0;
	else if (flags & BUI_ELEMENT_CLEAR)
		elem->clear = 1;
}

void	bui_set_element_color(t_bui_element *elem, Uint32 color)
{
	elem->color = color;
	elem->color_rgba = hex_to_rgba(elem->color);
	elem->color_light = rgba_to_hex((t_rgba){
			elem->color_rgba.a,
			elem->color_rgba.r * 0.8f,
			elem->color_rgba.g * 0.8f,
			elem->color_rgba.b * 0.8f});
	elem->color_dark = rgba_to_hex((t_rgba){
			elem->color_rgba.a,
			elem->color_rgba.r * 1.5f,
			elem->color_rgba.g * 1.5f,
			elem->color_rgba.b * 1.5f});
	SDL_FillRect(elem->surface[0], NULL, elem->color);
	SDL_FillRect(elem->surface[1], NULL, elem->color_light);
	SDL_FillRect(elem->surface[2], NULL, elem->color_dark);
}

void	bui_set_element_state_border(
		t_bui_element *elem, int thiccness, Uint32 color, int state)
{
	if (state == ELEMENT_ALL)
		draw_surface_border(elem->active_surface, color, thiccness);
	if (state == ELEMENT_ALL || state == ELEMENT_DEFAULT)
		draw_surface_border(elem->surface[0], color, thiccness);
	if (state == ELEMENT_ALL || state == ELEMENT_HOVER)
		draw_surface_border(elem->surface[1], color, thiccness);
	if (state == ELEMENT_ALL || state == ELEMENT_CLICK)
		draw_surface_border(elem->surface[2], color, thiccness);
}

void	bui_set_element_border(t_bui_element *elem, int thiccness, Uint32 color)
{
	bui_set_element_state_border(elem, thiccness, color, ELEMENT_ALL);
}

void	bui_set_element_state(t_bui_element *elem, int state)
{
	if (state > 2 || state < 0)
	{
		ft_putstr("[bui_set_element_state] no state with that number.\n");
		return ;
	}
	elem->state = state;
	if (state == ELEMENT_CLICK)
		elem->was_clicked_last_frame = 1;
}

void	bui_element_resize(t_bui_element *element, t_xywh coord)
{
	if (element->active_surface)
		SDL_FreeSurface(element->active_surface);
	if (element->surface[0])
		SDL_FreeSurface(element->surface[0]);
	if (element->surface[1])
		SDL_FreeSurface(element->surface[1]);
	if (element->surface[2])
		SDL_FreeSurface(element->surface[2]);
	element->surface[0] = create_surface(coord.w, coord.h);
	element->surface[1] = create_surface(coord.w, coord.h);
	element->surface[2] = create_surface(coord.w, coord.h);
	bui_set_element_color(element, element->color);
	element->active_surface = create_surface(coord.w, coord.h);
	element->position = coord;
}

t_bui_element	*bui_get_element_with_text(t_bui_libui *libui, char *text)
{
	return (bui_get_element_with_text_from_list(libui->elements, text));
}

t_bui_element	*bui_get_element_with_text_from_list(t_list *list, char *text)
{
	t_list	*curr;

	if (!text)
		return (NULL);
	curr = list;
	while (curr)
	{
		if (ft_strequ(text, ((t_bui_element *)curr->content)->text))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}
