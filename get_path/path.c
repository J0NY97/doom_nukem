/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 12:03:34 by nneronin          #+#    #+#             */
/*   Updated: 2021/07/28 16:04:03 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "sys/stat.h"
#include "libft.h"
#include "libpf.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int	main(void)
{
	int		fd;
	char	*line;
	char	*path1;

	fd = creat("./path.h", S_IRUSR | S_IWUSR);
	path1 = SDL_GetBasePath();
	ft_dprintf(fd, "\
#ifndef PATH_H\n\
# define PATH_H\n\
# define ROOT_PATH \"%s\"\n\
# define GAME_PATH \"%sgame/\"\n\
# define WAV_PATH \"%sgame/resources/WAV/\"\n\
# define BXPM_PATH \"%sgame/resources/BXPM/\"\n\
# define ICON_PATH \"%sgame/resources/ICON/\"\n\
# define TTF_PATH \"%sgame/resources/TTF/\"\n\
# define MAP_PATH \"%sgame/resources/MAPS/\"\n\
#endif"\
	, path1, path1, path1, path1, path1, path1, path1);
	close(fd);
	free(path1);
	return (0);
}
