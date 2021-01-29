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
/*
 ** Takes in ac and av and saves it in char *filename
 **		ac > 1
 **			read the 2nd arg and try to open that map if doesnt exist make a new one with that name
*/
int		args_parser(char **filename, int ac, char **av)
{
	ft_putstr("Parsing args\n");
	if (ac < 2)
	{
		ft_putstr("give map name as 2nd arg\n");
		return (0);
	}
	else if (ac > 1)
	{
		*filename = ft_strjoiner("./maps/", av[1], ".doom", NULL);
		ft_putstr("Map name : ");
		ft_putstr(*filename);
		ft_putchar('\n');
		return (1);
	}
	return (0);
}
