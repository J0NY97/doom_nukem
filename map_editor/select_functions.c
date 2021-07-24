/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/15 09:52:36 by jsalmi           ###   ########.fr       */
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
		grid->gap = ft_clamp(grid->gap + editor->libui->mouse_wheel_y, 2, 32);
	if (move_x == 0.0f && move_y == 0.0f)
		return ;
	movement(editor, move_x, move_y);
}

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
	bui_set_element_text(editor->general_info, str, 0, 0);
	ft_strdel(&str);
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
	ft_strdel(&s);
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

void	select_point(t_grid *grid)
{
	t_point	*point;

	point = get_point_from_list_around_radius(grid->points, grid->hover, 1.0f);
	if (point)
		grid->modify_point = point;
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

void	select_wall(t_editor *editor)
{
	t_wall	*wall;

	wall = get_wall_from_list_around_radius(editor->grid.walls,
			editor->grid.hover, 1.0f);
	if (wall)
	{
		editor->grid.modify_wall = wall;
		editor->grid.modify_sprite = NULL;
	}
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

t_wall	*get_wall_from_list_around_radius(
		t_list *walls, t_vector pos, float allowed_radius)
{
	t_list		*curr;
	t_wall		*wall;
	float		dist;

	curr = walls;
	while (curr)
	{
		wall = curr->content;
		dist = fabs(distance_from_vector_to_wall(pos, curr->content));
		if (dist <= allowed_radius)
		{
			if (pos.x >= fmin(wall->orig->pos.x, wall->dest->pos.x)
				- allowed_radius && pos.x <= fmax(wall->orig->pos.x,
					wall->dest->pos.x) + allowed_radius)
			{
				if (pos.y >= fmin(wall->orig->pos.y,
						wall->dest->pos.y) - allowed_radius && pos.y
					<= fmax(wall->orig->pos.y,
						wall->dest->pos.y) + allowed_radius)
					return (curr->content);
			}
		}
		curr = curr->next;
	}
	return (NULL);
}

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

void	draw_selected_wall(t_grid *grid)
{
	if (grid->modify_wall == NULL)
		return ;
	draw_wall_as_selected(grid,
		grid->elem->active_surface, grid->modify_wall);
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
				get_list_len(grid->modify_sector->walls));
		curr = grid->modify_sector->walls;
		while (curr)
		{
			draw_wall_as_selected(grid,
				editor->grid.elem->active_surface, curr->content);
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
