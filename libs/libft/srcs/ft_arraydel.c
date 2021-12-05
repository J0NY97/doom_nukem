#include "libft.h"

void	ft_arraydel(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
		ft_strdel(&arr[i]);
	free(arr);
	arr = NULL;
}
