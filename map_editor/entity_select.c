/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_select.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:43:00 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:43:14 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_entity	*get_entity_from_list_around_radius(
			t_list *entities, t_vector pos, float allowed_radius)
{
	t_entity	*temp;
	float		x;
	float		y;

	temp = get_entity_from_list_at_pos(entities, pos);
	if (temp)
		return (temp);
	x = -allowed_radius;
	while (x <= allowed_radius)
	{
		y = -allowed_radius;
		while (y <= allowed_radius)
		{
			temp = get_entity_from_list_at_pos(entities,
					(t_vector){pos.x + x, pos.y + y, 0});
			if (temp != NULL)
				break ;
			y += 0.5f;
		}
		if (temp != NULL)
			break ;
		x += 0.5f;
	}
	return (temp);
}

void	draw_selected_entity(t_grid *grid)
{
	if (grid->modify_entity == NULL)
		return ;
	gfx_draw_vector(grid->elem->active_surface, 0xffff0000, 8,
		gfx_vector_multiply(grid->modify_entity->pos, grid->gap));
}

void	select_entity(t_editor *editor)
{
	t_entity	*entity;

	entity = get_entity_from_list_around_radius(editor->grid.entities,
			editor->grid.hover, 1.0f);
	if (entity)
	{
		editor->grid.modify_entity = entity;
		editor->entity_type_drop->drop->toggle = 0;
	}
}
