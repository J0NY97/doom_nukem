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

t_point		*get_point_from_list(t_list *list, t_point *v)
{
	t_list *curr;

	curr = list;
	while (curr)
	{
		if (vector_compare(((t_point *)curr->content)->pos, (t_vector){v->pos.x, v->pos.y, v->pos.z}))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

t_wall		*get_wall_from_list(t_list **list, t_point *v1, t_point *v2)
{
	t_list *curr;
	t_wall *wall;

	curr = *list;
	while (curr)
	{
		wall = curr->content;
		if ((vector_compare(wall->orig->pos, v1->pos) && vector_compare(wall->dest->pos, v2->pos)) ||
			(vector_compare(wall->orig->pos, v2->pos) && vector_compare(wall->dest->pos, v1->pos)))
		{
			return (curr->content);
		}
		curr = curr->next;
	}
	return (NULL);
}

int		wall_in_list(t_wall *wall, t_list *wall_list)
{
	t_list *curr;

	curr = wall_list;
	while (curr)
	{
		if (wall_compare(curr->content, wall))
			return (1);
		curr = curr->next;
	}
	return (0);
}

// Calculates the amount of walls in a sector
int		get_sector_wall_amount(t_sector *sector)
{
	t_list *wall;
	int i;

	i = 0;
	wall = sector->walls;
	while (wall)
	{
		i++;
		wall = wall->next;
	}
	return (i);
}

// NOTE: Not to confuse with vector_on_line, which is vector math,
// 	this just checks if t_vector *v is either vec->dest or ->orig
int		vector_in_wall(t_vector v, t_wall *vec)
{
	if (vector_compare(v, vec->orig->pos) || vector_compare(v, vec->dest->pos))
		return (1);
	return (0);
}

void	split_wall(t_grid *grid, t_wall *old_wall, t_point *new_vec)
{
	t_wall	*temp;
	t_sector *sec_temp;

	temp = get_wall_from_list(&grid->walls, old_wall->dest, new_vec);
	if (temp == NULL)
		temp = get_wall_from_list(&grid->walls, old_wall->orig, new_vec);
	if (temp != NULL)
		return ;
	temp = new_wall(old_wall->dest, new_vec);
	add_to_list(&grid->walls, temp, sizeof(t_wall));
	old_wall->dest = new_vec;
	if (grid->modify_sector != NULL)
		add_to_list(&grid->modify_sector->walls, temp, sizeof(t_wall));
ft_printf("Wall split.\n");
}

void	update_real_dimensions(t_editor *doom, t_grid *grid)
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
		if (((t_point *)curr->content)->pos.x < low_x)
			low_x = ((t_point *)curr->content)->pos.x;
		if (((t_point *)curr->content)->pos.y < low_y)
			low_y = ((t_point *)curr->content)->pos.y;
		if (((t_point *)curr->content)->pos.x > hi_x)
			hi_x = ((t_point *)curr->content)->pos.x;
		if (((t_point *)curr->content)->pos.y > hi_y)
			hi_y = ((t_point *)curr->content)->pos.y;
		curr = curr->next;
	}
	grid->dimensions.x = low_x * grid->gap;
	grid->dimensions.y = low_y * grid->gap;
	grid->dimensions.w = hi_x * grid->gap;
	grid->dimensions.h = hi_y * grid->gap;
}

void	check_selected(t_editor *doom, t_grid *grid)
{
	t_point *temp1;
	t_point *temp2;
	t_wall *temp_wall;

	temp1 = NULL;
	temp2 = NULL;
	temp_wall = NULL;
	// if you have made a sector and want to add a point to the wall somewhere
	if (!vector_is_empty(grid->selected1) &&
		vector_is_empty(grid->selected2) &&
		grid->modify_sector != NULL && grid->modify_sector->first_point == NULL)
	{
		t_list *curr = grid->modify_sector->walls;
		while (curr)
		{
			if (vector_on_wall(grid->selected1, curr->content))
			{
				temp1 = (t_point *)malloc(sizeof(t_point));
				temp1->pos = grid->selected1;
				add_to_list(&grid->points, temp1, sizeof(t_point));
				split_wall(grid, curr->content, temp1);
				grid->selected1 = EMPTY_VEC;
				break ;
			}
			curr = curr->next;
		}
		return ;
	}
	if (vector_is_empty(grid->selected1) || vector_is_empty(grid->selected2))
	{
		if (!vector_is_empty(grid->selected1))
			gfx_draw_vector(grid->elem->active_surface, 0xffff0000, 1, gfx_vector_multiply(grid->selected1, grid->gap));
		return ;
	}
	// check if there is a vertex already with that x and y, if yes then dont copy it just give it the &
	ft_printf("Point Magic.\n");
	temp1 = get_point_from_list(grid->points, &(t_point){0, grid->selected1});
	temp2 = get_point_from_list(grid->points, &(t_point){0, grid->selected2});
	if (temp1 == NULL)
	{
		temp1 = (t_point *)malloc(sizeof(t_point));
		temp1->pos = gfx_new_vector(grid->selected1.x, grid->selected1.y, grid->selected1.z);
		add_to_list(&grid->points, temp1, sizeof(t_point));
	}
	if (temp2 == NULL)
	{
		temp2 = (t_point *)malloc(sizeof(t_point));
		temp2->pos = gfx_new_vector(grid->selected2.x, grid->selected2.y, grid->selected2.z);
		add_to_list(&grid->points, temp2, sizeof(t_point));
	}
	// check if a wall with that same points is in the sector walls, if yes then give it the &
	ft_printf("Wall magic.\n");
	temp_wall = get_wall_from_list(&grid->modify_sector->walls, temp1, temp2); // enable this to not make duplicate walls
	if (temp_wall == NULL)
	{
		// decide if you wanna split or not
		// only do it if the wall youre adding a vector to
		// is a part of the sector youre editing.
		t_list *curr = grid->modify_sector->walls;
		while (curr)
		{
			if (vector_on_wall(temp1->pos, curr->content))
				split_wall(grid, curr->content, temp1);
			if (vector_on_wall(temp2->pos, curr->content))
				split_wall(grid, curr->content, temp2);
			curr = curr->next;
		}
		temp_wall = new_wall(temp1, temp2);
		add_to_list(&grid->walls, temp_wall, sizeof(t_wall));
		add_to_list(&grid->modify_sector->walls, temp_wall, sizeof(t_wall));
	}
	if (grid->modify_sector->first_point == NULL)
		grid->modify_sector->first_point = temp1;
	if (vector_compare(grid->modify_sector->first_point->pos, temp_wall->dest->pos))
	{
		grid->modify_sector->first_point = NULL; // this might have some problems in the future if you dont make it NULL
		grid->modify_sector = NULL;
		grid->selected1 = EMPTY_VEC;
		grid->selected2 = EMPTY_VEC;
		ft_printf("Sector done!\n");
	}
	// It shouldnt have any difference if you make the selected1 the selected2 because it should be what it should before it comes here
	grid->selected1 = grid->selected2;
	grid->selected2 = EMPTY_VEC;
	ft_printf("Wall was added!\n");
}

void	click_calc(t_editor *doom, t_grid *grid)
{
	t_sector	*sector;

	// NOTE: mouse_down doesnt matter which button can be any of left, middle and right.
	if (doom->libui->mouse_down && SDL_GetMouseFocus() == doom->window->win)
	{
		// if the mouse doesnt hover the grid, just gtfo.
		if (!mouse_hover(doom->libui, (t_xywh) {grid->elem->position.x, grid->elem->position.y,
		grid->elem->position.x + grid->elem->position.w, grid->elem->position.y + grid->elem->position.h}))
			return ;

		// FUNNY: if you remove the mouse down last frame it will just draw endlessly :D
		//	"if (mouse_pressed(doom->libui, MKEY_LEFT))"
		if (doom->libui->mouse_down_last_frame && mouse_pressed(doom->libui, MKEY_LEFT))
		{
			if (grid->modify_sector == NULL)
			{
				ft_printf("Making new sector\n");
				sector = new_sector(grid->sector_amount++);
				add_to_list(&grid->sectors, sector, sizeof(t_sector));
				grid->modify_sector = sector;
			}
			if (vector_is_empty(grid->selected1))
				grid->selected1 = grid->hover;
			else if (!vector_compare(grid->selected1, grid->hover))
				grid->selected2 = grid->hover;
		}
		else if (doom->libui->mouse_down_last_frame && mouse_pressed(doom->libui, MKEY_RIGHT))
		{
			ft_putstr("Placed new entity.\n");
			t_entity *entity;

			entity = new_entity(grid->entity_amount++, grid->hover);
			add_to_list(&grid->entities, entity, sizeof(t_entity));
		}
		else if (mouse_pressed(doom->libui, MKEY_MIDDLE))
		{
			ft_putstr("Place spawn point.\n");
			doom->spawn.pos = grid->hover;
		}
	}
}

void	unselect_selected(t_editor *doom, t_grid *grid, SDL_Event *e)
{
	if (key_pressed(doom->libui, KEY_B))
	{
		if (grid->modify_sector == NULL) // you can only reset the selected vertices if sector is null which means you havent
										// started to modify a sector or you have finished modifying it.
		{
			grid->selected1 = EMPTY_VEC;
			grid->selected2 = EMPTY_VEC;
		}
		grid->modify_point = NULL;
		grid->modify_wall = NULL;
		grid->modify_entity = NULL;
		doom->grid.modify_sprite = NULL;
		grid->modify_sector = NULL;
		ft_putstr("Unselected everything.\n");
	}
}

void	hover_calc(t_editor *doom, t_grid *grid)
{
	int x;
	int y;
	int real_x;
	int real_y;
	int gap;

	if (SDL_GetMouseFocus() != doom->window->win)
		return ;
	SDL_GetMouseState(&x, &y);
	gap = grid->gap;
	real_x = (((x - grid->elem->position.x) / (gap / 2)) * (gap / 2)) / gap;
	real_y = (((y - grid->elem->position.y) / (gap / 2)) * (gap / 2)) / gap;
	grid->hover = gfx_new_vector(real_x, real_y, 0);
	gfx_draw_vector(grid->elem->active_surface, 0xffffffff, 1, gfx_vector_multiply(grid->hover, grid->gap));
}

void	draw_grid(t_editor *doom, t_grid *grid)
{
	int x;
	int y;
	int max_y;
	int max_x;

	// reset the grid, becaues we have made the libui ignore this element, so we have to control it ourselves
	fill_surface(grid->elem->active_surface, ((t_bui_window *)grid->elem->parent)->color);	
	//draw_rect(grid->elem->active_surface, 0, 0, grid->elem->position.w, grid->elem->position.h, ((t_bui_window *)grid->elem->parent)->color, 1);
	// this draws the grid on the grid view
		max_y = grid->elem->active_surface->h / grid->gap;
		max_x = grid->elem->active_surface->w / grid->gap;
		y = 0;
		while (y < max_y)
		{	
			gfx_draw_line(grid->elem->active_surface, doom->palette.elem_elem, (t_vector){0, y * grid->gap, 0}, (t_vector){grid->elem->active_surface->w, y * grid->gap, 0});
			y++;
		}
		x = 0;
		while (x < max_x)
		{
			gfx_draw_line(grid->elem->active_surface, doom->palette.elem_elem, (t_vector){x * grid->gap, 0, 0}, (t_vector){x * grid->gap, grid->elem->active_surface->h, 0});
			x++;
		}
//ft_printf("Grid drawn\n");
	// the dimensions
	gfx_draw_line(grid->elem->active_surface, 0xff999966, (t_vector){0, grid->dimensions.y, 0},
												(t_vector){grid->elem->active_surface->w, grid->dimensions.y, 0});
	gfx_draw_line(grid->elem->active_surface, 0xff999966, (t_vector){grid->dimensions.x, 0, 0},
												(t_vector){grid->dimensions.x, grid->elem->active_surface->h, 0});

	gfx_draw_line(grid->elem->active_surface, 0xff999966, (t_vector){0, grid->dimensions.h, 0},
												(t_vector){grid->elem->active_surface->w, grid->dimensions.h, 0});
	gfx_draw_line(grid->elem->active_surface, 0xff999966, (t_vector){grid->dimensions.w, 0, 0},
												(t_vector){grid->dimensions.w, grid->elem->active_surface->h, 0});
// the sector first point
	if (grid->modify_sector != NULL && grid->modify_sector->first_point != NULL)
		gfx_draw_vector(grid->elem->active_surface, 0xffff0000, 2, gfx_vector_multiply(grid->modify_sector->first_point->pos, grid->gap));
}

void	draw_points(t_editor *doom, t_grid *grid, t_list *points)
{
	t_list *curr;

	curr = points;
	while (curr)
	{
		gfx_draw_vector(grid->elem->active_surface, 0xff00ff00, 1, gfx_vector_multiply(((t_point *)curr->content)->pos, grid->gap));
		curr = curr->next;
	}
}

void	draw_walls(t_grid *grid, t_list **walls, Uint32 color)
{
	t_list *curr;

	curr = *walls;
	while (curr)
	{
		gfx_draw_line(grid->elem->active_surface, color, ((t_wall *)curr->content)->orig->pos, ((t_wall *)curr->content)->dest->pos);
		curr = curr->next;
	}
}

void	draw_sectors(t_editor *doom, t_grid *grid)
{
	t_list *curr;
	t_list *curr_wall;
	SDL_Surface *id_text;
	TTF_Font	*font;
	char		*str;

	curr = grid->sectors;
	while (curr)
	{
		//draw_walls(grid, &((t_sector *)curr->content)->walls, ((t_sector *)curr->content)->color);
		curr_wall = ((t_sector *)curr->content)->walls;
		((t_sector *)curr->content)->lowest_pos = gfx_new_vector(INT_MAX, INT_MAX, INT_MAX);
		((t_sector *)curr->content)->highest_pos = gfx_new_vector(INT_MIN, INT_MIN, INT_MIN);
		while (curr_wall)
		{
			t_vector orig_vec = gfx_vector_multiply(((t_wall *)curr_wall->content)->orig->pos, grid->gap);
			t_vector dest_vec = gfx_vector_multiply(((t_wall *)curr_wall->content)->dest->pos, grid->gap);
			if (((t_wall *)curr_wall->content)->neighbor != -1)
				gfx_draw_line(grid->elem->active_surface, 0xffff0000, orig_vec, dest_vec);
			else
				gfx_draw_line(grid->elem->active_surface, ((t_sector *)curr->content)->color, orig_vec, dest_vec);
			if (((t_wall *)curr_wall->content)->dest->pos.x < ((t_sector *)curr->content)->lowest_pos.x)
				((t_sector *)curr->content)->lowest_pos.x = ((t_wall *)curr_wall->content)->dest->pos.x;
			if (((t_wall *)curr_wall->content)->dest->pos.y < ((t_sector *)curr->content)->lowest_pos.y)
				((t_sector *)curr->content)->lowest_pos.y = ((t_wall *)curr_wall->content)->dest->pos.y;
			if (((t_wall *)curr_wall->content)->orig->pos.x < ((t_sector *)curr->content)->lowest_pos.x)
				((t_sector *)curr->content)->lowest_pos.x = ((t_wall *)curr_wall->content)->orig->pos.x;
			if (((t_wall *)curr_wall->content)->orig->pos.y < ((t_sector *)curr->content)->lowest_pos.y)
				((t_sector *)curr->content)->lowest_pos.y = ((t_wall *)curr_wall->content)->orig->pos.y;

			if (((t_wall *)curr_wall->content)->dest->pos.x > ((t_sector *)curr->content)->highest_pos.x)
				((t_sector *)curr->content)->highest_pos.x = ((t_wall *)curr_wall->content)->dest->pos.x;
			if (((t_wall *)curr_wall->content)->dest->pos.y > ((t_sector *)curr->content)->highest_pos.y)
				((t_sector *)curr->content)->highest_pos.y = ((t_wall *)curr_wall->content)->dest->pos.y;
			if (((t_wall *)curr_wall->content)->orig->pos.x > ((t_sector *)curr->content)->highest_pos.x)
				((t_sector *)curr->content)->highest_pos.x = ((t_wall *)curr_wall->content)->orig->pos.x;
			if (((t_wall *)curr_wall->content)->orig->pos.y > ((t_sector *)curr->content)->highest_pos.y)
				((t_sector *)curr->content)->highest_pos.y = ((t_wall *)curr_wall->content)->orig->pos.y;
			curr_wall = curr_wall->next;
		}
		t_vector low;
		t_vector high;

		low = gfx_vector_multiply(((t_sector *)curr->content)->lowest_pos, grid->gap);
		high = gfx_vector_multiply(((t_sector *)curr->content)->highest_pos, grid->gap);
		font = TTF_OpenFont("../libui/TTF/font.ttf", 20);
		str = ft_itoa(((t_sector *)curr->content)->id);
		id_text = TTF_RenderText_Blended(font, str, (SDL_Color){255, 255, 255, 255});
		SDL_BlitSurface(id_text, NULL, grid->elem->active_surface, &(SDL_Rect){low.x + ((high.x - low.x) / 2), low.y + ((high.y - low.y) / 2), id_text->w, id_text->h});
		SDL_FreeSurface(id_text);
		ft_strdel(&str);
		TTF_CloseFont(font);
		curr = curr->next;
	}
}

void	draw_entities(t_editor *doom, t_grid *grid)
{
	t_list		*curr;
	t_entity	*entity;
	t_vector	pos;

	curr = grid->entities;
	while (curr)
	{
		entity = curr->content;
		if (entity->preset == NULL)
			entity->preset = doom->default_entity;
		pos = gfx_vector_multiply(entity->pos, grid->gap);
		gfx_draw_vector(grid->elem->active_surface, 0xffaaab5d, 6, pos);
		if (entity->preset->mood == ENTITY_TYPE_HOSTILE)
			gfx_draw_vector(grid->elem->active_surface, 0xffff0000, 3, pos);
		else if (entity->preset->mood == ENTITY_TYPE_FRIENDLY)
			gfx_draw_vector(grid->elem->active_surface, 0xff00ff00, 3, pos);
		else if (entity->preset->mood == ENTITY_TYPE_NEUTRAL)
			gfx_draw_vector(grid->elem->active_surface, 0xff0000ff, 3, pos);

		float angle = entity->direction * (M_PI / 180);
		float dx = cos(angle) * 10.0f;
		float dy = sin(angle) * 10.0f;
		t_vector dir_pos = gfx_new_vector(dx + pos.x, dy + pos.y, 0);
		gfx_draw_vector(grid->elem->active_surface, 0xffaaab5d, 1, dir_pos);

		curr = curr->next;
	}
}

void	draw_hover_info(t_editor *doom, t_grid *grid)
{
	char		*str;
	
	str = ft_sprintf("%d, %d\nzoom: %d\n", (int)grid->hover.x, (int)grid->hover.y, (int)grid->gap);
	bui_set_element_text(doom->hover_info, str, 0, 0, 0xffffffff);
	ft_strdel(&str);
}

void	draw_selected_sector_info(t_editor *doom, t_grid *grid)
{
	char		*str;

	if (grid->modify_sector == NULL)
		return ;
	str = ft_sprintf("Sector:\nid: %d\nwalls: %d\n", grid->modify_sector->id, get_sector_wall_amount(grid->modify_sector));
	bui_set_element_text(doom->selected_sector_info, str, 0, 0, 0xffffffff);
	ft_strdel(&str);
}
