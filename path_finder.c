/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 12:03:34 by nneronin          #+#    #+#             */
/*   Updated: 2021/05/06 12:07:58 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdlib.h"
#include ""

int 	main(void)
{
	int fd;
	char *line;
	int i;

	i = -1;
	fd = open("path.h", O_RDWR);
	while (++i < 12)
	{
		line = get_next_line(fd, &line);
		free(&line);
	}

}
