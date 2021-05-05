#include "editor.h"

// TODO: move all the free functions in the same files as the malloc functions.

void	changer_prefab_free(void *prefab, size_t size)
{
	t_changer_prefab *pref;
	pref = prefab;

	free(prefab);
}

// NOTE: dont actually free it because the actual grid is never mallcoed.
void	grid_free(t_grid *grid)
{
ft_putstr("[Grid]\n");
	ft_lstdel(&grid->points, &free_point);
ft_putstr("Points freed.\n");
	ft_lstdel(&grid->walls, &free_wall);
ft_putstr("Walls freed.\n");
	ft_lstdel(&grid->sectors, &free_sector);
ft_putstr("Sectors freed.\n");
	ft_lstdel(&grid->entities, &free_entity);
ft_putstr("Entities freed.\n");
}

void	editor_free(t_editor *editor)
{
	ft_lstdel(&editor->entity_presets, &free_entity_preset);
	free_entity_preset(editor->default_entity, 0);

	changer_prefab_free(editor->scaler, 0);

	grid_free(&editor->grid);
	
	ft_strdel(&editor->fullpath);
	ft_strdel(&editor->mapname);

	// t_map stuff
	ft_strdel(&editor->map.texture_pack);
	ft_strdel(&editor->map.name);
	// t_spawn stuff
	// nothing to do
	
	free(editor->sprite_buttons);
	free(editor->entity_sprite_buttons);

	// TODO: this isnt ready so dont know how it will look yet in the mallocer.
//	ft_lstdel(&editor->all_textures, &editor_texture_free);
//	ft_lstdel(&editor->all_textures, &dummy_free_er);

	ft_lstdel(&editor->select_mode_buttons, &dummy_free_er);
	ft_lstdel(&editor->map_type_tickboxes, &dummy_free_er);

	// preset tab free...
	tab_preset_free(editor->wall_tab, 0);

	ft_lstdel(&editor->wall_texture_buttons, &dummy_free_er);
	ft_lstdel(&editor->portal_texture_buttons, &dummy_free_er);
	ft_lstdel(&editor->wall_sprite_buttons, &dummy_free_er);

	changer_prefab_free(editor->floor_height, 0);
	changer_prefab_free(editor->ceiling_height, 0);
	changer_prefab_free(editor->gravity, 0);
	changer_prefab_free(editor->lighting, 0);
	changer_prefab_free(editor->floor_scale, 0);
	changer_prefab_free(editor->ceiling_scale, 0);

	ft_lstdel(&editor->ceiling_texture_buttons, &dummy_free_er);
	ft_lstdel(&editor->floor_texture_buttons, &dummy_free_er);

	dropdown_preset_free(editor->entity_type_drop, 0);

	ft_lstdel(&editor->entity_direction_radio_buttons, &dummy_free_er);
}
