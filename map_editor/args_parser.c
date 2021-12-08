#include "editor.h"

/*
 * Returns: 1 if we have given a map_name,
 * 	so it knows if its worth even searching for;
*/
int	args_parser(t_editor *editor, int ac, char **av)
{
	char	**temp;
	int		arr_len;

	if (ac < 1)
		return (0);
	if (av[1])
	{
		if (!ft_strendswith(av[1], ".dnd")
			|| !ft_strendswith(av[1], ".dnds")
			|| !ft_strendswith(av[1], ".dnde"))
		{
			temp = ft_strsplit(av[1], '/');
			arr_len = ft_arrlen(temp);
			ft_strremove(temp[arr_len - 1], ".dnde");
			ft_strremove(temp[arr_len - 1], ".dnds");
			ft_strremove(temp[arr_len - 1], ".dnd");
			if (editor->map_name)
				ft_strdel(&editor->map_name);
			editor->map_name = ft_strdup(temp[arr_len - 1]);
			editor->map_full_path = ft_strdup(av[1]);
			ft_arraydel(temp);
			return (1);
		}
	}
	return (0);
}
