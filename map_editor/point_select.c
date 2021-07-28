/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_select.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:43:02 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:46:19 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_point	*get_point_from_list_around_radius(
		t_list *points, t_vector pos, float allowed_radius)
{
	t_point		*temp;
	float		x;
	float		y;

	temp = get_point_from_list_at_pos(points, pos);
	if (temp)
		return (temp);
	x = -allowed_radius;
	while (x <= allowed_radius)
	{
		y = -allowed_radius;
		while (y <= allowed_radius)
		{
			temp = get_point_from_list_at_pos(points,
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

void	draw_selected_point(t_editor *editor, t_grid *grid)
{
	char	*s;

	if (grid->modify_point == NULL)
		s = ft_strdup("Selected Vector: \nNULL");
	else
	{
		s = ft_sprintf("Selected Vector:\nx %d\ny %d\nconnections:%d\n",
				(int)grid->modify_point->pos.x,
				(int)grid->modify_point->pos.y,
				get_point_connection_amount(&grid->walls,
					grid->modify_point));
		gfx_draw_vector(grid->elem->active_surface, 0xffffae42, 2,
			gfx_vector_multiply(grid->modify_point->pos,
				grid->gap));
	}
	editor->selected_vector_info->text_color = 0xffffffff;
	bui_set_element_text(editor->selected_vector_info, s);
	ft_strdel(&s);
}

void	select_point(t_grid *grid)
{
	t_point	*point;

	point = get_point_from_list_around_radius(grid->points, grid->hover, 1.0f);
	if (point)
		grid->modify_point = point;
}
