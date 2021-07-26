#include "better_libui.h"

SDL_Surface	*load_image(char *file)
{
	SDL_Surface	*img;

	img = IMG_Load(file);
	if (!file || !img)
	{
		if (file)
			ft_printf("image_load: %s\n", IMG_GetError());
		return (NULL);
	}
	return (img);
}
