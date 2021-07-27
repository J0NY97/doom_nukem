#include "editor.h"

t_entity_preset	*new_entity_preset(char *name)
{
	t_entity_preset	*preset;

	preset = ft_memalloc(sizeof(t_entity_preset));
	preset->name = ft_strdup(name);
	preset->mood = ENTITY_TYPE_NEUTRAL;
	preset->texture = NULL;
	return (preset);
}

void	free_entity_preset(void *content, size_t size)
{
	t_entity_preset	*ent;

	(void)size;
	ent = content;
	if (ent == NULL)
		return ;
	ft_strdel(&ent->name);
	if (ent->texture)
		SDL_FreeSurface(ent->texture);
	ft_memdel((void **)&ent);
}
// TODO: Get from list if function.
t_entity_preset	*get_entity_preset_with_name(t_list *list, char *name)
{
	t_list			*curr;
	t_entity_preset	*preset;

	curr = list;
	while (curr)
	{
		preset = curr->content;
		if (ft_strcmp(preset->name, name) == 0)
			return (preset);
		curr = curr->next;
	}
	return (NULL);
}
