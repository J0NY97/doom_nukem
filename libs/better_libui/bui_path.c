/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 12:03:34 by nneronin          #+#    #+#             */
/*   Updated: 2021/07/26 12:43:13 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdlib.h"
#include "libft.h"
#include "libpf.h"
#include "fcntl.h"
#define SDL_MAIN_HANDLED
#include "SDL.h"

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

int	main(void)
{
	int		fd;
	char	*line;
	char	*path;
	char	*non_rword_path;

	fd = creat("bui_path.h", S_IRUSR | S_IWUSR);
	path = SDL_GetBasePath();
	build_non_rword_path(path);
	ft_dprintf(fd,
		"#ifndef BUI_PATH_H\n"\
		"# define BUI_PATH_H\n"\
		"# define BUI_PATH \"%s\"\n"\
		"#endif"\
		, path);
	close(fd);
	SDL_free(path);
	return (1);
}
