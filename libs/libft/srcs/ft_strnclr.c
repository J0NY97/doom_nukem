#include "libft.h"

void	ft_strnclr(char *str, size_t n)
{
	if (!str)
		return ;
	while (--n)
		str[n] = '\0';
}
