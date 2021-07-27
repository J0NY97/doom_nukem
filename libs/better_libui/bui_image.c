#include "better_libui.h"

void	bui_set_element_image_from_path(
		t_bui_element *element, int type, char *path, t_xywh *pos)
{
	SDL_Surface	*image;

	image = load_image(path);
	if (image)
	{
		bui_set_element_image(element, type, image, pos);
		SDL_FreeSurface(image);
	}
}

void	bui_set_element_image(
		t_bui_element *element, int type, SDL_Surface *image, t_xywh *pos)
{
	SDL_Rect	*rect;

	rect = NULL;
	if (!image)
	{
		ft_putstr("[bui_set_element_image] SDL_Surface *image doesnt exist.\n");
		return ;
	}
	if (pos)
		rect = &(SDL_Rect){pos->x, pos->y, pos->w, pos->h};
	if (type == ELEMENT_ALL)
	{
		SDL_BlitScaled(image, NULL, element->surface[0], rect);
		SDL_BlitScaled(image, NULL, element->surface[1], rect);
		SDL_BlitScaled(image, NULL, element->surface[2], rect);
	}
	else
		SDL_BlitScaled(image, NULL, element->surface[type], rect);
}

void	bui_set_element_image_all_states_image(
	t_bui_element *element, SDL_Surface *zeroth,
	SDL_Surface *first, SDL_Surface *second)
{
	bui_set_element_image(element, ELEMENT_DEFAULT, zeroth, NULL);
	bui_set_element_image(element, ELEMENT_HOVER, first, NULL);
	bui_set_element_image(element, ELEMENT_CLICK, second, NULL);
}

//void	bui_set_element_image_all_states_from_path
void	bui_set_element_image_to_states(
		t_bui_element *element, char *zeroth, char *first, char *second)
{
	bui_set_element_image_from_path(element, ELEMENT_DEFAULT, zeroth, NULL);
	bui_set_element_image_from_path(element, ELEMENT_HOVER, first, NULL);
	bui_set_element_image_from_path(element, ELEMENT_CLICK, second, NULL);
}
