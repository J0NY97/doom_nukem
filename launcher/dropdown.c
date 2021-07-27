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

void	difficulty_drop_init(t_ui_menu_play *play_menu, t_xywh c)
{
	// Difficulty text elem
	c = new_xywh(25, 25, 100, 20);
	play_menu->difficulty_text_elem
		= bui_new_element(play_menu->endless_view, "Difficulty:", c);
	bui_set_element_color(play_menu->difficulty_text_elem,
		play_menu->endless_view->color);
	play_menu->difficulty_text_elem->update_state = 0;
	// Difficulty dropdown
	play_menu->difficulty_dropdown
		= bui_new_dropdown_preset(play_menu->endless_view, "Difficulty",
			(t_xywh){play_menu->difficulty_text_elem->position.x,
			play_menu->difficulty_text_elem->position.y
			+ play_menu->difficulty_text_elem->position.h, 100, 20});
	play_menu->difficulty_dropdown->drop->text_x = 5;
	bui_set_element_image_to_states(play_menu->difficulty_dropdown->arrow,
		ICON_PATH"mc_button.png",
		ICON_PATH"mc_button_hover.png",
		ICON_PATH"mc_button_click.png");
	// Bass elements
	new_bass_element(play_menu->difficulty_dropdown, "easy");
	new_bass_element(play_menu->difficulty_dropdown, "normal");
	play_menu->difficulty_dropdown->active
		= play_menu->difficulty_dropdown->elements->content;
	new_bass_element(play_menu->difficulty_dropdown, "hard");
}
