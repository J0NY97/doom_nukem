/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:57:49 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 11:57:50 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

SDL_Surface	*load_image(char *file)
{
	SDL_Surface	*img;

	img = IMG_Load(file);
	if (!file || !img)
	{
		if (file)
			ft_printf("image_load: %s\n", IMG_GetError());
		return (NULL);
	}
	return (img);
}
