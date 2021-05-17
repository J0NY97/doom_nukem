#include "editor.h"

// TODO: move all the free functions in the same files as the malloc functions.

void	changer_prefab_free(void *prefab, size_t size)
{
	(void)size;
	t_changer_prefab *pref;
	pref = prefab;

	ft_memdel((void **)&prefab);
}

void	rword_free(void *content, size_t size)
{
	(void)size;
	ft_memdel((void **)&content);
}

// NOTE: dont actually free it because the actual grid is never mallcoed.
void	grid_free(t_grid *grid)
{
	t_list *curr;

	curr = grid->sectors;
	while (curr)
	{
		ft_lstdel(&((t_sector *)curr->content)->walls, &dummy_free_er);
		curr = curr->next;
	}
	ft_lstdel(&grid->points, &rword_free);
	ft_lstdel(&grid->walls, &rword_free);
	ft_lstdel(&grid->sectors, &rword_free);
	ft_lstdel(&grid->entities, &rword_free);
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

	// NOTE: these 2 lines commented out because currently they are not used.
	//free(editor->sprite_buttons);
	//free(editor->entity_sprite_buttons);

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

	changer_prefab_free(editor->texture_scale_changer, 0);
	changer_prefab_free(editor->sprite_scale_changer, 0);

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

	ft_memdel((void **)&editor);
}
