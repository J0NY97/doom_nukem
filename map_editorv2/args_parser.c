/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 12:37:03 by jsalmi            #+#    #+#             */
/*   Updated: 2020/09/26 15:27:27 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

// !!!!!!!!!!!!!!!!!!!IMPORTANT!!!!!!!!!!!!!!!!!!
// TODO: remember to norminette ft_strendswith(); in libft
int	args_parser(char **filename, int ac, char **av)
{
	ft_putstr("Parsing args\n");
	if (ac < 2)
	{
		ft_putstr("Usage: ./doom_editor [map_name]\n");
		return (0);
	}
	else if (ac > 1)
	{
		if (ft_strendswith(av[1], ".doom") == 0)
			*filename = ft_strjoiner("./maps/", av[1], NULL);
		else
			*filename = ft_strjoiner("./maps/", av[1], ".doom", NULL);
		ft_putstr("Map name : ");
		ft_putstr(*filename);
		ft_putchar('\n');
		return (1);
	}
	return (0);
}
