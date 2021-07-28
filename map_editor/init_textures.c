#include "editor.h"

static void	copy_bxpm_pix_to_surf_pix(t_bxpm *bxpm, SDL_Surface *surface)
{
	int	i;

	i = 0;
	while (i < bxpm->pix_nb)
	{
		((Uint32 *)surface->pixels)[i] = bxpm->clr[bxpm->pix[i]];
		i++;
	}
}

SDL_Surface	*load_bxpm_to_surface(char *bxpm_file)
{
	SDL_Surface	*surface;
	t_bxpm		*bxpm;
	int			result;

	bxpm = ft_memalloc(sizeof(t_bxpm));
	result = read_bxpm(bxpm, bxpm_file);
	if (result != 1)
	{
		ft_printf("[load_bxpm_to_surface] Couldnt open file: %s\n", bxpm_file);
		free(bxpm);
		return (NULL);
	}
	surface = create_surface(bxpm->w, bxpm->h);
	copy_bxpm_pix_to_surf_pix(bxpm, surface);
	free(bxpm->pix);
	free(bxpm->clr);
	free(bxpm);
	return (surface);
}

void	load_all_textures(t_editor *editor)
{
	int	i;

	i = 0;
	editor->texture_amount = MAP_TEXTURE_AMOUNT;
	editor->texture_textures = ft_memalloc(sizeof(SDL_Surface *)
			* editor->texture_amount);
	while (i < MAP_TEXTURE_AMOUNT)
	{
		editor->texture_textures[g_map_textures[i].id]
			= load_bxpm_to_surface(g_map_textures[i].path);
		i++;
	}
}
