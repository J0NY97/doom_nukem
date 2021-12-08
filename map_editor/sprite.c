#include "editor.h"

t_sprite	*sprite_new(void)
{
	t_sprite	*sprite;

	sprite = ft_memalloc(sizeof(t_sprite));
	return (sprite);
}

void	sprite_free(void *s, size_t size)
{
	t_sprite	*sprite;

	sprite = s;
	if (!sprite)
		return ;
	free(sprite);
}

void	sprite_print(t_sprite *sprite)
{
	ft_printf("[%s]\n", __FUNCTION__);
	ft_printf("\tpos : ");
	print_veci(sprite->pos.v, 2);
	ft_printf("\ttexture_id : %d\n", sprite->texture_id);
	ft_printf("\tscale : %.2f\n", sprite->scale);
	ft_printf("\ttype : %d\n", sprite->type);
}

t_sprite	*add_sprite(t_editor *editor)
{
	t_sprite	*sprite;

	sprite = sprite_new();
	sprite->pos = vec4i(0, 0, 64, 64);
	sprite->texture_id = 0;
	sprite->id = get_next_sprite_id(editor->sprites);
	sprite->scale = 1.0f;
	add_to_list(&editor->sprites, sprite, sizeof(t_sprite));
	++editor->sprite_amount;
	ft_printf("New Sprite Added (%d)\n", editor->sprite_amount);
	return (sprite);
}

/*
 * Removes all traces of this sprite;
*/
void	remove_sprite(t_editor *editor, t_sprite *sprite)
{
	if (!sprite)
		return ;
	remove_from_list(&editor->sprites, sprite);
	if (!sprite->parent)
		ft_printf("[%s] Sprite doesnt have parent... Error!\n", __FUNCTION__);
	else
		remove_from_list(&sprite->parent->sprites, sprite);
	sprite_free(sprite, sizeof(t_sprite));
	sprite = NULL;
	--editor->sprite_amount;
}

/*
 * When you select a sprite, update ui;
*/
void	set_sprite_ui(t_editor *editor, t_sprite *sprite)
{
	char	temp_str[20];

	if (!sprite || !editor)
		return ;
	editor->sprite_texture_something.id = sprite->texture_id;
	ui_element_image_set(editor->sprite_texture_image, UI_STATE_AMOUNT,
		editor->wall_textures[sprite->texture_id]);
	ui_input_set_text(editor->sprite_scale_input,
		ft_b_ftoa(sprite->scale, 2, temp_str));
	if (sprite->type == STATIC)
		ui_dropdown_activate(editor->sprite_type_dropdown,
			editor->sprite_type_static);
	else if (sprite->type == LOOP)
		ui_dropdown_activate(editor->sprite_type_dropdown,
			editor->sprite_type_loop);
	else if (sprite->type == ACTION)
		ui_dropdown_activate(editor->sprite_type_dropdown,
			editor->sprite_type_action);
	ui_input_set_text(editor->sprite_x_input,
		ft_b_itoa(sprite->pos.x, temp_str));
	ui_input_set_text(editor->sprite_y_input,
		ft_b_itoa(sprite->pos.y, temp_str));
}

void	get_sprite_ui(t_editor *editor, t_sprite *sprite)
{
	if (!sprite || !editor)
		return ;
	sprite->texture_id = editor->sprite_texture_something.id;
	if (ui_input_exit(editor->sprite_x_input))
		sprite->pos.x = ft_atoi(ui_input_get_text(editor->sprite_x_input));
	if (ui_input_exit(editor->sprite_y_input))
		sprite->pos.y = ft_atoi(ui_input_get_text(editor->sprite_y_input));
	if (ui_input_exit(editor->sprite_scale_input))
		sprite->scale = ft_atof(ui_input_get_text(editor->sprite_scale_input));
	if (ui_dropdown_exit(editor->sprite_type_dropdown))
	{
		if (editor->sprite_type_loop->state == UI_STATE_CLICK)
			sprite->type = LOOP;
		else if (editor->sprite_type_action->state == UI_STATE_CLICK)
			sprite->type = ACTION;
		else
			sprite->type = STATIC;
	}
}

int	get_next_sprite_id(t_list *list)
{
	t_sprite	*event;
	int			i;
	char		ids[1024];

	i = -1;
	memset(ids, 0, sizeof(char) * 1024);
	while (list)
	{
		event = list->content;
		ids[event->id] = 1;
		list = list->next;
	}
	while (ids[++i])
		;
	return (i);
}

t_sprite	*get_sprite_from_list_at_pos(t_list *list, t_vec2i pos)
{
	t_list		*curr;
	t_sprite	*sprite;

	curr = list;
	while (curr)
	{
		sprite = curr->content;
		if (vec2_in_vec4(pos, sprite->pos))
			return (sprite);
		curr = curr->next;
	}
	return (NULL);
}

t_sprite	*get_sprite_by_id_from_list(t_list *list, int id)
{
	while (list)
	{
		if (((t_sprite *)list->content)->id == id)
			return (list->content);
		list = list->next;
	}
	return (NULL);
}
