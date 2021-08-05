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

char	*remove_from_input(t_bui_element *input)
{
	int		len;
	char	*new_str;

	len = 0;
	new_str = NULL;
	if (input->text)
		len = ft_strlen(input->text);
	if (len > 0)
		new_str = ft_strndup(input->text, len - 1);
	return (new_str);
}

char	*add_to_input(t_bui_element *input)
{
	t_bui_libui	*libui;
	char		*new_str;

	libui = input->libui;
	if (input->text)
		new_str = ft_strdup(input->text);
	else
		new_str = ft_strnew(0);
	if (key_pressed(libui, SDL_SCANCODE_SPACE))
		ft_straddchar(&new_str, ' ');
	else
	{
		if (alpha_pressed(libui) || number_pressed(libui)
			|| special_char_pressed(libui))
		{
			if (!key_pressed(libui, SDL_SCANCODE_LSHIFT))
				ft_straddchar(&new_str, ft_tolower(libui->last_key[0]));
			else
				ft_stradd(&new_str, libui->last_key);
		}
	}
	return (new_str);
}

int	bui_input(t_bui_element *input)
{
	t_bui_libui	*libui;
	char		*new_str;
	int			len;

	libui = input->libui;
	if (bui_button_toggle(input))
	{
		if (libui->last_key != NULL)
		{
			if (key_pressed(libui, SDL_SCANCODE_RETURN))
			{
				input->toggle = 0;
				return (1);
			}
			else if (key_pressed(libui, SDL_SCANCODE_BACKSPACE))
				new_str = remove_from_input(input);
			else
				new_str = add_to_input(input);
			bui_set_element_text(input, new_str);
			ft_strdel(&new_str);
		}
	}
	return (0);
}
