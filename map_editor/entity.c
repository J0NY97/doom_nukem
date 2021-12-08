#include "editor.h"

t_entity	*entity_new(void)
{
	t_entity	*entity;

	entity = ft_memalloc(sizeof(t_entity));
	return (entity);
}

void	entity_free(t_entity *entity)
{
	if (!entity)
		return ;
	free(entity);
	entity = NULL;
}

t_entity	*add_entity(t_editor *editor)
{
	t_entity	*entity;

	entity = entity_new();
	entity->type = 1;
	++editor->entity_amount;
	add_to_list(&editor->entities, entity, sizeof(t_entity));
	return (entity);
}

int	remove_entity(t_editor *editor, t_entity *entity)
{
	if (!entity)
		return (0);
	remove_from_list(&editor->entities, entity);
	entity_free(entity);
	--editor->entity_amount;
	ft_printf("[%s] Entity Removed.\n", __FUNCTION__);
	return (1);
}

void	entity_render(t_editor *editor, t_entity *entity)
{
	t_vec2i	www;
	t_vec2i	eee;

	www = conversion(editor, entity->pos);
	eee = vec2i(20, 20);
	SDL_SetRenderTarget(editor->win_main->renderer, editor->win_main->texture);
	if (editor->entity_textures[entity->type])
	{
		SDL_RenderCopy(editor->win_main->renderer,
			editor->entity_textures[entity->type], NULL,
			&(SDL_Rect){www.x - (eee.x / 2), www.y - (eee.y / 2),
			eee.x, eee.y});
	}
	else
		ft_printf("Entity texture doesnt exist\n");
	SDL_SetRenderTarget(editor->win_main->renderer, NULL);
}

void	set_entity_ui(t_editor *editor, t_entity *entity)
{
	char	temp_str[20];

	ui_input_set_text(editor->entity_yaw_input,
		ft_b_itoa(entity->yaw, temp_str));
	ui_slider_value_set(editor->entity_yaw_slider, entity->yaw);
	ui_input_set_text(editor->entity_z_input, ft_b_itoa(entity->z, temp_str));
	ui_element_image_set(editor->entity_image, UI_STATE_DEFAULT,
		editor->entity_texture_surfaces[entity->type]);
	ui_dropdown_activate(editor->entity_dropdown,
		ui_dropdown_get_button_with_text(editor->entity_dropdown,
			g_entity_data[entity->type - 1].name));
}

/*
 * I know we are starting at 1, not set entity type is 0;
*/
int	get_entity_type(char *text)
{
	int		i;

	i = 0;
	while (++i <= ENTITY_AMOUNT)
	{
		if (ft_strequ(g_entity_data[i - 1].name, text))
			return (i);
	}
	return (0);
}

void	get_entity_ui(t_editor *editor, t_entity *entity)
{
	char	temp_str[20];

	if (ui_input_exit(editor->entity_yaw_input))
		entity->yaw = ft_clamp(ft_atoi(
					ui_input_get_text(editor->entity_yaw_input)), 0, 360);
	else if (ui_slider_updated(editor->entity_yaw_slider))
		entity->yaw = ui_slider_value_get(editor->entity_yaw_slider);
	if (ui_input_exit(editor->entity_yaw_input))
		ui_slider_value_set(editor->entity_yaw_slider, entity->yaw);
	if (ui_slider_updated(editor->entity_yaw_slider))
		ui_input_set_text(editor->entity_yaw_input,
			ft_b_itoa(entity->yaw, temp_str));
	if (ui_input_exit(editor->entity_z_input))
		entity->z = ft_atoi(ui_input_get_text(editor->entity_z_input));
	if (ui_dropdown_exit(editor->entity_dropdown)
		&& ui_dropdown_active(editor->entity_dropdown))
	{
		entity->type = get_entity_type(
				ui_dropdown_active_text(editor->entity_dropdown));
		ui_element_image_set(editor->entity_image, UI_STATE_DEFAULT,
			editor->entity_texture_surfaces[entity->type]);
	}
}

void	print_entities(t_list *entities)
{
	while (entities)
	{
		print_entity(entities->content);
		entities = entities->next;
	}
}

void	print_entity(t_entity *entity)
{
	ft_printf("[ENTITY]\n");
	ft_printf("name : %s\n", g_entity_data[entity->type].name);
	ft_printf("pos : ");
	print_veci(entity->pos.v, 2);
	ft_printf("z : %d\n", entity->z);
	ft_printf("yaw : %d\n", entity->yaw);
	ft_printf("inside sector ? : %d\n", entity->inside_sector != NULL);
}

t_entity	*get_entity_from_list_at_pos(t_list *list, t_vec2i v)
{
	t_list		*curr;
	t_entity	*p;

	curr = list;
	while (curr)
	{
		p = curr->content;
		if (compare_veci(p->pos.v, v.v, 2))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

t_entity	*get_entity_from_list_around_radius(
		t_list *points, t_vec2i pos, float allowed_radius)
{
	t_entity	*temp;
	float		x;
	float		y;

	temp = get_entity_from_list_at_pos(points, pos);
	if (temp)
		return (temp);
	x = -allowed_radius;
	while (x <= allowed_radius)
	{
		y = -allowed_radius;
		while (y <= allowed_radius)
		{
			temp = get_entity_from_list_at_pos(points,
					vec2i(pos.x + x, pos.y + y));
			if (temp != NULL)
				break ;
			y += 0.5f;
		}
		if (temp != NULL)
			break ;
		x += 0.5f;
	}
	return (temp);
}

void	entity_check_errors(t_editor *editor, t_entity *entity)
{
	if (!entity->inside_sector)
	{
		draw_text(editor->drawing_surface, "Not Inside Sector!",
			editor->font, conversion(editor, entity->pos), 0xffff0000);
		editor->errors += 1;
	}
	else if (entity->inside_sector
		&& (entity->z < entity->inside_sector->floor_height
			|| entity->z > entity->inside_sector->ceiling_height))
	{
		draw_text(editor->drawing_surface, "Z not between Floor & Ceiling!",
			editor->font, conversion(editor, entity->pos), 0xffff0000);
		editor->errors += 1;
	}
}
