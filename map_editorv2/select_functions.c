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
	else if (doom->option.modify_sprite != NULL)
	{
		if (key_pressed(doom->libui, KEY_LEFT))
			doom->option.modify_sprite->pos.x -= (double)grid->gap;
		else if (key_pressed(doom->libui, KEY_RIGHT))
			doom->option.modify_sprite->pos.x += (double)grid->gap;
		else if (key_pressed(doom->libui, KEY_UP))
			doom->option.modify_sprite->pos.y -= (double)grid->gap;
		else if (key_pressed(doom->libui, KEY_DOWN))
			doom->option.modify_sprite->pos.y += (double)grid->gap;
		else if (key_pressed(doom->libui, KPKEY_PLUS))
		{
			doom->option.modify_sprite->w += 5.0f;
			doom->option.modify_sprite->h += 5.0f;
		}
		else if (key_pressed(doom->libui, KPKEY_MINUS))
		{
			doom->option.modify_sprite->w -= 5.0f;
			doom->option.modify_sprite->h -= 5.0f;
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

void	select_point(t_editor *doom, t_grid *grid, SDL_Event *e)
{
	t_point *temp;

	temp = get_point_from_list(grid, &(t_point){.pos = grid->hover});
	if (temp == NULL)
		return ;
	grid->modify_point = temp;
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

void	selection(t_editor *doom, t_grid *grid, SDL_Event *e)
{
	if (mouse_pressed(doom->libui, MKEY_LEFT))
	{
		if (grid->modify_wall == NULL && grid->modify_sector == NULL && grid->modify_entity == NULL)
			select_point(doom, grid, e);
		if (grid->modify_point == NULL && grid->modify_sector == NULL && grid->modify_entity == NULL)
			select_wall(doom, grid, e);
		if (grid->modify_wall == NULL && grid->modify_point == NULL && grid->modify_sector == NULL)
			select_entity(doom, grid, e);
		if (grid->modify_wall == NULL && grid->modify_point == NULL && grid->modify_entity == NULL)
			select_sector(doom, grid, e);
	}
}

void	select_wall(t_editor *doom, t_grid *grid, SDL_Event *e)
{
	t_list		*curr;
	t_wall		*temp;

	temp = NULL;
	curr = grid->walls;
	while (curr)
	{
		// @Improvement: if you dont want it to be pixel perfect look into (Point-Line Distance--2-Dimensional)
		if (vector_on_wall(grid->hover, curr->content))
		{
			temp = curr->content;
			break ;
		}
		curr = curr->next;
	}
	if (temp == NULL)
		return ;
	doom->option.modify_sprite = NULL;
	grid->modify_wall = temp;
printf("Wall selected.\n");
}

void	draw_selected_wall(t_editor *doom, t_grid *grid)
{
	SDL_Surface *text;
	SDL_Rect	temp;
	char		*str;
	TTF_Font	*font;
	int			margin;

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
	margin = 25;
	str = ft_sprintf("Selected Line:\norig:\tdest:\nx %.1f\tx %.1f\ny %.1f\ty %.1f\n",
					grid->modify_wall->orig->pos.x, grid->modify_wall->dest->pos.x,
					grid->modify_wall->orig->pos.y, grid->modify_wall->dest->pos.y);
	font = TTF_OpenFont("../libui/TTF/font.ttf", 20);
	text = TTF_RenderText_Blended_Wrapped(font, str, (SDL_Color) {255, 255, 255, 255}, doom->option.info->active_surface->w - (margin * 2));
	temp.x = margin;
	temp.y = margin;
	temp.w = text->w;
	temp.h = text->h;
	SDL_BlitSurface(text, NULL, doom->option.info->active_surface, &temp);
	ft_strdel(&str);
	TTF_CloseFont(font);
	SDL_FreeSurface(text);
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

void	select_sector(t_editor *doom, t_grid *grid, SDL_Event *e)
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

void	select_entity(t_editor *doom, t_grid *grid, SDL_Event *e)
{
	t_list		*curr;
	t_entity	*temp;

	temp = NULL;
	curr = grid->entities;
	while (curr)
	{
		t_entity *ent;

		ent = curr->content;
		if (vector_compare(ent->pos, grid->hover))
		{
			temp = ent;
			break ;
		}
		curr = curr->next;
	}
	if (temp == NULL)
		return ;
	grid->modify_entity = temp;
// when entity is chosen you have to populate the values in the edit screen, pretty spaghett but...
// TODO: you cant edit the text of elements yet, when you can fix this.
/*
	char *str;

	str = ft_sprintf("id: %d\n", grid->modify_entity->id);
	ft_set_text(&doom->option.ent_info_id_text->text, str);
	ft_strdel(&str);

	str = ft_itoa(grid->modify_entity->max_health);
	ft_set_text(&doom->option.ent_info_health_text_area->text, str);
	ft_strdel(&str);

	str = ft_itoa(grid->modify_entity->speed);
	ft_set_text(&doom->option.ent_info_speed_text_area->text, str);
	ft_strdel(&str);

	str = ft_itoa(grid->modify_entity->armor);
	ft_set_text(&doom->option.ent_info_armor_text_area->text, str);
	ft_strdel(&str);
	*/

	// TODO: drop down doesnt work yet, when it does fix this.
	/*
	int index = 0;
	SDL_Event eb;
	curr = ((t_drop_down *)doom->option.type_dropdown->info)->items;
	while (curr)
	{
		if (index == grid->modify_entity->type)
			ft_drop_item_function(eb, curr->content);
		index++;
		curr = curr->next;
	}
	*/
ft_printf("Entity selected.\n");
}

void	draw_selected_entity(t_editor *doom, t_grid *grid)
{
	if (grid->modify_entity == NULL)
		return ;
	gfx_draw_vector(grid->elem->active_surface, 0xffff0000, 8, gfx_vector_multiply(grid->modify_entity->pos, grid->gap));
}
