/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_text.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:59:02 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 12:22:11 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

SDL_Surface	*bui_make_text_surface_from_recipe(t_text_recipe *recipe)
{
	SDL_Surface	*surface;
	TTF_Font	*font;
	t_rgba		rgb;

	if (!recipe->text)
		ft_printf("[bui_make_text_surface_from_recipe] no recipe->text");
	font = TTF_OpenFont(recipe->font_name, recipe->font_size);
	if (!font)
	{
		ft_printf("[bui_make_text_surface_from_recipe] error : %s\n",
			TTF_GetError());
		return (NULL);
	}
	TTF_SetFontStyle(font, recipe->font_style);
	rgb = hex_to_rgba(recipe->text_color);
	surface = TTF_RenderUTF8_Blended_Wrapped(font, recipe->text,
			(SDL_Color){.a = rgb.a, .r = rgb.r, .g = rgb.g, .b = rgb.b},
			recipe->max_w);
	TTF_CloseFont(font);
	if (!surface)
	{
		ft_printf("[bui_make_text_surface_from_recipe] no surface made for "\
			"text: %s,TTF?ERROR : %s!", recipe->text, TTF_GetError());
	}
	return (surface);
}

void	bui_set_element_text(t_bui_element *elem, char *text)
{
	char	*temp_font_name;

	if (elem->text)
		ft_strdel(&elem->text);
	if (elem->text_surface)
		SDL_FreeSurface(elem->text_surface);
	elem->text = NULL;
	elem->text_surface = NULL;
	if (!text)
		return ;
	elem->text = ft_strdup(text);
	temp_font_name = ft_strdup(elem->font_name);
	bui_set_element_text_font(elem, temp_font_name,
		elem->font_size, elem->text_color);
	ft_strdel(&temp_font_name);
}

void	bui_remove_element_text(t_bui_element *elem)
{
	if (elem->text)
		ft_strdel(&elem->text);
	if (elem->text_surface)
		SDL_FreeSurface(elem->text_surface);
	elem->text = NULL;
	elem->text_surface = NULL;
}

void	bui_set_element_text_font(
		t_bui_element *elem, char *font_name, Uint32 size, Uint32 color)
{
	t_text_recipe	recipe;
	SDL_Surface		*text_surface;

	if (!elem->text || !font_name)
		return ;
	text_surface = bui_make_text_surface_from_recipe(&(t_text_recipe){
			.text = elem->text, .font_name = font_name,
			.font_style = elem->font_style, .max_w = elem->position.w
			- elem->text_x, .text_color = color, .font_size = size});
	if (text_surface)
	{
		if (elem->text_surface)
			SDL_FreeSurface(elem->text_surface);
		if (elem->font)
			TTF_CloseFont(elem->font);
		ft_strdel(&elem->font_name);
		elem->font_name = ft_strdup(font_name);
		elem->font = TTF_OpenFont(font_name, size);
		elem->text_surface = text_surface;
		elem->text_color = color;
		elem->font_size = size;
		elem->text_w = elem->text_surface->w;
		elem->text_h = elem->text_surface->h;
	}
	else
		ft_putstr("[bui_set_element_text_font] Couldnt make text surface.\n");
}
