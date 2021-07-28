#include "editor.h"

void	new_radio_button(
		t_list **list, t_bui_element *parent, t_xywh c, char *str)
{
	t_xywh			coord;
	t_bui_element	*radio;

	coord = new_xywh(c.x, c.y, 15, 15);
	radio = bui_new_element(parent, str, coord);
	radio->text_y = -100;
	bui_set_element_color(radio, 0x00);
	bui_set_element_image_to_states(radio,
		ICON_PATH"radio_button_off.png",
		ICON_PATH"radio_button_hover.png",
		ICON_PATH"radio_button_on.png");
	add_to_list(list, radio, sizeof(t_bui_element));
}

void	add_to_entity_editor(t_editor *editor)
{
	t_xywh	coord;
	t_list	*curr;

	curr = editor->entity_presets;
	while (curr)
	{
		preset_dropdown_add_element(editor->entity_type_drop,
			((t_entity_preset *)curr->content)->name);
		curr = curr->next;
	}
	coord = new_xywh(editor->edit_toolbox_entity->position.w * 0.5f - 50,
			editor->edit_toolbox_entity->position.h * 0.5f, 100, 100);
	coord.x -= 7;
	new_radio_button(&editor->entity_direction_radio_buttons,
		editor->edit_entity_direction, (t_xywh){coord.x, 25, 0, 0}, "270");
	new_radio_button(&editor->entity_direction_radio_buttons,
		editor->edit_entity_direction, (t_xywh){coord.x + 25, 25 + 25, 0, 0},
		"0");
	new_radio_button(&editor->entity_direction_radio_buttons,
		editor->edit_entity_direction, (t_xywh){coord.x, 25 + 50, 0, 0}, "90");
	new_radio_button(&editor->entity_direction_radio_buttons,
		editor->edit_entity_direction, (t_xywh){coord.x - 25, 25 + 25, 0, 0},
		"180");
}

void	init_entity_editor2(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(5, 20,
			editor->edit_toolbox_entity->position.w - 10, 20);
	editor->entity_type_drop = bui_new_dropdown_preset(
			editor->edit_toolbox_entity, "Entity types", coord);
	coord = new_xywh(editor->edit_toolbox_entity->position.w * 0.5f
			- 50, editor->edit_toolbox_entity->position.h * 0.5f, 100, 100);
	editor->edit_entity_direction = bui_new_element(
			editor->edit_toolbox_entity, "Direction", coord);
	bui_set_element_color(editor->edit_entity_direction,
		editor->palette.elem_elem);
	bui_set_element_text_color(editor->edit_entity_direction, 0xffffffff);
	editor->edit_entity_direction->update_state = 0;
}

void	init_entity_editor(t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(5, 5, editor->edit_window->position.w
			* 0.20f, editor->edit_window->position.h - 10);
	editor->edit_toolbox_entity = bui_new_menu(editor->edit_window,
			"Entity Toolbox", coord);
	bui_set_element_color(editor->edit_toolbox_entity,
		editor->palette.elem_elem_elem);
	coord = new_xywh(editor->edit_toolbox_sector->position.x
			+ editor->edit_toolbox_sector->position.w + 5, 5,
			editor->edit_window->position.w
			- editor->edit_toolbox_sector->position.w - 15,
			editor->edit_window->position.h - 10);
	editor->edit_view_entity = bui_new_menu(editor->edit_window,
			"Entity View", coord);
	bui_set_element_color(editor->edit_view_entity,
		editor->palette.elem_elem_elem);
	init_entity_editor2(editor);
	add_to_entity_editor(editor);
}
