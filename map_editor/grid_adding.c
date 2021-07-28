#include "editor.h"

void	click_calc_sec(t_grid *grid)
{
	t_sector	*sector;

	if (grid->modify_sector == NULL)
	{
		sector = new_sector(grid->sector_amount++);
		add_to_list(&grid->sectors, sector, sizeof(t_sector));
		grid->modify_sector = sector;
	}
	if (vector_is_empty(grid->selected1))
		grid->selected1 = grid->hover;
	else if (!vector_compare(grid->selected1, grid->hover))
		grid->selected2 = grid->hover;
}

void	click_calc_ent(t_editor *editor)
{
	t_entity	*entity;

	entity = new_entity(editor->grid.entity_amount++, editor->grid.hover);
	entity->preset = get_entity_preset_with_name(
			editor->entity_presets, "Barrel");
	add_to_list(&editor->grid.entities, entity, sizeof(t_entity));
}

void	click_calc(t_editor *editor, t_grid *grid)
{
	if (!mouse_hover(editor->libui, (t_xywh){
			grid->elem->position.x, grid->elem->position.y,
			grid->elem->position.w, grid->elem->position.h}))
		return ;
	if (editor->libui->mouse_down_last_frame
		&& mouse_pressed(editor->libui, SDL_BUTTON_LEFT))
		click_calc_sec(grid);
	else if (editor->libui->mouse_down_last_frame
		&& mouse_pressed(editor->libui, SDL_BUTTON_RIGHT))
		click_calc_ent(editor);
	else if (mouse_pressed(editor->libui, SDL_BUTTON_MIDDLE))
		editor->spawn.pos = grid->hover;
}
