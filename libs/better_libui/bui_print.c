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
