/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 15:36:48 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	update_real_dimensions(t_grid *grid)
{
	float	low_x;
	float	low_y;
	float	hi_x;
	float	hi_y;
	t_list	*curr;

	low_x = grid->elem->active_surface->w;
	low_y = grid->elem->active_surface->h;
	hi_x = -1;
	hi_y = -1;
	curr = grid->points;
	while (curr)
	{
		low_x = ft_min(((t_point *)curr->content)->pos.x, low_x);
		low_y = ft_min(((t_point *)curr->content)->pos.y, low_y);
		hi_x = ft_max(((t_point *)curr->content)->pos.x, hi_x);
		hi_y = ft_max(((t_point *)curr->content)->pos.y, hi_y);
		curr = curr->next;
	}
	grid->dimensions.x = low_x * grid->gap;
	grid->dimensions.y = low_y * grid->gap;
	grid->dimensions.w = hi_x * grid->gap;
	grid->dimensions.h = hi_y * grid->gap;
}

void	hover_calc(t_editor *doom, t_grid *grid)
{
	int	x;
	int	y;
	int	real_x;
	int	real_y;
	int	gap;

	if (SDL_GetMouseFocus() != doom->window->win)
		return ;
	grid->last_hover = grid->hover;
	SDL_GetMouseState(&x, &y);
	gap = grid->gap;
	real_x = (((x - grid->elem->position.x) / (gap / 2)) * (gap / 2)) / gap;
	real_y = (((y - grid->elem->position.y) / (gap / 2)) * (gap / 2)) / gap;
	grid->hover = gfx_new_vector(real_x, real_y, 0);
	gfx_draw_vector(grid->elem->active_surface, 0xffffffff, 1,
		gfx_vector_multiply(grid->hover, grid->gap));
}

void	draw_dimensions(t_grid *grid)
{
	gfx_draw_line(grid->elem->active_surface, 0xff999966,
		(t_vector){0, grid->dimensions.y, 0},
		(t_vector){grid->elem->active_surface->w,
		grid->dimensions.y, 0});
	gfx_draw_line(grid->elem->active_surface, 0xff999966,
		(t_vector){grid->dimensions.x, 0, 0},
		(t_vector){grid->dimensions.x,
		grid->elem->active_surface->h, 0});
	gfx_draw_line(grid->elem->active_surface, 0xff999966,
		(t_vector){0, grid->dimensions.h, 0},
		(t_vector){grid->elem->active_surface->w,
		grid->dimensions.h, 0});
	gfx_draw_line(grid->elem->active_surface, 0xff999966,
		(t_vector){grid->dimensions.w, 0, 0},
		(t_vector){grid->dimensions.w,
		grid->elem->active_surface->h, 0});
}

void	draw_grid(t_editor *doom, t_grid *grid)
{
	int	i;
	int	max;

	SDL_FillRect(grid->elem->active_surface, NULL,
		((t_bui_window *)grid->elem->parent)->color);
	i = -1;
	max = grid->elem->active_surface->h / grid->gap;
	while (i++ < max)
		gfx_draw_line(grid->elem->active_surface,
			doom->palette.elem_elem,
			(t_vector){0, i * grid->gap, 0},
			(t_vector){grid->elem->active_surface->w,
			i * grid->gap, 0});
	i = -1;
	max = grid->elem->active_surface->w / grid->gap;
	while (i++ < max)
		gfx_draw_line(grid->elem->active_surface,
			doom->palette.elem_elem,
			(t_vector){i * grid->gap, 0, 0},
			(t_vector){i * grid->gap,
			grid->elem->active_surface->h, 0});
	draw_dimensions(grid);
}

void	draw_hover_info(t_editor *doom, t_grid *grid)
{
	char	*str;

	str = ft_sprintf("%d, %d\nzoom: %d\n", (int)grid->hover.x,
			(int)grid->hover.y, (int)grid->gap);
	doom->hover_info->text_color = 0xffffffff;
	bui_set_element_text(doom->hover_info, str);
	ft_strdel(&str);
}
