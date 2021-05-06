/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 12:03:34 by nneronin          #+#    #+#             */
/*   Updated: 2021/05/06 12:52:32 by jsalmi           ###   ########.fr       */
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

	i = -1;
	fd = creat("path.h", S_IRUSR | S_IWUSR);
	temp = realpath("./", NULL);
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
# define ROOT_PATH \"%s/\"\n#endif", temp);
	close(fd);
	free(temp);
	return (0);
}
