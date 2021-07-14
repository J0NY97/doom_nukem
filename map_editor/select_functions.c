/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/05/17 16:36:29 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	move_wall(t_editor *editor, int move_x, int move_y)
{
	editor->grid.modify_wall->orig->pos.x += move_x;
	editor->grid.modify_wall->dest->pos.x += move_x;
	editor->grid.modify_wall->orig->pos.y += move_y;
	editor->grid.modify_wall->dest->pos.y += move_y;
}

void	move_sector(t_editor *editor, int move_x, int move_y)
{
	t_list	*curr;

	curr = editor->grid.modify_sector->walls;
	while (curr)
	{
		((t_wall *)curr->content)->dest->pos.x += move_x / 2;
		((t_wall *)curr->content)->dest->pos.y += move_y / 2;
		((t_wall *)curr->content)->orig->pos.x += move_x / 2;
		((t_wall *)curr->content)->orig->pos.y += move_y / 2;
		curr = curr->next;
	}
}

void	move_pos(t_vector *pos, int move_x, int move_y)
{
	pos->x = move_x;
	pos->y = move_y;
}

void	move_map(t_editor *editor, int move_x, int move_y)
{
	t_list		*curr;
	t_point		*point;
	t_entity	*entity;

	curr = editor->grid.points;
	while (curr)
	{
		point = curr->content;
		point->pos.x += move_x;
		point->pos.y += move_y;
		curr = curr->next;
	}
	curr = editor->grid.entities;
	while (curr)
	{
		entity = curr->content;
		entity->pos.x += move_x;
		entity->pos.y += move_y;
		curr = curr->next;
	}
	editor->spawn.pos.x += move_x;
	editor->spawn.pos.y += move_y;
}

void	movement(t_editor *editor, int move_x, int move_y)
{
	if (editor->grid.modify_point != NULL)
		move_pos(&editor->grid.modify_point->pos,
			editor->grid.hover.x, editor->grid.hover.y);
	else if (editor->grid.modify_wall != NULL)
		move_wall(editor, move_x, move_y);
	else if (editor->grid.modify_sector != NULL)
		move_sector(editor, move_x, move_y);
	else if (editor->grid.modify_entity != NULL)
		move_pos(&editor->grid.modify_entity->pos,
			editor->grid.hover.x, editor->grid.hover.y);
	else if (move_x != 0.0f || move_y != 0.0f)
		move_map(editor, move_x, move_y);
}

void	drag_calc(t_editor *editor, t_grid *grid)
{
	float	move_x;
	float	move_y;

	move_x = 0.0f;
	move_y = 0.0f;
	if ((SDL_GetWindowFlags(editor->window->win) & SDL_WINDOW_MOUSE_FOCUS)
		&& key_pressed(editor->libui, MKEY_RIGHT))
	{
		move_x = grid->hover.x - grid->last_hover.x;
		move_y = grid->hover.y - grid->last_hover.y;
	}
	if (grid->elem->was_hovered_last_frame
		&& editor->libui->mouse_wheel_y != 0)
		grid->gap = ft_clamp(grid->gap + editor->libui->mouse_wheel_y,
			2, 32);
	if (move_x == 0.0f && move_y == 0.0f)
		return ;
	movement(editor, move_x, move_y);
}

void	select_point(t_grid *grid)
{
	t_point		*temp;
	float		allowed_radius;
	float		x;
	float		y;

	temp = get_point_from_list(grid->points, &(t_point){.pos = grid->hover});
	if (temp == NULL)
	{
		allowed_radius = 1.0f;
		x = -allowed_radius;
		while (x <= allowed_radius)
		{
			y = -allowed_radius;
			while (y <= allowed_radius)
			{
				temp = get_point_from_list(grid->points,
					&(t_point){.pos = (t_vector){
					grid->hover.x + x, grid->hover.y + y, 0}});
				if (temp != NULL)
					break ;
				y += 0.5f;
			}
			if (temp != NULL)
				break ;
			x += 0.5f;
		}
		if (temp == NULL)
			return ;
	}
	grid->modify_point = temp;
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
	bui_set_element_text(editor->selected_vector_info, s, 0, 0);
	ft_strdel(&ss);
}

int	get_point_connection_amount(t_list **walls, t_point *point)
{
	t_list	*curr;
	int	amount;

	curr = *walls;
	amount = 0;
	while (curr)
	{
		if (vector_compare(((t_wall *)curr->content)->dest->pos,
			point->pos) || vector_compare(
			((t_wall *)curr->content)->orig->pos, point->pos))
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
	if (editor->libui->mouse_down_last_frame
		&& mouse_pressed(editor->libui, MKEY_LEFT))
	{
		if (bui_button_toggle(editor->select_mode_vertex))
			select_point(grid);
		else
			editor->grid.modify_point = NULL;
		if (bui_button_toggle(editor->select_mode_wall))
			select_wall(editor, grid);
		else
			editor->grid.modify_wall = NULL;
		if (bui_button_toggle(editor->select_mode_entity))
			select_entity(editor, grid);
		else
			editor->grid.modify_entity = NULL;
		if (bui_button_toggle(editor->select_mode_sector))
			select_sector(grid);
		else
			editor->grid.modify_sector = NULL;
	}
}

float	distance_from_vector_to_wall(t_vector p0, t_wall *wall)
{
	t_vector	p1;
	t_vector	p2;
	float		dist;
	float		up;
	float		down;

	p1 = wall->orig->pos;
	p2 = wall->dest->pos;
	up = (p2.x - p1.x) * (p1.y - p0.y)
		- (p1.x - p0.x) * (p2.y - p1.y);
	down = sqrt(ft_pow(p2.x - p1.x, 2) + ft_pow(p2.y - p1.y, 2));
	dist = up / down;
	return (dist);
}

void	select_wall(t_editor *editor, t_grid *grid)
{
	t_list		*curr;
	t_wall		*temp;
	t_wall		*wall;
	t_vector	v;
	float		allowed_radius;
	float		dist;

	allowed_radius = 1.0f;
	temp = NULL;
	v = grid->hover;
	curr = grid->walls;
	while (curr)
	{
		wall = curr->content;
		dist = fabs(distance_from_vector_to_wall(v, curr->content));
		if (dist <= allowed_radius)
		{
			if (v.x >= fmin(wall->orig->pos.x, wall->dest->pos.x)
			- allowed_radius && v.x <= fmax(wall->orig->pos.x,
			wall->dest->pos.x) + allowed_radius)
			{
				if (v.y >= fmin(wall->orig->pos.y,
				wall->dest->pos.y) - allowed_radius && v.y
				<= fmax(wall->orig->pos.y,
				wall->dest->pos.y) + allowed_radius)
				{
					temp = curr->content;
					break ;
				}
			}
		}
		curr = curr->next;
	}
	if (temp == NULL)
		return ;
	editor->grid.modify_sprite = NULL;
	grid->modify_wall = temp;
}

void	draw_selected_wall(t_grid *grid)
{
	t_vector orig;
	t_vector dest;

	if (grid->modify_wall == NULL)
		return ;
	orig = gfx_vector_multiply((t_vector){(grid->modify_wall->orig->pos.x),
			(grid->modify_wall->orig->pos.y), 0}, grid->gap);
	dest = gfx_vector_multiply((t_vector){(grid->modify_wall->dest->pos.x),
			(grid->modify_wall->dest->pos.y), 0}, grid->gap);
	orig = gfx_vector_add(orig, 1);
	dest = gfx_vector_add(dest, 1);
	gfx_draw_line(grid->elem->active_surface, 0xffffae42, orig, dest);
}

void	draw_selected_sector(t_editor *editor, t_grid *grid)
{
	t_list		*curr;
	t_vector	orig;
	t_vector	dest;
	char		*str;

	if (grid->modify_sector == NULL)
		str = ft_strdup("Selected Sector: \nNULL");
	else
	{
		str = ft_sprintf("Selected Sector:\nid: %d\nwalls: %d\n",
			grid->modify_sector->id,
			get_list_len(&grid->modify_sector->walls));
		curr = grid->modify_sector->walls;
		while (curr)
		{
			orig = gfx_vector_add(gfx_vector_multiply(((t_wall *)
				curr->content)->orig->pos, grid->gap), 1);
			dest = gfx_vector_add(gfx_vector_multiply(((t_wall *)
				curr->content)->dest->pos, grid->gap), 1);
			gfx_draw_line(grid->elem->active_surface,
				0xffffae42, orig, dest);
			orig = gfx_vector_sub(orig, 2);
			dest = gfx_vector_sub(dest, 2);
			gfx_draw_line(grid->elem->active_surface,
				0xffffae42, orig, dest);
			curr = curr->next;
		}
	}
	bui_set_element_text(editor->selected_sector_info, str, 0, 0);
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

void	select_entity(t_editor *editor, t_grid *grid)
{
	t_entity	*temp;
	float		allowed_radius;
	float		x;
	float		y;


	temp = get_entity_from_list_at_pos(editor->grid.entities,
		editor->grid.hover);
	if (temp == NULL)
	{
		allowed_radius = 1.0f;
		x = -allowed_radius;
		while (x <= allowed_radius)
		{
			y = -allowed_radius;
			while (y <= allowed_radius)
			{
				temp = get_entity_from_list_at_pos(
					editor->grid.entities, (t_vector){
					grid->hover.x + x, grid->hover.y + y,
					0});
				if (temp != NULL)
					break ;
				y += 0.5f;
			}
			if (temp != NULL)
				break ;
			x += 0.5f;
		}
		if (temp == NULL)
			return ;
	}
	editor->grid.modify_entity = temp;
	editor->entity_type_drop->drop->toggle = 0;	
}

void	draw_selected_entity(t_grid *grid)
{
	if (grid->modify_entity == NULL)
		return ;
	gfx_draw_vector(grid->elem->active_surface, 0xffff0000, 8,
		gfx_vector_multiply(grid->modify_entity->pos, grid->gap));
}
