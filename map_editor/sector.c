/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:44:27 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 13:20:56 by jsalmi           ###   ########.fr       */
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
	sector->floor_texture = 0;
	sector->ceiling_texture = 0;
	sector->floor_texture_scale = 1.0f;
	sector->ceiling_texture_scale = 1.0f;
	sector->gravity = 9;
	sector->light_level = 10;
	sector->color = random_color();
	return (sector);
}

void	free_sector(void *content, size_t size)
{
	t_sector	*sector;

	ft_putstr("[free_sector]\n");
	(void)size;
	sector = content;
	if (sector == NULL)
		return ;
	ft_putstr("Starting to free walls from sector.\n");
	if (sector->walls)
		ft_lstdel(&sector->walls, &dummy_free_er);
	ft_putstr("All walls from sector freed.\n");
	free(sector);
}

void	remove_from_sectors(t_list **sectors, t_sector *sec)
{
	remove_from_list_if_with(sectors, sec, &pointer_compare, &free_sector);
}

int	sector_compare(t_sector *s1, t_sector *s2)
{
	if (s1->id == s2->id)
		return (1);
	return (0);
}

t_sector	*get_sector_with_wall(t_list **sector_list, t_wall *wall)
{
	t_list	*curr;
	t_list	*rruc;

	curr = *sector_list;
	while (curr)
	{
		rruc = ((t_sector *)curr->content)->walls;
		while (rruc)
		{
			if (rruc->content == wall)
				return (curr->content);
			rruc = rruc->next;
		}
		curr = curr->next;
	}
	return (NULL);
}

void	remove_all_non_existing_sectors(t_editor *editor)
{
	t_list		*sec;
	t_sector	*sector;

	sec = editor->grid.sectors;
	while (sec)
	{
		sector = sec->content;
		sec = sec->next;
		if (sector && ft_lstlen(sector->walls) == 0)
			remove_from_sectors(&editor->grid.sectors, sector);
	}
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
	if (i == 0)
		return ;
	draw_sector_number(sector, grid, x /= i, y /= i);
}

void	draw_sectors(t_grid *grid)
{
	t_list	*curr;

	curr = grid->sectors;
	while (curr)
	{
		draw_sector(curr->content, grid);
		curr = curr->next;
	}
	if (grid->modify_sector != NULL
		&& grid->modify_sector->first_point != NULL)
		gfx_draw_vector(grid->elem->active_surface, 0xffff0000, 2,
			gfx_vector_multiply(grid->modify_sector->first_point->pos,
				grid->gap));
}
