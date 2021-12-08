#include "editor.h"

t_event	*event_new(void)
{
	t_event	*event;

	event = ft_memalloc(sizeof(t_event));
	return (event);
}

void	event_free(t_event *event)
{
	event->pointer = NULL;
	ft_strdel(&event->sector);
	free(event);
}

t_event	*add_event(t_editor *editor)
{
	t_event	*event;
	t_vec2	new_pos;

	event = event_new();
	event->elem = event_element_new(editor->event_menu);
	event->id = get_next_event_id(editor->events);
	event->elem->event = event;
	new_pos = vec2(event->elem->menu.pos.x,
			((t_ui_scrollbar *)editor->event_scrollbar->element)->bot_most.y
			+ 10);
	ui_element_pos_set2(&event->elem->menu, new_pos);
	add_to_list(&editor->event_elements, event->elem, sizeof(t_event_elem));
	add_to_list(&editor->event_element_buttons,
		event->elem->button, sizeof(t_ui_element));
	add_to_list(&editor->events, event, sizeof(t_event));
	++editor->event_amount;
	ft_printf("New event added (%d)\n", editor->event_amount);
	ui_scrollbar_event(editor->event_scrollbar, (SDL_Event){});
	ui_scrollbar_render(editor->event_scrollbar);
	ui_scroll_value_set(editor->event_scrollbar,
		((t_ui_scrollbar *)editor->event_scrollbar->element)->max);
	return (event);
}

void	realign_event_buttons(t_editor *editor)
{
	t_list			*curr;
	t_event_elem	*event_elem;
	int				new_val;
	int				h;
	int				i;

	curr = editor->event_elements;
	i = 0;
	h = 60;
	while (curr && curr->next)
		curr = curr->next;
	while (curr)
	{
		event_elem = curr->content;
		ui_element_pos_set2(&event_elem->menu, vec2(event_elem->menu.pos.x,
				((t_ui_scrollbar *)editor->event_scrollbar->element)->top_most.y
				+ (event_elem->menu.pos.h * i) + (i * 10)));
		i++;
		curr = curr->prev;
	}
	new_val = ft_max(((t_ui_scrollbar *)editor->event_scrollbar->element)->value
			- h - 10, 0);
	ui_scroll_value_set(editor->event_scrollbar, new_val);
	((t_ui_scrollbar *)editor->event_scrollbar->element)->update = 1;
}

/*
 * ft_printf("If you see ui_element_remove_child_parent,
 * 	dont get scared, those are coming from the ui_element_free.\n");
*/
void	remove_event(t_editor *editor, t_event *event)
{
	ft_printf("[%s] Removing event.\n", __FUNCTION__);
	ui_element_remove_from_list(event->elem->button,
		&editor->event_element_buttons);
	remove_event_elem_from_list(event->elem, &editor->event_elements);
	remove_event_from_list(event, &editor->events);
	event_elem_free(event->elem);
	event_free(event);
	--editor->event_amount;
	realign_event_buttons(editor);
	ft_printf("Removing event. (total : %d)\n", editor->event_amount);
}

/*
 * Try to activate button with the 'target_id_text',
 * 	if it doesnt exist we update the list,
 * 	and try again.
*/
/*
 * TODO: we have problem here,
 * if you update id drop with more sectors than sprites,
 * and then open sprite ids with less buttons;
*/
void	activate_id_button(t_editor *editor, int type, char *target_id_text)
{
	t_ui_element	*id_button;
	t_ui_recipe		*recipe;
	char			**texts;

	id_button = ui_dropdown_get_button_with_text(
			editor->event_id_dropdown, target_id_text);
	if (!id_button)
	{
		if (type == TYPE_SECTOR)
			texts = gen_sector_id_texts(editor->sectors);
		else
			return ;
		recipe = ui_layout_get_recipe(&editor->layout, "event_id_button");
		create_buttons_to_list_from_texts_remove_extra(
			ui_dropdown_get_menu_element(editor->event_id_dropdown),
			texts, recipe);
		ft_arraydel(texts);
		id_button = ui_dropdown_get_button_with_text(
				editor->event_id_dropdown, target_id_text);
	}
	ui_dropdown_activate(editor->event_id_dropdown, id_button);
}

void	set_event_ui(t_editor *editor, t_event *event)
{
	char	temp_str[20];

	ui_dropdown_activate(editor->event_type_dropdown,
		ui_dropdown_get_button_with_text(editor->event_type_dropdown,
			(char *)g_event_type[event->type]));
	ui_dropdown_activate(editor->event_action_dropdown,
		ui_dropdown_get_button_with_text(editor->event_action_dropdown,
			(char *)g_event_action[event->action].name));
	if (event->pointer)
	{
		if (event->pointer_type == TYPE_SECTOR)
			ft_b_itoa(((t_sector *)event->pointer)->id, temp_str);
		else
			ft_b_itoa(((t_sprite *)event->pointer)->id, temp_str);
		activate_id_button(editor, event->pointer_type, temp_str);
	}
	if (event->sector)
		ui_input_set_text(editor->event_sector_input, event->sector);
	ui_input_set_text(editor->event_min_input, ft_b_itoa(event->min, temp_str));
	ui_input_set_text(editor->event_max_input, ft_b_itoa(event->max, temp_str));
	ui_input_set_text(editor->event_speed_input,
		ft_b_itoa(event->speed, temp_str));
}

void	remove_event_from_list(t_event *event, t_list **list)
{
	t_list	*curr;

	curr = *list;
	while (curr)
	{
		if (event == curr->content)
			ft_lstdelone_nonfree(list, curr);
		curr = curr->next;
	}
}

void	event_elem_free(t_event_elem *elem)
{
	elem->menu.free_me = 0;
	elem->id.free_me = 0;
	elem->type.free_me = 0;
	elem->action.free_me = 0;
	elem->target_id.free_me = 0;
	elem->sector.free_me = 0;
	elem->min.free_me = 0;
	elem->max.free_me = 0;
	elem->speed.free_me = 0;
	ui_element_set_id(&elem->menu, "menu");
	ui_element_set_id(&elem->id, "id");
	ui_element_set_id(&elem->type, "type");
	ui_element_set_id(&elem->action, "action");
	ui_element_set_id(&elem->target_id, "target_id");
	ui_element_set_id(&elem->sector, "sector");
	ui_element_set_id(&elem->min, "min");
	ui_element_set_id(&elem->max, "max");
	ui_element_set_id(&elem->speed, "speed");
	ui_element_set_id(elem->button, "button");
	ui_element_free(&elem->menu, elem->menu.element_type);
	/*
	ui_element_free(elem->button, elem->button->element_type);
	ui_element_free(&elem->id, elem->id.element_type);
	ui_element_free(&elem->type, elem->type.element_type);
	ui_element_free(&elem->action, elem->action.element_type);
	ui_element_free(&elem->target_id, elem->target_id.element_type);
	ui_element_free(&elem->sector, elem->sector.element_type);
	ui_element_free(&elem->min, elem->min.element_type);
	ui_element_free(&elem->max, elem->max.element_type);
	ui_element_free(&elem->speed, elem->speed.element_type);
	*/
	elem->event = NULL;
	free(elem);
}

void	remove_event_elem_from_list(t_event_elem *elem, t_list **list)
{
	t_list	*curr;

	curr = *list;
	while (curr)
	{
		if (curr->content == elem)
			ft_lstdelone_nonfree(list, curr);
		curr = curr->next;
	}
}

t_event_elem	*event_element_new(t_ui_element *parent)
{
	t_event_elem	*event_elem;

	event_elem = ft_memalloc(sizeof(t_event_elem));
	set_elem_parent_and_recipe(&event_elem->menu,
		UI_TYPE_MENU, parent, "event_menu_prefab");
	set_elem_parent_and_recipe(&event_elem->id,
		UI_TYPE_LABEL, &event_elem->menu, "event_id_prefab");
	set_elem_parent_and_recipe(&event_elem->type,
		UI_TYPE_LABEL, &event_elem->menu, "event_type_prefab");
	set_elem_parent_and_recipe(&event_elem->action,
		UI_TYPE_LABEL, &event_elem->menu, "event_action_prefab");
	set_elem_parent_and_recipe(&event_elem->target_id,
		UI_TYPE_LABEL, &event_elem->menu, "event_target_id_prefab");
	set_elem_parent_and_recipe(&event_elem->sector,
		UI_TYPE_LABEL, &event_elem->menu, "event_sector_prefab");
	set_elem_parent_and_recipe(&event_elem->min,
		UI_TYPE_LABEL, &event_elem->menu, "event_min_prefab");
	set_elem_parent_and_recipe(&event_elem->max,
		UI_TYPE_LABEL, &event_elem->menu, "event_max_prefab");
	set_elem_parent_and_recipe(&event_elem->speed,
		UI_TYPE_LABEL, &event_elem->menu, "event_speed_prefab");
	event_elem->button = ft_memalloc(sizeof(t_ui_element));
	set_elem_parent_and_recipe(event_elem->button,
		UI_TYPE_BUTTON, &event_elem->menu, "event_button_prefab");
	return (event_elem);
}

/*
 * Update the selected event with the selected info on the event edit menu;
 * NOTE: this is the first input getter function we made,
 * 		but this is basically 'get_event_ui';
 *
 * char	*active_text; // DONT FREE!
*/
void	update_event2(t_editor *editor, t_event *event)
{
	char	*active_text;

	active_text = ui_dropdown_active_text(editor->event_id_dropdown);
	if (active_text)
	{
		if (event->action == SECTOR)
		{
			event->pointer_type = TYPE_SECTOR;
			event->pointer = get_sector_by_id_from_list(editor->sectors,
					ft_atoi(active_text));
		}
		else
		{
			event->pointer_type = TYPE_SPRITE;
			event->pointer = get_sprite_by_id_from_list(editor->sprites,
					ft_atoi(active_text));
		}
	}
	active_text = ui_input_get_text(editor->event_sector_input);
	if (active_text)
	{
		if (event->sector)
			ft_strdel(&event->sector);
		event->sector = ft_strdup(active_text);
	}
}

void	update_event3(t_editor *editor, t_event *event)
{
	char	*active_text;

	active_text = ui_input_get_text(editor->event_min_input);
	if (active_text)
		event->min = ft_atoi(active_text);
	active_text = ui_input_get_text(editor->event_max_input);
	if (active_text)
		event->max = ft_atoi(active_text);
	active_text = ui_input_get_text(editor->event_speed_input);
	if (active_text)
		event->speed = ft_atoi(active_text);
}

void	update_event(t_editor *editor, t_event *event)
{
	int		i;
	char	*active_text;

	active_text = ui_dropdown_active_text(editor->event_type_dropdown);
	i = -1;
	while (++i < EVENT_TYPE_AMOUNT)
		if (ft_strequ(active_text, g_event_type[i]))
			event->type = i;
	active_text = ui_dropdown_active_text(editor->event_action_dropdown);
	i = -1;
	while (++i < EVENT_ACTION_AMOUNT)
		if (ft_strequ(active_text, g_event_action[i].name))
			event->action = g_event_action[i].id;
	update_event2(editor, event);
	update_event3(editor, event);
}

/*
 * From the event in elem, update all the corresponding elems in elem;
*/
void	update_event_elem(t_event_elem *elem)
{
	char	temp[29];

	ft_printf("[%s] Updating event_elem. ", __FUNCTION__);
	ui_label_set_text(&elem->id, ft_b_itoa(elem->event->id, temp));
	ui_label_set_text(&elem->type, (char *)g_event_type[elem->event->type]);
	ui_label_set_text(&elem->action, g_event_action[elem->event->action].name);
	if (elem->event->pointer)
	{
		if (elem->event->pointer_type == TYPE_SECTOR)
			ui_label_set_text(&elem->target_id,
				ft_b_itoa(((t_sector *)elem->event->pointer)->id, temp));
		else
			ui_label_set_text(&elem->target_id,
				ft_b_itoa(((t_sprite *)elem->event->pointer)->id, temp));
	}
	ui_label_set_text(&elem->sector, ft_sprintf("%s", elem->event->sector));
	ui_label_set_text(&elem->min, ft_b_itoa(elem->event->min, temp));
	ui_label_set_text(&elem->max, ft_b_itoa(elem->event->max, temp));
	ui_label_set_text(&elem->speed, ft_b_itoa(elem->event->speed, temp));
	ft_printf("Success.\n");
}

int	get_next_event_id(t_list *list)
{
	t_event		*event;
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

int	get_event_type(char *type_name)
{
	int	ddd;

	ddd = -1;
	while (++ddd < EVENT_TYPE_AMOUNT)
		if (ft_strequ(type_name, g_event_type[ddd]))
			return (ddd);
	return (0);
}

int	get_event_action(char *type_name)
{
	int	ddd;

	ddd = -1;
	while (++ddd < EVENT_ACTION_AMOUNT)
		if (ft_strequ(type_name, g_event_action[ddd].name))
			return (g_event_action[ddd].id);
	return (0);
}
