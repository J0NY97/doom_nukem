/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_entity_presets.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 13:19:46 by jsalmi            #+#    #+#             */
/*   Updated: 2021/08/04 14:04:08 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

SDL_Surface	*yoink_from_surface(SDL_Surface *image, t_xywh coord)
{
	SDL_Surface	*surface;

	surface = create_surface(coord.w, coord.h);
	SDL_BlitSurface(image, &(SDL_Rect){coord.x, coord.y, coord.w, coord.h},
		surface, NULL);
	return (surface);
}

void	init_entity_presets(t_list **list)
{
	int				i;
	int const		*tc;
	SDL_Surface		*image;
	t_entity_preset	*preset;

	i = 0;
	while (i < ENTITY_AMOUNT)
	{
		preset = new_entity_preset(g_entity_data[i].name);
		preset->mood = ENTITY_TYPE_HOSTILE;
		if (!g_entity_data[i].type)
			preset->mood = ENTITY_TYPE_NEUTRAL;
		image = load_bxpm_to_surface(g_entity_data[i].path);
		tc = g_entity_data[i].tc;
		preset->texture = yoink_from_surface(image,
				(t_xywh){tc[0], tc[1], tc[2], tc[3]});
		add_to_list(list, preset, sizeof(t_entity_preset));
		SDL_FreeSurface(image);
		i++;
	}
}
