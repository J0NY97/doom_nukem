/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/05/17 18:28:36 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_point	*get_point_from_list(t_list *list, t_point *v)
{
	t_list	*curr;

	curr = list;
	while (curr)
	{
		if (vector_compare(((t_point *)curr->content)->pos,
		(t_vector){v->pos.x, v->pos.y, v->pos.z}))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

t_wall	*get_wall_from_list(t_list **list, t_point *v1, t_point *v2)
{
	t_list	*curr;
	t_wall	*wall;

	curr = *list;
	while (curr)
	{
		wall = curr->content;
		if ((vector_compare(wall->orig->pos, v1->pos)
		&& vector_compare(wall->dest->pos, v2->pos))
		|| (vector_compare(wall->orig->pos, v2->pos)
		&& vector_compare(wall->dest->pos, v1->pos)))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

int	vector_in_wall(t_vector v, t_wall *vec)
{
	if (vector_compare(v, vec->orig->pos)
	|| vector_compare(v, vec->dest->pos))
		return (1);
	return (0);
}

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
		low_x = min(((t_point *)curr->content)->pos.x, low_x);
		low_y = min(((t_point *)curr->content)->pos.y, low_y);
		hi_x = max(((t_point *)curr->content)->pos.x, hi_x);
		hi_y = max(((t_point *)curr->content)->pos.y, hi_y);
		curr = curr->next;
	}
	grid->dimensions.x = low_x * grid->gap;
	grid->dimensions.y = low_y * grid->gap;
	grid->dimensions.w = hi_x * grid->gap;
	grid->dimensions.h = hi_y * grid->gap;
}

t_point	*get_point_from_wall_in_sector(t_sector *sector, t_point *v)
{
	t_wall	*w;
	t_list	*wall;

	wall = sector->walls;
	while (wall)
	{
		w = wall->content;
		if (vector_compare(w->orig->pos, v->pos))
			return (w->orig);	
		else if (vector_compare(w->dest->pos, v->pos))
			return (w->dest);
		wall = wall->next;
	}
	return (NULL);
}

t_point	*get_existing_point_or_new(t_grid *grid, t_vector pos)
{
	t_point	*point;

	point = get_point_from_wall_in_sector(grid->modify_sector,
			&(t_point){0, pos});
	if (point == NULL)
	{
		point = new_point(pos);
		add_to_list(&grid->points, point, sizeof(t_point));
	}
	return (point);
}

t_wall	*get_existing_wall_or_new(t_grid *grid, t_point *temp1, t_point *temp2)
{
	t_wall	*wall;

	wall = NULL;
	wall = get_wall_from_list(&grid->modify_sector->walls, temp1, temp2);
	if (wall == NULL)
	{
		wall = new_wall(temp1, temp2);
		add_to_list(&grid->walls, wall, sizeof(t_wall));
		add_to_list(&grid->modify_sector->walls, wall, sizeof(t_wall));
	}
	return (wall);
}

void	check_selected(t_grid *grid)
{
	t_point	*temp1;
	t_point	*temp2;
	t_wall	*temp_wall;

	temp1 = NULL;
	temp2 = NULL;
	temp_wall = NULL;
	if (vector_is_empty(grid->selected2))
		return ;
	temp1 = get_existing_point_or_new(grid, grid->selected1);
	temp2 = get_existing_point_or_new(grid, grid->selected2);
	temp_wall = get_existing_wall_or_new(grid, temp1, temp2);
	if (grid->modify_sector->first_point == NULL)
		grid->modify_sector->first_point = temp1;
	if (grid->modify_sector->first_point == temp_wall->dest)
	{
		grid->modify_sector->first_point = NULL;
		grid->modify_sector = NULL;
		grid->selected1 = EMPTY_VEC;
		grid->selected2 = EMPTY_VEC;
	}
	grid->selected1 = grid->selected2;
	grid->selected2 = EMPTY_VEC;
}

void	click_calc_sec(t_grid *grid)
{
	t_sector	*sector;

	if (grid->modify_sector == NULL)
	{
		sector = new_sector(grid->sector_amount++);
		add_to_list(&grid->sectors, sector, sizeof(t_sector));
		grid->modify_sector = sector;
	}
	if (vector_is_empty(grid->selected1))
		grid->selected1 = grid->hover;
	else if (!vector_compare(grid->selected1, grid->hover))
		grid->selected2 = grid->hover;
}

void	click_calc_ent(t_editor *editor)
{
	t_entity	*entity;

	entity = new_entity(editor->grid.entity_amount++, editor->grid.hover);
	entity->preset = get_entity_preset_with_name(
		editor->entity_presets, "Barrel");
	add_to_list(&editor->grid.entities, entity, sizeof(t_entity));
}

void	click_calc(t_editor *editor, t_grid *grid)
{

	if (!mouse_hover(editor->libui, (t_xywh) {
	grid->elem->position.x, grid->elem->position.y,
	grid->elem->position.w, grid->elem->position.h}))
		return ;
	if (editor->libui->mouse_down_last_frame &&
	mouse_pressed(editor->libui, MKEY_LEFT))
		click_calc_sec(grid);
	else if (editor->libui->mouse_down_last_frame &&
	mouse_pressed(editor->libui, MKEY_RIGHT))
		click_calc_ent(editor);
	else if (mouse_pressed(editor->libui, MKEY_MIDDLE))
		editor->spawn.pos = grid->hover;
}

void	unselect_selected(t_editor *editor, t_grid *grid)
{
	if (key_pressed(editor->libui, KEY_B))
	{
		grid->selected1 = EMPTY_VEC;
		grid->selected2 = EMPTY_VEC;
		grid->modify_point = NULL;
		grid->modify_wall = NULL;
		grid->modify_entity = NULL;
		editor->grid.modify_sprite = NULL;
		grid->modify_sector = NULL;
	}
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

	fill_surface(grid->elem->active_surface,
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

void	draw_points(t_grid *grid, t_list *points)
{
	t_list *curr;

	curr = points;
	while (curr)
	{
		gfx_draw_vector(grid->elem->active_surface, 0xff00ff00, 1,
			gfx_vector_multiply(((t_point *)curr->content)->pos,
			grid->gap));
		curr = curr->next;
	}
}

void	draw_wall(t_wall *wall, t_grid *grid, Uint32 color)
{
	t_vector orig_vec;
	t_vector dest_vec;

	orig_vec = gfx_vector_multiply(wall->orig->pos, grid->gap);
	dest_vec = gfx_vector_multiply(wall->dest->pos, grid->gap);
	gfx_draw_line(grid->elem->active_surface, color, orig_vec, dest_vec);
}

void	draw_walls(t_grid *grid, t_list **walls, Uint32 color)
{
	t_list *curr;

	curr = *walls;
	while (curr)
	{
		gfx_draw_line(grid->elem->active_surface, color,
			((t_wall *)curr->content)->orig->pos,
			((t_wall *)curr->content)->dest->pos);
		curr = curr->next;
	}
}

void	draw_sector_number(t_sector *sector, t_grid *grid, float x, float y)
{
	char		*str;
	SDL_Surface	*id_text;

	sector->center = (t_vector) {x, y, 0};
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
	int	i;
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
		if (((t_wall *)wall->content)->neighbor != -1)
			draw_wall(wall->content, grid, 0xffff0000);
		else
			draw_wall(wall->content, grid, sector->color);
		wall = wall->next;
	}
	i = get_list_len(&sector->walls) * 2;
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
	if (grid->modify_sector != NULL &&
	grid->modify_sector->first_point != NULL)
		gfx_draw_vector(grid->elem->active_surface, 0xffff0000, 2,
		gfx_vector_multiply(grid->modify_sector->first_point->pos,
		grid->gap));
}

void	draw_entity(t_editor *editor, t_entity *entity)
{
	t_vector	pos;
	int		color;
	float		angle;

	if (entity->preset == NULL)
		entity->preset = editor->default_entity;
	pos = gfx_vector_multiply(entity->pos, editor->grid.gap);
	gfx_draw_vector(editor->grid.elem->active_surface, 0xffaaab5d, 6, pos);
	color = 0xff0000ff;
	if (entity->preset->mood == ENTITY_TYPE_HOSTILE)
		color = 0xffff0000;
	else if (entity->preset->mood == ENTITY_TYPE_FRIENDLY)
		color = 0xff00ff00;
	gfx_draw_vector(editor->grid.elem->active_surface, color, 3, pos);
	angle = entity->direction * (M_PI / 180);
	gfx_draw_vector(editor->grid.elem->active_surface, 0xffaaab5d, 1,
		gfx_new_vector(cos(angle) * 10.0f + pos.x,
		sin(angle) * 10.0f + pos.y, 0));
}

void	draw_entities(t_editor *editor)
{
	t_list		*curr;

	curr = editor->grid.entities;
	while (curr)
	{
		draw_entity(editor, curr->content);
		curr = curr->next;
	}
}

void	draw_hover_info(t_editor *doom, t_grid *grid)
{
	char	*str;
	
	str = ft_sprintf("%d, %d\nzoom: %d\n", (int)grid->hover.x,
		(int)grid->hover.y, (int)grid->gap);
	doom->hover_info->text_color = 0xffffffff;
	bui_set_element_text(doom->hover_info, str, 0, 0);
	ft_strdel(&str);
}
