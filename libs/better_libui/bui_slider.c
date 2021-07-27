/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_slider.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:58:55 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 11:58:56 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

int	bui_get_slider_value(int min, int max, int pos_x, int w)
{
	float	pixels_per_value;
	int		total_values;
	int		nth_value;
	int		curr_value;

	total_values = abs(max - min);
	pixels_per_value = (float)total_values / (float)w;
	nth_value = pixels_per_value * (float)pos_x;
	curr_value = min + nth_value;
	return (ft_clamp(curr_value, min, max));
}

int	bui_set_slider_value(int value, int min, int max, int w)
{
	return (ft_clamp(((float)w / (float)(abs(max - min)))
		* (float)(abs(min - value)), 0, w));
}
