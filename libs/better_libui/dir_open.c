/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir_open.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 11:59:46 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 11:59:47 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

t_list	*dir_open(char *folder_path, unsigned char type, int *size)
{
	char			*temp;
	DIR				*dir;
	t_list			*list;
	struct dirent	*file;

	*size = 0;
	list = NULL;
	dir = opendir(folder_path);
	if (!dir)
		return (NULL);
	file = readdir(dir);
	while (file)
	{
		if (file->d_type == type && file->d_name[0] != '.')
		{
			temp = ft_strdup(file->d_name);
			add_to_list(&list, temp, sizeof(char *));
			*size += 1;
		}
		file = readdir(dir);
	}
	closedir(dir);
	return (list);
}
