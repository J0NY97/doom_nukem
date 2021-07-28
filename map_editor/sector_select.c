/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_select.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:44:35 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:44:37 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	draw_wall_as_selected(t_grid *grid, SDL_Surface *surface, t_wall *wall)
{
	t_vector	orig;
	t_vector	dest;

	orig = gfx_vector_add(gfx_vector_multiply(wall->orig->pos, grid->gap), 1);
	dest = gfx_vector_add(gfx_vector_multiply(wall->dest->pos, grid->gap), 1);
	gfx_draw_line(surface, 0xffffae42, orig, dest);
	orig = gfx_vector_sub(orig, 2);
	dest = gfx_vector_sub(dest, 2);
	gfx_draw_line(surface, 0xffffae42, orig, dest);
}

void	draw_selected_sector(t_editor *editor, t_grid *grid)
{
	t_list		*curr;
	char		*str;

	if (grid->modify_sector == NULL)
		str = ft_strdup("Selected Sector: \nNULL");
	else
	{
		str = ft_sprintf("Selected Sector:\nid: %d\nwalls: %d\n",
				grid->modify_sector->id,
				ft_lstlen(grid->modify_sector->walls));
		curr = grid->modify_sector->walls;
		while (curr)
		{
			draw_wall_as_selected(grid,
				editor->grid.elem->active_surface, curr->content);
			curr = curr->next;
		}
	}
	bui_set_element_text(editor->selected_sector_info, str);
	ft_strdel(&str);
}

void	select_sector(t_grid *grid)
{
	t_list		*curr;
	t_sector	*temp;
	t_sector	*sec;

	temp = NULL;
	curr = grid->sectors;
	while (curr)
	{
		sec = curr->content;
		if (gfx_hitbox_square(
				grid->elem->libui->mouse_x - grid->elem->position.x,
				grid->elem->libui->mouse_y - grid->elem->position.y,
				(t_xywh){(int)sec->center.x - (grid->gap),
				(int)sec->center.y - (grid->gap),
				grid->gap * 2, grid->gap * 2}))
		{
			temp = curr->content;
			break ;
		}
		curr = curr->next;
	}
	if (temp == NULL)
		return ;
	grid->modify_sector = temp;
}
