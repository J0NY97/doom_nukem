/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 15:07:25 by jsalmi            #+#    #+#             */
/*   Updated: 2020/09/27 16:30:53 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	drag_calc(t_editor *doom, t_grid *grid, SDL_Event *e)
{
	t_list *curr;
	float move_x = 0.0f;
	float move_y = 0.0f;

	if (key_pressed(doom->libui, KEY_LEFT))
		move_x = -0.5f;
	else if (key_pressed(doom->libui, KEY_RIGHT))
		move_x = 0.5f;
	if (key_pressed(doom->libui, KEY_UP))
		move_y = -0.5f;
	else if (key_pressed(doom->libui, KEY_DOWN))
		move_y = 0.5f;
	if (grid->modify_point != NULL) // vector movement
	{
		grid->modify_point->pos.x += move_x;
		grid->modify_point->pos.y += move_y;
	}
	else if (doom->grid.modify_sprite != NULL)
	{
		if (key_pressed(doom->libui, KEY_LEFT))
			doom->grid.modify_sprite->pos.x -= (double)grid->gap;
		else if (key_pressed(doom->libui, KEY_RIGHT))
			doom->grid.modify_sprite->pos.x += (double)grid->gap;
		else if (key_pressed(doom->libui, KEY_UP))
			doom->grid.modify_sprite->pos.y -= (double)grid->gap;
		else if (key_pressed(doom->libui, KEY_DOWN))
			doom->grid.modify_sprite->pos.y += (double)grid->gap;
		else if (key_pressed(doom->libui, KPKEY_PLUS))
		{
			doom->grid.modify_sprite->w += 5.0f;
			doom->grid.modify_sprite->h += 5.0f;
		}
		else if (key_pressed(doom->libui, KPKEY_MINUS))
		{
			doom->grid.modify_sprite->w -= 5.0f;
			doom->grid.modify_sprite->h -= 5.0f;
		}
	}
	else if (grid->modify_wall != NULL) // wall movement
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
			((t_wall *)curr->content)->dest->pos.x += move_x;
			((t_wall *)curr->content)->dest->pos.y += move_y;
			((t_wall *)curr->content)->orig->pos.x += move_x;
			((t_wall *)curr->content)->orig->pos.y += move_y;
			curr = curr->next;
		}
	}
	else if (grid->modify_entity != NULL)
	{
		grid->modify_entity->pos.x += move_x;
		grid->modify_entity->pos.y += move_y;
	}
	else if (move_x != 0.0f || move_y != 0.0f)
	{
		ft_putstr("are we here\n");
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
	// scrolling
	else if (grid->elem->was_hovered_last_frame && doom->libui->mouse_wheel_y != 0)
	{
		// TODO: figure out why this cant go under 2... not even 1
		grid->gap = grid->gap + doom->libui->mouse_wheel_y > 1 ? grid->gap + doom->libui->mouse_wheel_y : 2;
		ft_printf("gap: %d\n",grid->gap);
	}
}

// Get the nearest point in a radius of where you clicked.
void	select_point(t_editor *doom, t_grid *grid)
{
	t_point *temp;
	t_vector temp_pos;

	// 1. check if youre clicking exactly where there is a point, to make this fast.
	temp = get_point_from_list(grid->points, &(t_point){.pos = grid->hover});
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

	/* OLD VERSION
	t_point *temp;

	temp = get_point_from_list(grid->points, &(t_point){.pos = grid->hover});
	if (temp == NULL)
		return ;
	grid->modify_point = temp;
	END OLD VERSION */ 
ft_printf("Point selected.\n");
}

void	draw_selected_point(t_editor *doom, t_grid *grid)
{
	SDL_Surface *text;
	SDL_Rect	temp;
	char		*str;
	TTF_Font	*font;
	int			margin;

	if (grid->modify_point == NULL)
		return ;
	gfx_draw_vector(grid->elem->active_surface, 0xffffae42, 2, gfx_vector_multiply(grid->modify_point->pos, grid->gap));
	margin = 10;
	str = ft_sprintf("Selected Vector:\nx %d\ny %d\nconnections: %d\n", (int)grid->modify_point->pos.x, (int)grid->modify_point->pos.y, get_point_connection_amount(&grid->walls, grid->modify_point));
	font = TTF_OpenFont("../libui/TTF/font.ttf", 20);
	text = TTF_RenderText_Blended_Wrapped(font, str, (SDL_Color) {255, 255, 255, 255}, doom->info_area->active_surface->w - (margin * 2));
	temp.x = margin + 100;
	temp.y = margin;
	temp.w = text->w;
	temp.h = text->h;
	SDL_BlitSurface(text, NULL, doom->info_area->active_surface, &temp);
	ft_strdel(&str);
	TTF_CloseFont(font);
	SDL_FreeSurface(text);
}

int		get_point_connection_amount(t_list **walls, t_point *point)
{
	t_list	*curr;
	int		amount;

	curr = *walls;
	amount = 0;
	while (curr)
	{
		if (vector_compare(((t_wall *)curr->content)->dest->pos, point->pos) ||
			vector_compare(((t_wall *)curr->content)->orig->pos, point->pos))
			amount++;
		curr = curr->next;
	}
	return (amount);
}

int		vector_on_wall(t_vector v, t_wall *wall)
{
	if (gfx_vector_dot(v, wall->orig->pos, wall->dest->pos) == 0)
	{
		if (v.x >= fmin(wall->orig->pos.x, wall->dest->pos.x) && v.x <= fmax(wall->orig->pos.x, wall->dest->pos.x))
		{
			if (v.y >= fmin(wall->orig->pos.y, wall->dest->pos.y) && v.y <= fmax(wall->orig->pos.y, wall->dest->pos.y))
				return (1);
		}
	}
	return (0);
}

void	selection(t_editor *editor, t_grid *grid, SDL_Event *e)
{
	if (editor->libui->mouse_down_last_frame && mouse_pressed(editor->libui, MKEY_LEFT))
	{
		// Vertex
		if (bui_button_toggle(editor->select_mode_vertex))
			select_point(editor, grid);
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
			select_sector(editor, grid);
		else
			editor->grid.modify_sector = NULL;
	}
}

// TODO: try to remove the pow()
float	distance_from_vector_to_wall(t_vector p0, t_wall *wall)
{
	t_vector p1;
	t_vector p2;
	float	dist;

	p1 = wall->orig->pos;
	p2 = wall->dest->pos;

	float up = (p2.x - p1.x) * (p1.y - p0.y) - (p1.x - p0.x) * (p2.y - p1.y);
	float down = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));

	dist = up / down;
	return (dist);
}

// @Improvement: if you dont want it to be pixel perfect look into (Point-Line Distance--2-Dimensional)
// TODO: this function now selects the first wall you come across that is <= allowed radius,
// 	make it go through all the walls and return the closest wall. 
void	select_wall(t_editor *doom, t_grid *grid)
{
	t_list		*curr;
	t_wall		*temp;
	t_wall		*wall;
	t_vector	v;
	float		allowed_radius = 2.0f;

	temp = NULL;
	v = grid->hover;
	curr = grid->walls;
	while (curr)
	{
		wall = curr->content;
		float dist = fabs(distance_from_vector_to_wall(v, curr->content));
		ft_printf("VectorToWall: %.1f\n", dist);
		if (dist <= allowed_radius)
		{
			if (v.x >= fmin(wall->orig->pos.x, wall->dest->pos.x) - allowed_radius &&
			v.x <= fmax(wall->orig->pos.x, wall->dest->pos.x) + allowed_radius)
			{
				if (v.y >= fmin(wall->orig->pos.y, wall->dest->pos.y) - allowed_radius &&
				v.y <= fmax(wall->orig->pos.y, wall->dest->pos.y) + allowed_radius)
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

	/* OLD VERSION
	t_list		*curr;
	t_wall		*temp;

	temp = NULL;
	curr = grid->walls;
	while (curr)
	{
		gfx_vector_string(grid->hover);
		if (vector_on_wall(grid->hover, curr->content))
		{
			temp = curr->content;
			break ;
		}
		curr = curr->next;
	}
	if (temp == NULL)
		return ;
	doom->grid.modify_sprite = NULL;
	grid->modify_wall = temp;
	END OLD VERSION */
printf("Wall selected.\n");
}

void	draw_selected_wall(t_editor *doom, t_grid *grid)
{
	if (grid->modify_wall == NULL)
		return ;
	t_vector orig = gfx_vector_multiply((t_vector){(grid->modify_wall->orig->pos.x),
											(grid->modify_wall->orig->pos.y), 0}, grid->gap);
	t_vector dest = gfx_vector_multiply((t_vector){(grid->modify_wall->dest->pos.x),
											(grid->modify_wall->dest->pos.y), 0}, grid->gap);
	orig.x += 1;
	orig.y += 1;
	dest.x += 1;
	dest.y += 1;
	gfx_draw_line(grid->elem->active_surface, 0xffffae42, orig, dest);
}

void	draw_selected_sector(t_editor *doom, t_grid *grid)
{
	t_list *curr_wall;

	if (grid->modify_sector == NULL)
		return ;
	curr_wall = grid->modify_sector->walls;
	while (curr_wall)
	{
		t_vector orig = gfx_vector_multiply((t_vector){(((t_wall *)curr_wall->content)->orig->pos.x),
											(((t_wall *)curr_wall->content)->orig->pos.y), 0}, grid->gap);
		t_vector dest = gfx_vector_multiply((t_vector){(((t_wall *)curr_wall->content)->dest->pos.x),
											(((t_wall *)curr_wall->content)->dest->pos.y), 0}, grid->gap);
		orig.x += 1;
		orig.y += 1;
		dest.x += 1;
		dest.y += 1;
		gfx_draw_line(grid->elem->active_surface, 0xffffae42, orig, dest);
		curr_wall = curr_wall->next;
	}
}

void	select_sector(t_editor *doom, t_grid *grid)
{
	t_list		*curr;
	t_sector		*temp;

	temp = NULL;

	curr = grid->sectors;
	while (curr)
	{
		t_sector *sec;

		sec = curr->content;
		gfx_draw_vector(grid->elem->active_surface, 0xffffff00, 3, sec->lowest_pos);
		gfx_draw_vector(grid->elem->active_surface, 0xffff00ff, 3, sec->highest_pos);
		if (gfx_hitbox_square(grid->hover.x, grid->hover.y, (t_xywh){(int)sec->lowest_pos.x, (int)sec->lowest_pos.y, (int)sec->highest_pos.x - (int)sec->lowest_pos.x, (int)sec->highest_pos.y - (int)sec->lowest_pos.y}))
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

void	draw_selected_entity(t_editor *doom, t_grid *grid)
{
	if (grid->modify_entity == NULL)
		return ;
	gfx_draw_vector(grid->elem->active_surface, 0xffff0000, 8, gfx_vector_multiply(grid->modify_entity->pos, grid->gap));
}
