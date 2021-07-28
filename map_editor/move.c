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
