/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 12:03:34 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/10 20:15:00 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SDL.h"
#include "sys/stat.h"
#include "libft.h"
#include "libpf.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define SDL_MAIN_HANDLED

char	*build_non_rword_path(char *rword_path)
{
	int		i;

	i = 0;
	while (rword_path[i])
	{
		if (rword_path[i] == '\\')
			rword_path[i] = '/';
		i++;
	}
	return (rword_path);
}

//# define MAP_PATH \"%sgame/resources/MAPS/\"\n
int	main(void)
{
	int		fd;
	char	*line;
	char	*path;

	fd = creat("path.h", S_IRUSR | S_IWUSR);
	path = SDL_GetBasePath();
	build_non_rword_path(path);
	ft_dprintf(fd, "\
#ifndef PATH_H\n\
# define PATH_H\n\
# define ROOT_PATH \"%s\"\n\
# define WAV_PATH \"%sgame/resources/WAV/\"\n\
# define BXPM_PATH \"%sgame/resources/BXPM/\"\n\
# define ICON_PATH \"%sgame/resources/ICON/\"\n\
# define TTF_PATH \"%sgame/resources/TTF/\"\n\
# define MAP_PATH \"%sgame/resources/MAPS/\"\n\
# define BMP_PATH \"%sgame/resources/GAME/\"\n\
# define GAME_PATH \"%sgame/\"\n\
# define EDITOR_PATH \"%smap_editor/\"\n\
# define LAUNCHER_PATH \"%slauncher/\"\n\
#endif"\
	, path, path, path, path, path, path, path, path, path, path);
	close(fd);
	SDL_free(path);
	return (1);
}
