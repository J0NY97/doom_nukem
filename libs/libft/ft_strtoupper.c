#include "libft.h"

char	*ft_strtoupper(const char *src)
{
	char	*str;
	int		len;
	int		i;

	if (!src)
		return (NULL);
	i = 0;
	len = ft_strlen(src);
	str = ft_strnew(len);
	while (i < len)
	{
		str[i] = ft_toupper(src[i]);
		i++;
	}
	return (str);
}
