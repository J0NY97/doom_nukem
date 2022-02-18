/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_file_content.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 15:39:53 by nneronin          #+#    #+#             */
/*   Updated: 2022/02/18 02:29:53 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stdio.h"
#include "sys/stat.h"

char	*get_file_content(const char *file)
{
	char	*final;
	FILE	*fp;
	int		fd;
	struct stat	file_stat;
	int		file_len;

	if (!file)
		return (NULL);
	fp = fopen(file, "r");
	if (!fp)
		return (NULL);
	fd = fileno(fp);
	fstat(fd, &file_stat);
	file_len = file_stat.st_size;
	final = malloc(file_len + 1);
	fread(final, file_len, 1, fp);
	final[file_len] = 0;
	fclose(fp);
	return (final);
}