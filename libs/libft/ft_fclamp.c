
#include "libft.h"

float	ft_fclamp(float value, float min, float max)
{
	if (value < min)
		return (min);
	else if (value > max)
		return (max);
	return (value);
}
