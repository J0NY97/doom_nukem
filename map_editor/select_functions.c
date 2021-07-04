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

void	movement(t_editor *doom, t_grid *grid, int move_x, int move_y)
{
	t_list *curr;

	if (grid->modify_point != NULL)
	{
		grid->modify_point->pos.x = grid->hover.x;
		grid->modify_point->pos.y = grid->hover.y;
	}
	else if (doom->grid.modify_sprite != NULL)
	{
		if (key_pressed(doom->libui, MKEY_RIGHT))
		{
			ft_printf("[Warning] Drag and drop not workings yet!\n");
		}
		else
		{
			if (key_pressed(doom->libui, KEY_LEFT))
				doom->grid.modify_sprite->pos.x -= (double)grid->gap;
			else if (key_pressed(doom->libui, KEY_RIGHT))
				doom->grid.modify_sprite->pos.x += (double)grid->gap;
			else if (key_pressed(doom->libui, KEY_UP))
				doom->grid.modify_sprite->pos.y -= (double)grid->gap;
			else if (key_pressed(doom->libui, KEY_DOWN))
				doom->grid.modify_sprite->pos.y += (double)grid->gap;
		}
	}
	else if (grid->modify_wall != NULL)
	{
		grid->modify_wall->orig->pos.x += move_x;
		grid->modify_wall->dest->pos.x += move_x;
		grid->modify_wall->orig->pos.y += move_y;
		grid->modify_wall->dest->pos.y += move_y;
	}
	else if (grid->modify_sector != NULL)
	{
		curr = grid->modify_sector->walls;
		while (curr)
		{
			((t_wall *)curr->content)->dest->pos.x += move_x / 2;
			((t_wall *)curr->content)->dest->pos.y += move_y / 2;
			((t_wall *)curr->content)->orig->pos.x += move_x / 2;
			((t_wall *)curr->content)->orig->pos.y += move_y / 2;
			curr = curr->next;
		}
	}
	else if (grid->modify_entity != NULL)
	{
		grid->modify_entity->pos.x = grid->hover.x;
		grid->modify_entity->pos.y = grid->hover.y;
	}
	else if (move_x != 0.0f || move_y != 0.0f) // moving whole map
	{
		t_point *point;

		curr = grid->points;
		while (curr)
		{
			point = curr->content;
			point->pos.x += move_x;
			point->pos.y += move_y;
			curr = curr->next;
		}

		t_entity *entity;
		curr = grid->entities;
		while (curr)
		{
			entity = curr->content;
			entity->pos.x += move_x;
			entity->pos.y += move_y;
			curr = curr->next;
		}

		doom->spawn.pos.x += move_x;
		doom->spawn.pos.y += move_y;
	}
}

void	drag_calc(t_editor *editor, t_grid *grid)
{
	float move_x;
	float move_y;

	move_x = 0.0f;
	move_y = 0.0f;
	if (key_pressed(editor->libui, MKEY_RIGHT))
	{
		move_x = grid->hover.x - grid->last_hover.x;
		move_y = grid->hover.y - grid->last_hover.y;
	}
	// scrolling
	// TOOD: clamp this so we get rid of the ternary
	if (grid->elem->was_hovered_last_frame && editor->libui->mouse_wheel_y != 0)
		grid->gap = grid->gap + editor->libui->mouse_wheel_y > 1 ? grid->gap + editor->libui->mouse_wheel_y : 2;
	// small optimization
	if (move_x == 0.0f && move_y == 0.0f)
		return ;
	movement(editor, grid, move_x, move_y);
}

// Get the nearest point in a radius of where you clicked.
void	select_point(t_grid *grid)
{
	t_point *temp;
	t_vector temp_pos;

	// 1. check if youre clicking exactly where there is a point, to make this fast.
	temp = get_point_from_list(grid->points, &(t_point){.pos = grid->hover}); // this is needed! dont remove.
	// 2. if theres no point exactly where you clicked. go here
	if (temp == NULL)
	{
		// 3. go through all the points and check if they are inside the radius of the wanted amount.
		// NOTE: for now returning the first found.
		// TODO: make this return the closest, it now returns the first found starting from top left.
		float allowed_radius = 1.0f; // pixels
		float x = -allowed_radius;
		float y = -allowed_radius;
		while (x <= allowed_radius)
		{
			while (y <= allowed_radius)
			{
				temp_pos = gfx_new_vector(grid->hover.x + x, grid->hover.y + y, grid->hover.z);
				temp = get_point_from_list(grid->points,
					&(t_point){
						.pos = temp_pos
					});
				if (temp != NULL)
				{
					ft_putstr("Point found at: ");
					gfx_vector_string(temp_pos);
					break ;
				}
				y += 0.5f;
			}
			if (temp != NULL)
				break ;
			y = -allowed_radius;
			x += 0.5f;
		}
		if (temp == NULL)
			return ;
	}
	grid->modify_point = temp;
ft_printf("Point selected.\n");
}

void	draw_selected_point(t_editor *editor, t_grid *grid)
{
	char	*str;

	if (grid->modify_point == NULL)
		str = ft_strdup("Selected Vector: \nNULL");
	else
	{
		str = ft_sprintf("Selected Vector:\nx %d\ny %d\nconnections: %d\n",
			(int)grid->modify_point->pos.x,
			(int)grid->modify_point->pos.y,
			get_point_connection_amount(&grid->walls, grid->modify_point));
		gfx_draw_vector(grid->elem->active_surface, 0xffffae42, 2,
			gfx_vector_multiply(grid->modify_point->pos, grid->gap));
	}
	editor->selected_vector_info->text_color = 0xffffffff;
	bui_set_element_text(editor->selected_vector_info, str, 0, 0);
	ft_strdel(&str);
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
	if (editor->libui->mouse_down_last_frame && mouse_pressed(editor->libui, MKEY_LEFT))
	{
		// Vertex
		if (bui_button_toggle(editor->select_mode_vertex))
			select_point(grid);
		else
			editor->grid.modify_point = NULL;
		// Wall
		if (bui_button_toggle(editor->select_mode_wall))
			select_wall(editor, grid);
		else
			editor->grid.modify_wall = NULL;
		// Entity
		if (bui_button_toggle(editor->select_mode_entity))
			select_entity(editor, grid);
		else
			editor->grid.modify_entity = NULL;
		// Sector
		if (bui_button_toggle(editor->select_mode_sector))
			select_sector(grid);
		else
			editor->grid.modify_sector = NULL;
		ft_putstr("Selection Done.\n");
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

void	select_wall(t_editor *doom, t_grid *grid)
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
	doom->grid.modify_sprite = NULL;
	grid->modify_wall = temp;
printf("Wall selected.\n");
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
	t_list *curr_wall;
	t_wall *wall;
	t_vector orig;
	t_vector dest;
	char *str;

	if (grid->modify_sector == NULL)
		str = ft_strdup("Selected Sector: \nNULL");
	else
	{
		str = ft_sprintf("Selected Sector:\nid: %d\nwalls: %d\n",
			grid->modify_sector->id,
			get_list_len(&grid->modify_sector->walls));
		curr_wall = grid->modify_sector->walls;
		while (curr_wall)
		{
			wall = curr_wall->content;
			orig = gfx_vector_add(gfx_vector_multiply(wall->orig->pos, grid->gap), 1);
			dest = gfx_vector_add(gfx_vector_multiply(wall->dest->pos, grid->gap), 1);
			gfx_draw_line(grid->elem->active_surface, 0xffffae42, orig, dest);
			orig = gfx_vector_sub(orig, 2);
			dest = gfx_vector_sub(dest, 2);
			gfx_draw_line(grid->elem->active_surface, 0xffffae42, orig, dest);
			curr_wall = curr_wall->next;
		}
	}
	editor->selected_sector_info->text_color = 0xffffffff;
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
ft_printf("Sector selected.\n");
}

// Basically same as select_point... when you have that normed, you can easily norme this.
void	select_entity(t_editor *editor, t_grid *grid)
{
	t_entity *temp;
        t_vector temp_pos;

        // 1. check if youre clicking exactly where there is a point, to make this fast.
        temp = get_entity_from_list_at_pos(editor->grid.entities, editor->grid.hover);
        // 2. if theres no point exactly where you clicked. go here
        if (temp == NULL)
        {
                // 3. go through all the points and check if they are inside the radius of the wanted amount.
                // NOTE: for now returning the first found.
                // TODO: make this return the closest, it now returns the first found starting from top left.
                float allowed_radius = 1.0f; // pixels
                float x = -allowed_radius;
                float y = -allowed_radius;
                while (x <= allowed_radius)
                {
                        while (y <= allowed_radius)
                        {
                                temp_pos = gfx_new_vector(grid->hover.x + x, grid->hover.y + y, grid->hover.z);
                                temp = get_entity_from_list_at_pos(editor->grid.entities, temp_pos);
                                if (temp != NULL)
                                {
                                        ft_putstr("Entity found at: ");
                                        gfx_vector_string(temp_pos);
                                        break ;
                                }
                                y += 0.5f;
                        }
                        if (temp != NULL)
                                break ;
                        y = -allowed_radius;
                        x += 0.5f;
                }
                if (temp == NULL)
                        return ;
        }
	editor->grid.modify_entity = temp;
	// TODO: this is temporary fix to the drop being open and automatically making the new edit entity same as the 
	// 	previous entity you edited.
	editor->entity_type_drop->drop->toggle = 0;	
ft_printf("Entity selected.\n");
}

void	draw_selected_entity(t_grid *grid)
{
	if (grid->modify_entity == NULL)
		return ;
	gfx_draw_vector(grid->elem->active_surface, 0xffff0000, 8,
		gfx_vector_multiply(grid->modify_entity->pos, grid->gap));
}
