#include "better_libui.h"

SDL_Surface	*bui_new_text(
		char *text, TTF_Font *font, Uint32 font_color, int max_w)
{
	SDL_Surface	*text_surface;
	t_rgba		rgb;

	if (!text)
	{
		ft_printf("[bui_new_text] char *text == NULL\n");
		return (NULL);
	}
	if (!font)
	{
		ft_printf("[bui_new_text]: TTF_Font *font == NULL\n");
		return (NULL);
	}
	if (ft_strlen(text) == 0)
		ft_printf("[bui_new_text] char *text len == 0\n");
	rgb = hex_to_rgba(font_color);
	text_surface = TTF_RenderUTF8_Blended_Wrapped(font, text,
			(SDL_Color){.a = rgb.a, .r = rgb.r, .g = rgb.g, .b = rgb.b}, max_w);
	if (!text_surface)
		ft_printf("[bui_new_text]: text_surface ; %s\n", TTF_GetError());
	return (text_surface);
}

// This function is in prototype mode.
SDL_Surface	*bui_make_text_surface_from_recipe(t_text_recipe *recipe)
{
	SDL_Surface	*surface;
	TTF_Font	*font;
	t_rgba		rgb;

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
	return (surface);
}

// Dont use this before its tested.
void	bui_element_update_text(t_bui_element *elem)
{
	TTF_Font	*font;	
	SDL_Surface	*surface;

	font = TTF_OpenFont(elem->font_name, elem->font_size);
	if (!font)
	{
		ft_printf("[bui_element_update_text] TTF_ERROR:  %s", TTF_GetError());
		return ;
	}
	if (elem->font)
		TTF_CloseFont(elem->font);
	elem->font = font;
	surface = bui_new_text(elem->text, elem->font,
			elem->text_color, elem->position.w);
	if (!surface)
	{
		ft_printf("[bui_element_update_text] TTF_ERROR:  %s", TTF_GetError());
		return ;
	}
	if (elem->text_surface)
		SDL_FreeSurface(elem->text_surface);
	elem->text_surface = surface;
}
