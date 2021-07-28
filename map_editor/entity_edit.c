#include "editor.h"

void	draw_selected_entity_texture(t_editor *editor)
{
	SDL_Surface	*ent_tex;
	SDL_Surface	*dst_surf;
	SDL_Rect	rect;
	float		aspect;

	if (!editor->grid.modify_entity->preset->texture)
		return ;
	ent_tex = editor->grid.modify_entity->preset->texture;
	dst_surf = editor->edit_view_entity->active_surface;
	aspect = ft_fmin((float)dst_surf->w / (float)ent_tex->w,
			(float)dst_surf->h / (float)ent_tex->h);
	rect.w = ent_tex->w * aspect;
	rect.h = ent_tex->h * aspect;
	rect.x = (dst_surf->w - rect.w) / 2;
	rect.y = (dst_surf->h - rect.h) / 2;
	SDL_BlitScaled(editor->grid.modify_entity->preset->texture, NULL,
		editor->edit_view_entity->active_surface, &rect);
}

void	entity_drop_down_event(t_editor *editor)
{
	editor->entity_type_drop->active = bui_get_element_with_text_from_list(
			editor->entity_type_drop->elements,
			editor->grid.modify_entity->preset->name);
	preset_dropdown_events(editor->entity_type_drop);
	if (editor->entity_type_drop->active != NULL)
	{
		editor->grid.modify_entity->preset
			= get_entity_preset_with_name(editor->entity_presets,
				editor->entity_type_drop->active->text);
	}
}

