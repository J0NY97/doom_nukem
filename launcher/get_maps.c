#include "launcher.h"
#include <dirent.h>

void	*return_error(char *message, void *return_value)
{
	ft_putstr(message);
	return (return_value);
}

t_list	*get_maps(char *directory)
{
	DIR				*dirp;
	struct dirent	*dp;
	char			*file;
	t_list			*maps;

	maps = NULL;
	dirp = opendir(directory);
	if (!dirp)
		return (return_error("[get_maps] Couldnt open file.", NULL));
	dp = readdir(dirp);
	while (dp)
	{
		if ((ft_strendswith(dp->d_name, ".endless") == 0)
			|| (ft_strendswith(dp->d_name, ".story") == 0))
		{
			file = ft_strdup(dp->d_name);
			ft_printf("Found map named: %s", file);
			add_to_list(&maps, file, sizeof(char *));
			ft_printf(" == %s\n", maps->content);
		}
		dp = readdir(dirp);
	}
	closedir(dirp);
	ft_printf("Map directory: %s read.\n", directory);
	return (maps);
}
