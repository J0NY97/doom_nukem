#include "editor.h"

t_entity	*new_entity(int id, t_vector pos)
{
	t_entity	*entity;

	entity = ft_memalloc(sizeof(t_entity));
	entity->id = id;
	entity->pos = pos;
	entity->direction = 90;
	entity->preset = NULL;
	return (entity);
}

void	free_entity(void *content, size_t size)
{
	t_entity	*entity;

	(void)size;
	entity = content;
	if (entity == NULL)
		return ;
	ft_memdel((void **)&entity);
}

t_entity	*get_entity_from_list_at_pos(t_list *list, t_vector pos)
{
	t_list	*curr;

	curr = list;
	while (curr)
	{
		if (vector_compare(pos, ((t_entity *)curr->content)->pos))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}
void	draw_entity(t_editor *editor, t_entity *entity)
{
	t_vector	pos;
	int			color;
	float		angle;

	if (entity->preset == NULL)
		entity->preset = editor->default_entity;
	pos = gfx_vector_multiply(entity->pos, editor->grid.gap);
	gfx_draw_vector(editor->grid.elem->active_surface, 0xffaaab5d, 6, pos);
	color = 0xff0000ff;
	if (entity->preset->mood == ENTITY_TYPE_HOSTILE)
		color = 0xffff0000;
	else if (entity->preset->mood == ENTITY_TYPE_FRIENDLY)
		color = 0xff00ff00;
	gfx_draw_vector(editor->grid.elem->active_surface, color, 3, pos);
	angle = entity->direction * (M_PI / 180);
	gfx_draw_vector(editor->grid.elem->active_surface, 0xffaaab5d, 1,
		gfx_new_vector(cos(angle) * 10.0f + pos.x,
			sin(angle) * 10.0f + pos.y, 0));
}

void	draw_entities(t_editor *editor)
{
	t_list		*curr;

	curr = editor->grid.entities;
	while (curr)
	{
		draw_entity(editor, curr->content);
		curr = curr->next;
	}
}
