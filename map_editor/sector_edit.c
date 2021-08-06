/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_edit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:42:16 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:42:31 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	sector_changer_prefab_events(t_editor *editor)
{
	changer_prefab_events(editor->floor_height,
		&editor->grid.modify_sector->floor_height, 1);
	changer_prefab_events(editor->ceiling_height,
		&editor->grid.modify_sector->ceiling_height, 1);
	changer_prefab_events(editor->gravity,
		&editor->grid.modify_sector->gravity, 1);
	changer_prefab_events(editor->lighting,
		&editor->grid.modify_sector->light_level, 1);
	changer_prefab_events_float(editor->floor_scale,
		&editor->grid.modify_sector->floor_texture_scale, 0.1f);
	changer_prefab_events_float(editor->ceiling_scale,
		&editor->grid.modify_sector->ceiling_texture_scale, 0.1f);
}

void	draw_fandc_scaled_line(
	t_editor *editor, SDL_Surface *surf, t_vector center, float scale)
{
	t_vector	*v;

	v = get_scaled_line(surf, get_nth_from_list(
				&editor->grid.modify_sector->walls,
				editor->grid.modify_sector->floor_slope_wall_id)->content,
			center, scale - 1.5f);
	gfx_draw_line(surf, 0xff0000ff, v[0], v[1]);
	free(v);
	v = get_scaled_line(surf, get_nth_from_list(
				&editor->grid.modify_sector->walls,
				editor->grid.modify_sector->ceiling_slope_wall_id)->content,
			center, scale - 0.5f);
	gfx_draw_line(surf, 0xff00ff00, v[0], v[1]);
	free(v);
}

void	draw_sector_viewer(t_editor *editor, SDL_Surface *surf)
{
	t_vector	*v;
	t_vector	center;
	t_list		*curr;
	float		scale;

	curr = editor->grid.modify_sector->walls;
	scale = editor->slope_sector->position.h
		/ get_wall_length(get_longest_wall_from_list(curr));
	center = gfx_vector_divide(editor->grid.modify_sector->center,
			editor->grid.gap);
	while (curr)
	{
		v = get_scaled_line(surf, curr->content, center, scale - 1.0f);
		gfx_draw_line(surf, editor->grid.modify_sector->color,
			v[0], v[1]);
		free(v);
		curr = curr->next;
	}
	draw_fandc_scaled_line(editor, surf, center, scale);
}
