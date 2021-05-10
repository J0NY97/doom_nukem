/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 12:03:34 by nneronin          #+#    #+#             */
/*   Updated: 2021/05/08 12:11:49 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdlib.h"
#include "libft.h"
#include "ft_printf.h"

int 	main(void)
{
	int		fd;
	char	*line;
	char	*path;

	fd = creat("path.h", S_IRUSR | S_IWUSR);
	path = realpath("./", NULL);
	ft_fprintf(fd, "\
#ifndef PATH_H\n\
# define PATH_H\n\
# define ROOT_PATH \"%s/\"\n\
# define GAME_PATH \"%s/game/\"\n\
#endif"\
	, path, path);
	close(fd);
	free(path);
	return (1);
}
