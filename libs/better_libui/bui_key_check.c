/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 14:05:02 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:03:55 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

int	alpha_pressed(t_bui_libui *libui)
{
	int	i;

	i = SDL_SCANCODE_A;
	while (i <= SDL_SCANCODE_Z)
	{
		if (libui->keys[i])
			return (1);
		i++;
	}
	return (0);
}

int	number_pressed(t_bui_libui *libui)
{
	int	i;

	i = SDL_SCANCODE_1;
	while (i <= SDL_SCANCODE_0)
	{
		if (libui->keys[i])
			return (1);
		i++;
	}
	return (0);
}

int	special_char_pressed(t_bui_libui *libui)
{
	int	i;

	i = SDL_SCANCODE_MINUS;
	while (i <= SDL_SCANCODE_SLASH)
	{
		if (libui->keys[i])
			return (1);
		i++;
	}
	return (0);
}
