/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:55:37 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 11:55:38 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	changer_prefab_free(void *prefab, size_t size)
{
	(void)size;
	ft_memdel((void **)&prefab);
}

void	rword_free(void *content, size_t size)
{
	(void)size;
	(void)content;
}

void	grid_free(t_grid *grid)
{
	ft_lstdel(&grid->sectors, &free_sector);
	ft_lstdel(&grid->walls, &free_wall);
	ft_lstdel(&grid->points, &free_point);
	ft_lstdel(&grid->entities, &free_entity);
	TTF_CloseFont(grid->font);
}

void	editor_free2(t_editor *editor)
{
	int	i;

	changer_prefab_free(editor->ceiling_scale, 0);
	ft_lstdel(&editor->sector_texture_buttons, &dummy_free_er);
	changer_prefab_free(editor->slope_floor_wall_changer, 0);
	changer_prefab_free(editor->slope_floor_angle_changer, 0);
	changer_prefab_free(editor->slope_ceiling_wall_changer, 0);
	changer_prefab_free(editor->slope_ceiling_angle_changer, 0);
	dropdown_preset_free(editor->entity_type_drop, 0);
	ft_lstdel(&editor->entity_direction_radio_buttons, &dummy_free_er);
	i = -1;
	while (++i < editor->texture_amount)
		SDL_FreeSurface(editor->texture_textures[i]);
	free(editor->texture_textures);
	ft_strdel(&editor->event_str);
}

void	editor_free(t_editor *editor)
{
	ft_lstdel(&editor->entity_presets, &free_entity_preset);
	free_entity_preset(editor->default_entity, 0);
	changer_prefab_free(editor->scaler, 0);
	grid_free(&editor->grid);
	ft_strdel(&editor->fullpath);
	ft_strdel(&editor->mapname);
	ft_lstdel(&editor->select_mode_buttons, &dummy_free_er);
	ft_lstdel(&editor->map_type_tickboxes, &dummy_free_er);
	tab_preset_free(editor->wall_tab, 0);
	ft_lstdel(&editor->wall_texture_buttons, &dummy_free_er);
	ft_lstdel(&editor->portal_texture_buttons, &dummy_free_er);
	ft_lstdel(&editor->wall_sprite_buttons, &dummy_free_er);
	changer_prefab_free(editor->texture_scale_changer, 0);
	changer_prefab_free(editor->sprite_scale_changer, 0);
	changer_prefab_free(editor->sprite_changer, 0);
	changer_prefab_free(editor->floor_height, 0);
	changer_prefab_free(editor->ceiling_height, 0);
	changer_prefab_free(editor->gravity, 0);
	changer_prefab_free(editor->lighting, 0);
	changer_prefab_free(editor->floor_scale, 0);
	editor_free2(editor);
	ft_memdel((void **)&editor);
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
}
