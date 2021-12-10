#include "libft.h"

void	ft_putarr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = -1;
	while (arr[++i])
	{
		ft_putstr(arr[i]);
		ft_putchar('\n');
	}
}
