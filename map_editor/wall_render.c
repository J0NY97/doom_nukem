/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:44:52 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:44:53 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	render_sprites_on_the_wall(
	t_editor *editor, SDL_Surface *scaled_wall, int scale)
{
	t_list		*curr;
	t_sprite	*sprite;
	SDL_Surface	*temp_sprite;

	curr = editor->grid.modify_wall->sprites;
	while (curr)
	{
		sprite = curr->content;
		temp_sprite
			= editor->texture_textures[sprite->sprite_id];
		sprite->coord = new_xywh(
				scale * sprite->real_x, scale * sprite->real_y,
				scale * sprite->scale, scale * sprite->scale);
		SDL_BlitScaled(temp_sprite, NULL, scaled_wall, &(SDL_Rect){
			sprite->coord.x, sprite->coord.y,
			sprite->coord.w, sprite->coord.h});
		curr = curr->next;
	}
	if (editor->grid.modify_sprite != NULL)
		draw_rect_border(scaled_wall,
			(t_xywh){editor->grid.modify_sprite->coord.x,
			editor->grid.modify_sprite->coord.y,
			editor->grid.modify_sprite->coord.w,
			editor->grid.modify_sprite->coord.h}, 0xff0000ff, 2);
}

void	render_scaled_wall(t_editor *editor, SDL_Surface *scaled_wall)
{
	SDL_Surface	*ent_tex;
	SDL_Surface	*dst_surf;
	SDL_Rect	rect;
	float		aspect;

	ent_tex = scaled_wall;
	dst_surf = editor->edit_view_wall->active_surface;
	aspect = ft_fmin((float)dst_surf->w / (float)ent_tex->w,
			(float)dst_surf->h / (float)ent_tex->h);
	rect.w = ent_tex->w * aspect;
	rect.h = ent_tex->h * aspect;
	rect.x = (dst_surf->w - rect.w) / 2;
	rect.y = (dst_surf->h - rect.h) / 2;
	SDL_BlitScaled(scaled_wall, NULL,
		editor->edit_view_wall->active_surface, &rect);
	gfx_draw_rect(editor->edit_view_wall->active_surface, 0xff00ff00,
		(t_xywh){rect.x, rect.y, rect.w, rect.h});
	SDL_FreeSurface(scaled_wall);
}

void	render_texture_on_the_wall(
		SDL_Surface *surface, SDL_Surface *texture,
		float texture_scale, float scale)
{
	int			y;
	int			x;
	int			x_axis;
	int			y_axis;
	SDL_Rect	temp;

	x_axis = (((float)surface->w / scale) / texture_scale);
	y_axis = (((float)surface->h / scale) / texture_scale);
	y = 0;
	while (y < y_axis)
	{
		x = 0;
		while (x < x_axis)
		{
			temp.w = scale * texture_scale;
			temp.h = scale * texture_scale;
			temp.x = x * temp.w;
			temp.y = y * temp.h;
			SDL_BlitScaled(texture, NULL, surface, &temp);
			x++;
		}
		y++;
	}
}

void	wall_render(t_editor *editor)
{
	float		scale;
	t_xywh		dim;
	t_wall		*wall;
	t_sector	*sec;
	SDL_Surface	*scaled_wall;

	SDL_FillRect(editor->edit_view_wall->active_surface, NULL, 0xff000000);
	wall = editor->grid.modify_wall;
	dim.w = gfx_distance(wall->orig->pos, wall->dest->pos) * editor->scale;
	sec = get_sector_with_wall(&editor->grid.sectors, wall);
	dim.h = sec->ceiling_height - sec->floor_height;
	scale = editor->edit_view_wall->position.w / dim.w;
	scaled_wall = create_surface(dim.w * scale, dim.h * scale);
	render_texture_on_the_wall(scaled_wall, editor->texture_textures[
		ft_clamp(wall->texture_id, 0, editor->texture_amount)],
		editor->grid.modify_wall->texture_scale, scale);
	render_sprites_on_the_wall(editor, scaled_wall, scale);
	render_scaled_wall(editor, scaled_wall);
}
