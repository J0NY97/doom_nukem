#include "editor.h"

t_changer_prefab *
	new_changer_prefab(t_bui_element *parent_menu, char *title, t_xywh coord)
{
	t_changer_prefab	*prefab;
	t_xywh				temp_coord;

	prefab = ft_memalloc(sizeof(t_changer_prefab));
	prefab->menu = bui_new_element(parent_menu, title, coord);
	bui_set_element_flags(prefab->menu, BUI_ELEMENT_DONT_UPDATE_STATE);
	temp_coord = new_xywh(0, coord.h - 20, 20, 20);
	prefab->sub_button = bui_new_element(prefab->menu, "-", temp_coord);
	temp_coord = new_xywh(coord.w - 20, coord.h - 20, 20, 20);
	prefab->add_button = bui_new_element(prefab->menu, "+", temp_coord);
	temp_coord.x = prefab->sub_button->position.x
		+ prefab->sub_button->position.w;
	temp_coord.w = prefab->menu->position.w - temp_coord.x
		- (prefab->menu->position.w - prefab->add_button->position.x);
	prefab->value = bui_new_element(prefab->menu, "not set", temp_coord);
	bui_set_element_flags(prefab->value, BUI_ELEMENT_DONT_UPDATE_STATE);
	return (prefab);
}

void	changer_prefab_events(
			t_changer_prefab *changer,
			int *current_value,
			int change_amount)
{
	char	*str;

	str = NULL;
	if (bui_button(changer->add_button))
		*current_value += change_amount;
	else if (bui_button(changer->sub_button))
		*current_value -= change_amount;
	str = ft_sprintf("%d", *current_value);
	bui_set_element_text(changer->value, str);
	ft_strdel(&str);
}

void	changer_prefab_events_float(
			t_changer_prefab *changer,
			float *current_value,
			float change_amount)
{
	char	*str;

	str = NULL;
	if (bui_button(changer->add_button))
		*current_value += change_amount;
	else if (bui_button(changer->sub_button))
		*current_value -= change_amount;
	str = ft_sprintf("%.1f", *current_value);
	bui_set_element_text(changer->value, str);
	ft_strdel(&str);
}
