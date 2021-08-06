/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:44:08 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:44:10 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

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
	pos->x += move_x;
	pos->y += move_y;
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

int	move_things(t_editor *editor, int move_x, int move_y)
{
	if (editor->grid.modify_point != NULL)
		move_pos(&editor->grid.modify_point->pos, move_x, move_y);
	else if (editor->grid.modify_wall != NULL)
	{
		move_pos(&editor->grid.modify_wall->orig->pos, move_x, move_y);
		move_pos(&editor->grid.modify_wall->dest->pos, move_x, move_y);
	}
	else if (editor->grid.modify_sector != NULL)
		move_sector(editor, move_x, move_y);
	else if (editor->grid.modify_entity != NULL)
		move_pos(&editor->grid.modify_entity->pos, move_x, move_y);
	else
		return (0);
	return (1);
}

int	movement(t_editor *editor)
{
	float	move_x;
	float	move_y;

	move_x = 0.0f;
	move_y = 0.0f;
	if (!(SDL_GetWindowFlags(editor->window->win) & SDL_WINDOW_MOUSE_FOCUS))
		return (0);
	if (mouse_pressed(editor->libui, SDL_BUTTON_RIGHT))
	{
		move_x = editor->grid.hover.x - editor->grid.last_hover.x;
		move_y = editor->grid.hover.y - editor->grid.last_hover.y;
	}
	if (key_pressed(editor->libui, SDL_SCANCODE_LCTRL))
	{
		if (editor->grid.elem->was_hovered_last_frame
			&& editor->libui->mouse_wheel_y != 0)
			editor->grid.gap = ft_clamp(editor->grid.gap
					+ editor->libui->mouse_wheel_y, 2, 32);
		else if (move_x != 0.0f || move_y != 0.0f)
			move_map(editor, move_x, move_y);
		return (1);
	}
	if (move_x == 0.0f && move_y == 0.0f)
		return (0);
	return (move_things(editor, move_x, move_y));
}
