/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 12:03:34 by nneronin          #+#    #+#             */
/*   Updated: 2021/07/19 19:04:37 by jsalmi           ###   ########.fr       */
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
# define GAME_PATH ROOT_PATH\"game/\"\n\
# define RESOURCE_PATH GAME_PATH\"resources/\"\n\
# define TTF_PATH RESOURCE_PATH\"TTF/\"\n\
# define UI_PATH RESOURCE_PATH\"UI/\"\n\
# define ICON_PATH RESOURCE_PATH\"ICON/\"\n\
#endif"\
	, path);
	close(fd);
	free(path);
	return (1);
}
