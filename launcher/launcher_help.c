#include "launcher.h"

t_bui_element	*new_bass_element(t_preset_dropdown *drop, char *text)
{
	t_bui_element	*elem;

	elem = preset_dropdown_add_element(drop, text);
	bui_set_element_image_to_states(elem,
		ICON_PATH"mc_button_x3.png",
		ICON_PATH"mc_button_x3_hover.png",
		ICON_PATH"mc_button_x3_disable.png");
	elem->text_x = 5;
	bui_center_element_text_y(elem);
	return (elem);
}

t_bui_element	*new_main_menu_menu(t_bui_element *parent, char *text)
{
	t_bui_element	*menu;
	t_xywh			c;

	c.x = parent->position.w / 4;
	c.y = parent->position.h / 3;
	c.w = parent->position.w - c.x - (parent->position.w / 8 / 2);
	c.h = parent->position.h - c.y - (parent->position.w / 8 / 2);
	menu = bui_new_element(parent, text, c);
	bui_set_element_color(menu, 0xd0740000);
	bui_set_element_border(menu, 2, 0xffc90000);
	bui_element_set_text_font(menu, TTF_PATH"AmazDoom.ttf");
	bui_set_element_text_size(menu, 40);
	bui_set_element_text_position(menu, 10, 5);
	menu->update_state = 0;
	menu->clear = 1;
	return (menu);
}

t_bui_element	*new_main_menu_button(t_launcher *launcher, char *text, int i)
{
	t_bui_element	*button;
	t_xywh			c;

	c.w = launcher->menu->position.w / 8;
	c.h = c.w / 2;
	c.y = (launcher->menu->position.h / 3) + (i * (c.h + 25));
	c.x = launcher->menu->position.w / 8 / 2;
	button = bui_new_element(launcher->menu, text, c);
	bui_set_element_text_font(button, TTF_PATH"AmazDoom.ttf", 48, 0xff000000);
	bui_set_element_image_all_states_image(button,
		launcher->mc_button,
		launcher->mc_button_hover,
		launcher->mc_button_click);
	bui_center_element_text(button);
	return (button);
}

t_bui_element	*new_map_button(
				t_bui_element *parent, char *text, int i, t_xywh c)
{
	t_bui_element	*button;
	t_xywh			coord;
	int				button_gap;
	int				amount_on_x;

	button_gap = 20;
	coord.w = 100;
	coord.h = 50;
	amount_on_x = c.w / (coord.w + button_gap + c.x);
	coord.x = c.x + (i % (amount_on_x + 1)) * (coord.w + button_gap);
	coord.y = c.y + (i / (amount_on_x + 1)) * (coord.h + button_gap);
	button = bui_new_element(parent, text, coord);
	bui_set_element_color(button, 0xff5398BE);
	bui_set_element_image_to_states(button,
		ICON_PATH"mc_button_x3.png",
		ICON_PATH"mc_button_x3_hover.png",
		ICON_PATH"mc_button_x3_click.png");
	bui_set_element_text_font(button, TTF_PATH"DroidSans.ttf", 12, 0xff000000);
	bui_center_element_text(button);
	return (button);
}

char	*get_clicked_map_button(t_list *map_buttons)
{
	t_list			*curr;
	t_bui_element	*elem;

	curr = map_buttons;
	while (curr)
	{
		elem = curr->content;
		if (bui_button(elem))
			return (elem->text);
		curr = curr->next;
	}
	return (NULL);
}

t_xywh	get_next_pos(t_xywh start_pos, int i)
{
	t_xywh	c;
	int		gap_y;

	gap_y = 10;
	c.w = 100;
	c.h = start_pos.h;
	c.x = start_pos.x;
	c.y = start_pos.y + (i * (c.h + gap_y));
	return (c);
}
