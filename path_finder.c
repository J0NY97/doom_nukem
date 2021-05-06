/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 12:03:34 by nneronin          #+#    #+#             */
/*   Updated: 2021/05/06 13:08:11 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdlib.h"
#include "libft.h"
#include "ft_printf.h"

int 	main(void)
{
	int		i;
	int		fd;
	char	*line;
	char	*temp;
	char	*game_path;

	i = -1;
	fd = creat("path.h", S_IRUSR | S_IWUSR);
	temp = realpath("./", NULL);
	game_path = ft_strjoin(temp, "/game");
	ft_fprintf(fd, "/* ************************************************************************** */\n\
/*                                                                            */\n\
/*                                                        :::      ::::::::   */\n\
/*   path.h                                             :+:      :+:    :+:   */\n\
/*                                                    +:+ +:+         +:+     */\n\
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */\n\
/*                                                +#+#+#+#+#+   +#+           */\n\
/*   Created: 2021/05/06 12:13:01 by jsalmi            #+#    #+#             */\n\
/*   Updated: 2021/05/06 12:13:07 by jsalmi           ###   ########.fr       */\n\
/*                                                                            */\n\
/* ************************************************************************** */\n\
\n\
#ifndef PATH_H\n\
# define PATH_H\n\
# define GAME_PATH \"%s/\"\n\
# define ROOT_PATH \"%s/\"\n#endif", game_path, temp);
	close(fd);
	free(temp);
	free(game_path);
	return (0);
}
