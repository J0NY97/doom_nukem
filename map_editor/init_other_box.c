#include "editor.h"

t_bui_element *
	new_map_type_tickbox(t_bui_element *parent, char *text, t_xywh coord)
{
	t_bui_element	*tick;
	t_xywh			text_coord;

	text_coord = coord;
	text_coord.w = 50;
	bui_new_element(parent, text, text_coord);
	coord.x += text_coord.w;
	tick = bui_new_element(parent, NULL, coord);
	bui_set_element_image_to_states(tick,
		ICON_PATH"tick_box_off.png",
		ICON_PATH"tick_box_hover.png",
		ICON_PATH"tick_box_on.png");
	return (tick);
}

void	map_type_tickbox_init(int gap, t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(editor->button_save->position.x
			+ editor->button_save->position.w + gap,
			editor->button_save->position.y - gap, 20, 20);
	editor->endless_tickbox = new_map_type_tickbox(editor->other_mode,
			"endless", coord);
	coord = new_xywh(editor->button_save->position.x
			+ editor->button_save->position.w + gap,
			editor->button_save->position.y + gap, 20, 20);
	editor->story_tickbox = new_map_type_tickbox(editor->other_mode,
			"story", coord);
	if (ft_strendswith(editor->fullpath, ".story") == 0)
		editor->active_map_type = editor->story_tickbox;
	else
		editor->active_map_type = editor->endless_tickbox;
	add_to_list(&editor->map_type_tickboxes,
		editor->endless_tickbox, sizeof(t_bui_element));
	add_to_list(&editor->map_type_tickboxes,
		editor->story_tickbox, sizeof(t_bui_element));
}

void	button_init_other(int button_w, int gap, t_editor *editor)
{
	t_xywh	coord;

	coord = new_xywh(gap, 20, 100, 20);
	editor->map_name_input
		= bui_new_element(editor->other_mode, editor->mapname, coord);
	editor->map_name_input->text_x = 5;
	coord = new_xywh(editor->map_name_input->position.x
			+ editor->map_name_input->position.w + gap, 20,
			button_w, button_w);
	editor->button_save = bui_new_element(editor->other_mode,
			"save", coord);
	editor->button_save->text_y = -20;
	bui_set_element_image_to_states(editor->button_save,
		ICON_PATH"save_button.png",
		ICON_PATH"save_button_click.png",
		ICON_PATH"save_button_click.png");
	map_type_tickbox_init(gap, editor);
}
void	button_init(t_editor *editor)
{
	t_xywh	coord;
	int		gap;
	int		button_w;

	gap = 10;
	button_w = 32;
	coord = new_xywh(10, 25, 50, 55);
	coord = new_xywh(editor->select_mode->position.x,
			editor->select_mode->position.y
			+ editor->select_mode->position.h + 10,
			editor->toolbox->position.w - 20, 55);
	editor->other_mode = bui_new_element(editor->toolbox, "Other", coord);
	editor->other_mode->update_state = 0;
	bui_set_element_text_font(editor->other_mode, TTF_PATH"DroidSans.ttf",
		editor->other_mode->font_size, 0xffffffff);
	bui_set_element_color(editor->other_mode, editor->palette.elem_elem);
	button_init_other(button_w, gap, editor);
}
