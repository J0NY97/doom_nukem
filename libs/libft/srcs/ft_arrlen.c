#include "libft.h"

int	ft_arrlen(char **arr)
{
	int	i;

	if (!arr)
		return (0);
	i = -1;
	while (arr[++i]);
	return (i);
}
