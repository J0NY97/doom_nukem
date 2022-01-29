/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_file_content.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 15:39:53 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/08 15:40:24 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stdio.h"
#include "sys/stat.h"

char	*get_file_content(char *file)
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
	fread(final, 1, file_len, fp);
	final[file_len] = '\0';
	fclose(fp);
	return (final);
}