#include "better_libui.h"

t_bui_element	*bui_new_element(t_bui_element *parent, char *text, t_xywh pos)
{
	t_bui_element	*elem;

	elem = ft_memalloc(sizeof(t_bui_element));
	elem->children = NULL;
	elem->parent = parent;
	elem->libui = parent->libui;
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
	bui_set_element_text(elem, text, 0, 0);
	elem->state = ELEMENT_DEFAULT;
	elem->show = 1;
	elem->type = TYPE_ELEMENT;
	elem->update = 1;
	elem->update_state = 1;
	add_to_list(&parent->children, elem, sizeof(t_bui_element));
	add_to_list(&elem->libui->elements, elem, sizeof(t_bui_element));
	return (elem);
}

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

int	bui_button(t_bui_element *button)
{
	if (button->state == ELEMENT_CLICK
		&& button->was_clicked_last_frame
		&& !button->already_clicked)
	{
		button->already_clicked = 1;
		return (1);
	}
	else
		return (0);
}

int	bui_button_allow_duplicate(t_bui_element *button)
{
	if (button->state == ELEMENT_CLICK)
		return (1);
	else
		return (0);
}

int	bui_button_toggle(t_bui_element *button)
{
	if (button->state == ELEMENT_CLICK)
	{
		if (button->was_clicked_last_frame == 1
			&& button->already_clicked == 0)
		{
			button->already_clicked = 1;
			button->toggle = button->toggle == 0;
		}
		return (1);
	}	
	else
		return (0);
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

void	draw_surface_border(SDL_Surface *surface, Uint32 color, int thiccness)
{
	int		i;
	t_xywh	c;

	c.x = 0;
	c.y = 0;
	c.w = surface->w;
	c.h = surface->h;
	i = 0;
	if (!surface)
		ft_printf("[draw_surface_border] surface == NULL.\n");
	while (i < thiccness)
	{
		draw_rect(surface, (t_xywh){c.x + i, c.y + i,
			c.w - (i * 2), c.h - (i * 2)}, color, 0);
		i++;
	}
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

/*
 * the name of this function should probably be element_set_text,
 * 		and the function below, somethinig else.
*/
void	bui_change_element_text(t_bui_element *elem, char *text)
{
	bui_set_element_text(elem, text, elem->text_x, elem->text_y);
}

// remove itn x and y at some ponit.
void	bui_set_element_text(t_bui_element *elem, char *text, int x, int y)
{
	char	*temp_font_name;

	if (elem->text)
		ft_strdel(&elem->text);
	if (text == NULL)
	{
		elem->text = NULL;
		return ;
	}
	if (elem->text_surface)
	{
		SDL_FreeSurface(elem->text_surface);
		elem->text_surface = NULL;
	}
	elem->text = ft_strdup(text);
	elem->text_x = x;
	elem->text_y = y;
	temp_font_name = ft_strdup(elem->font_name);
	bui_set_element_text_font(elem, temp_font_name,
		elem->font_size, elem->text_color);
	ft_strdel(&temp_font_name);
}

void	bui_remove_element_text(t_bui_element *elem)
{
	if (elem->text)
		ft_strdel(&elem->text);
	if (elem->font)
		TTF_CloseFont(elem->font);
	if (elem->text_surface)
		SDL_FreeSurface(elem->text_surface);
	if (elem->font_name)
		ft_strdel(&elem->font_name);
	elem->text = NULL;
	elem->font = NULL;
	elem->text_surface = NULL;
	elem->font_name = NULL;
}

void	bui_set_element_text_color(t_bui_element *elem, Uint32 color)
{
	char	*font_name;

	font_name = ft_strdup(elem->font_name);
	bui_set_element_text_font(elem, font_name, elem->font_size, color);
	ft_strdel(&font_name);
}

void	bui_set_element_text_size(t_bui_element *elem, Uint32 size)
{
	char	*font_name;

	font_name = ft_strdup(elem->font_name);
	bui_set_element_text_font(elem, font_name, size, elem->text_color);
	ft_strdel(&font_name);
}

void	bui_element_set_text_font(t_bui_element *elem, char *font_name)
{
	bui_set_element_text_font(elem, font_name,
		elem->font_size, elem->text_color);
}

void	bui_element_set_text_style(t_bui_element *elem, int style)
{
	char	*font_name;

	elem->font_style = style;
	font_name = ft_strdup(elem->font_name);
	bui_set_element_text_font(elem, font_name,
		elem->font_size, elem->text_color);
	ft_strdel(&font_name);
}

void	bui_set_element_text_font(
		t_bui_element *elem, char *font_name, Uint32 size, Uint32 color)
{
	t_text_recipe	recipe;
	SDL_Surface		*text_surface;

	if (!elem->text || !font_name)
		return ;
	text_surface = bui_make_text_surface_from_recipe(&(t_text_recipe){
			.text = elem->text, .font_name = font_name,
			.font_style = elem->font_style, .max_w = elem->position.w
			- elem->text_x, .text_color = color, .font_size = size});
	if (text_surface)
	{
		if (elem->text_surface)
			SDL_FreeSurface(elem->text_surface);
		if (elem->font)
			TTF_CloseFont(elem->font);
		ft_strdel(&elem->font_name);
		elem->font_name = ft_strdup(font_name);
		elem->font = TTF_OpenFont(font_name, size);
		elem->text_surface = text_surface;
		elem->text_color = color;
		elem->font_size = size;
		elem->text_w = elem->text_surface->w;
		elem->text_h = elem->text_surface->h;
	}
	else
		ft_putstr("[bui_set_element_text_font] Couldnt make text surface.\n");
}

void	bui_set_element_text_position(t_bui_element *elem, int x, int y)
{
	elem->text_x = x;
	elem->text_y = y;
}

void	bui_center_element_text_x(t_bui_element *elem)
{
	int	text_w;
	int	text_h;

	if (!elem->font || !elem->text)
		return ;
	TTF_SizeText(elem->font, elem->text, &text_w, &text_h);
	elem->text_x = (elem->position.w / 2) - (text_w / 2);
}

void	bui_center_element_text_y(t_bui_element *elem)
{
	int	text_w;
	int	text_h;

	if (!elem->font || !elem->text)
		return ;
	TTF_SizeText(elem->font, elem->text, &text_w, &text_h);
	elem->text_y = (elem->position.h / 2) - (text_h / 2);
}

void	bui_center_element_text(t_bui_element *elem)
{
	if (!elem->font || !elem->text)
		return ;
	bui_center_element_text_x(elem);
	bui_center_element_text_y(elem);
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

char	*remove_from_input(t_bui_element *input)
{
	int		len;
	char	*new_str;

	len = 0;
	new_str = NULL;
	if (input->text)
		len = ft_strlen(input->text);
	if (len > 0)
		new_str = ft_strndup(input->text, len - 1);
	return (new_str);
}

char	*add_to_input(t_bui_element *input)
{
	t_bui_libui	*libui;
	char		*new_str;

	libui = input->libui;
	if (input->text)
		new_str = ft_strdup(input->text);
	else
		new_str = ft_strnew(0);
	if (key_pressed(libui, KEY_SPACE))
		ft_straddchar(&new_str, ' ');
	else
	{
		if (alpha_pressed(libui) || number_pressed(libui))
		{
			if (!key_pressed(libui, KEY_LSHIFT))
				ft_straddchar(&new_str,
					ft_tolower(libui->last_key[0]));
			else
				ft_stradd(&new_str, libui->last_key);
		}
	}
	return (new_str);
}

int	bui_input(t_bui_element *input)
{
	t_bui_libui	*libui;
	char		*new_str;
	int			len;

	libui = input->libui;
	if (bui_button_toggle(input))
	{
		if (libui->last_key != NULL)
		{
			if (key_pressed(libui, KEY_RETURN))
			{
				input->toggle = 0;
				return (1);
			}
			else if (key_pressed(libui, KEY_BACKSPACE))
				new_str = remove_from_input(input);
			else
				new_str = add_to_input(input);
			bui_change_element_text(input, new_str);
			ft_strdel(&new_str);
		}
	}
	return (0);
}

void	bui_set_element_image_from_path(
		t_bui_element *element, int type, char *path, t_xywh *pos)
{
	SDL_Surface	*image;

	image = load_image(path);
	if (image)
	{
		bui_set_element_image(element, type, image, pos);
		SDL_FreeSurface(image);
	}
}

void	bui_set_element_image(
		t_bui_element *element, int type, SDL_Surface *image, t_xywh *pos)
{
	SDL_Rect	*rect;

	rect = NULL;
	if (!image)
	{
		ft_putstr("[bui_set_element_image] SDL_Surface *image doesnt exist.\n");
		return ;
	}
	if (pos)
		rect = &(SDL_Rect){pos->x, pos->y, pos->w, pos->h};
	if (type == ELEMENT_ALL)
	{
		SDL_BlitScaled(image, NULL, element->surface[0], rect);
		SDL_BlitScaled(image, NULL, element->surface[1], rect);
		SDL_BlitScaled(image, NULL, element->surface[2], rect);
	}
	else
		SDL_BlitScaled(image, NULL, element->surface[type], rect);
}

void	bui_set_element_image_all_states_image(
	t_bui_element *element, SDL_Surface *zeroth,
	SDL_Surface *first, SDL_Surface *second)
{
	bui_set_element_image(element, ELEMENT_DEFAULT, zeroth, NULL);
	bui_set_element_image(element, ELEMENT_HOVER, first, NULL);
	bui_set_element_image(element, ELEMENT_CLICK, second, NULL);
}

//void	bui_set_element_image_all_states_from_path
void	bui_set_element_image_to_states(
		t_bui_element *element, char *zeroth, char *first, char *second)
{
	bui_set_element_image_from_path(element, ELEMENT_DEFAULT, zeroth, NULL);
	bui_set_element_image_from_path(element, ELEMENT_HOVER, first, NULL);
	bui_set_element_image_from_path(element, ELEMENT_CLICK, second, NULL);
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
