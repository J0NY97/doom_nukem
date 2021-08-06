/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:44:38 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * how many poitns, walls, sectors, entities, wallsprites, (portals?)
*/
void	update_general_info_element(t_editor *editor)
{
	char	*str;

	recount_everything(editor);
	str = ft_sprintf("v: %d, w: %d, s: %d, e: %d\n",
			editor->grid.point_amount, editor->grid.wall_amount,
			editor->grid.sector_amount, editor->grid.entity_amount);
	bui_set_element_text(editor->general_info, str);
	ft_strdel(&str);
}

int	get_point_connection_amount(t_list **walls, t_point *point)
{
	t_list	*curr;
	int		amount;

	curr = *walls;
	amount = 0;
	while (curr)
	{
		if (vector_compare(((t_wall *)curr->content)->dest->pos, point->pos)
			|| vector_compare(((t_wall *)curr->content)->orig->pos, point->pos))
			amount++;
		curr = curr->next;
	}
	return (amount);
}

int	vector_on_wall(t_vector v, t_wall *wall)
{
	if (gfx_vector_dot(v, wall->orig->pos, wall->dest->pos) == 0)
	{
		if (v.x >= fmin(wall->orig->pos.x, wall->dest->pos.x)
			&& v.x <= fmax(wall->orig->pos.x, wall->dest->pos.x))
		{
			if (v.y >= fmin(wall->orig->pos.y, wall->dest->pos.y)
				&& v.y <= fmax(wall->orig->pos.y, wall->dest->pos.y))
				return (1);
		}
	}
	return (0);
}

void	selection(t_editor *editor, t_grid *grid)
{
	if (!(SDL_GetWindowFlags(editor->window->win) & SDL_WINDOW_MOUSE_FOCUS))
		return ;
	if (editor->libui->mouse_down_last_frame
		&& mouse_pressed(editor->libui, SDL_BUTTON_LEFT))
	{
		if (bui_button_toggle(editor->select_mode_vertex))
			select_point(grid);
		else
			editor->grid.modify_point = NULL;
		if (bui_button_toggle(editor->select_mode_wall))
			select_wall(editor);
		else
			editor->grid.modify_wall = NULL;
		if (bui_button_toggle(editor->select_mode_entity))
			select_entity(editor);
		else
			editor->grid.modify_entity = NULL;
		if (bui_button_toggle(editor->select_mode_sector))
			select_sector(grid);
		else
			editor->grid.modify_sector = NULL;
	}
}
