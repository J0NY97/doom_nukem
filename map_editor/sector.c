/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:44:27 by jsalmi            #+#    #+#             */
/*   Updated: 2021/09/16 13:43:48 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_sector	*new_sector(int id)
{
	t_sector	*sector;

	sector = ft_memalloc(sizeof(t_sector));
	sector->id = id;
	sector->walls = NULL;
	sector->first_point = NULL;
	sector->floor_height = 0;
	sector->ceiling_height = 20;
	sector->floor_texture = 2;
	sector->ceiling_texture = 2;
	sector->floor_texture_scale = 10.0f;
	sector->ceiling_texture_scale = 10.0f;
	sector->gravity = 20;
	sector->light_level = 0;
	sector->color = random_color();
	return (sector);
}

void	free_sector(void *content, size_t size)
{
	t_sector	*sector;

	(void)size;
	sector = content;
	if (sector == NULL)
		return ;
	if (sector->walls)
		ft_lstdel(&sector->walls, &dummy_free_er);
	free(sector);
}

void	draw_sector_number(t_sector *sector, t_grid *grid, float x, float y)
{
	char		*str;
	SDL_Surface	*id_text;

	sector->center = (t_vector){x, y, 0};
	if (grid->font)
	{
		str = ft_itoa(sector->id);
		id_text = TTF_RenderText_Blended(grid->font, str,
				(SDL_Color){255, 255, 255, 255});
		SDL_BlitSurface(id_text, NULL, grid->elem->active_surface,
			&(SDL_Rect){x - (id_text->w / 2), y - (id_text->h / 2),
			id_text->w, id_text->h});
		SDL_FreeSurface(id_text);
		ft_strdel(&str);
	}
}

//	The New one
//void	draw_sector(t_sector *sector, t_grid *grid, float x, float y)
//{
//	float	a;
//	float	t;
//	t_list	*w;
//	t_wall	*wall;
//
//	a = 0.0f;
//	w = sector->walls;
//	while (w)
//	{
//		wall = w->content;
//		t = wall->orig->pos.x * wall->dest->pos.y * grid->gap
//			- wall->dest->pos.x * wall->orig->pos.y * grid->gap;
//		a += t;
//		x += ((wall->orig->pos.x + wall->dest->pos.x) * grid->gap) * t;
//		y += ((wall->orig->pos.y + wall->dest->pos.y) * grid->gap) * t;
//		if (wall->neighbor_sector != NULL)
//			draw_wall(wall, grid, 0xffff0000);
//		else
//			draw_wall(wall, grid, sector->color);
//		w = w->next;
//	}
//	x = x / (6.0 * (a * 0.5));
//	y = y / (6.0 * (a * 0.5));
//	draw_sector_number(sector, grid, x, y);
//}

void	draw_sector(t_sector *sector, t_grid *grid)
{
	int		i;
	float	x;
	float	y;
	t_list	*wall;
	t_wall	*w;

	x = 0;
	y = 0;
	wall = sector->walls;
	while (wall)
	{
		w = wall->content;
		x += (w->orig->pos.x + w->dest->pos.x) * grid->gap;
		y += (w->orig->pos.y + w->dest->pos.y) * grid->gap;
		if (((t_wall *)wall->content)->neighbor_sector != NULL)
			draw_wall(wall->content, grid, 0xffff0000);
		else
			draw_wall(wall->content, grid, sector->color);
		wall = wall->next;
	}
	i = ft_lstlen(sector->walls) * 2;
	if (i < 2)
		return ;
	//sector_center(sector, grid, &x, &y);
	draw_sector_number(sector, grid, x /= i, y /= i);
}

void	draw_sectors(t_grid *grid)
{
	t_list	*curr;

	curr = grid->sectors;
	while (curr)
	{
		//draw_sector(curr->content, grid, 0, 0);
		draw_sector(curr->content, grid);
		curr = curr->next;
	}
	if (grid->modify_sector != NULL
		&& grid->modify_sector->first_point != NULL)
		gfx_draw_vector(grid->elem->active_surface, 0xffff0000, 2,
			gfx_vector_multiply(grid->modify_sector->first_point->pos,
				grid->gap));
}
