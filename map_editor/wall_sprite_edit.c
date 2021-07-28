#include "editor.h"

void	move_selected_sprite(t_editor *editor)
{
	float	move_speed;

	move_speed = 0.1f;
	if (editor->grid.modify_sprite != NULL)
	{
		if (key_pressed(editor->libui, SDL_SCANCODE_LEFT))
			editor->grid.modify_sprite->real_x -= move_speed;
		else if (key_pressed(editor->libui, SDL_SCANCODE_RIGHT))
			editor->grid.modify_sprite->real_x += move_speed;
		if (key_pressed(editor->libui, SDL_SCANCODE_UP))
			editor->grid.modify_sprite->real_y -= move_speed;
		else if (key_pressed(editor->libui, SDL_SCANCODE_DOWN))
			editor->grid.modify_sprite->real_y += move_speed;
		changer_prefab_events_float(editor->sprite_scale_changer,
			&editor->grid.modify_sprite->scale, 0.1f);
		if (bui_button(editor->remove_wall_sprite_button))
		{
			remove_from_sprites(&editor->grid.modify_wall->sprites,
				editor->grid.modify_sprite);
			editor->grid.modify_sprite = NULL;
		}
	}
}

void	change_selected_sprite(t_editor *editor)
{
	if (editor->grid.modify_wall->sprites != NULL)
	{
		changer_prefab_events(editor->sprite_changer,
			&editor->selected_sprite, 1);
		if (editor->selected_sprite
			>= ft_lstlen(editor->grid.modify_wall->sprites))
			editor->selected_sprite = 0;
		else if (editor->selected_sprite < 0)
			editor->selected_sprite = ft_lstlen(
					editor->grid.modify_wall->sprites) - 1;
		editor->grid.modify_sprite = get_nth_from_list(
				&editor->grid.modify_wall->sprites,
				editor->selected_sprite)->content;
	}
}

void	change_selected_sprite_texture(t_editor *editor)
{
	char	*temp;

	if (editor->grid.modify_sprite != NULL)
	{
		temp = ft_itoa(editor->grid.modify_sprite->sprite_id);
		if (only_one_button_toggled_at_a_time(editor->wall_sprite_buttons,
				&editor->active_wall_sprite))
		{
		}
		else
			toggle_on_element_with_text(editor->wall_sprite_buttons,
				&editor->active_wall_sprite, temp);
		ft_strdel(&temp);
		if (editor->active_wall_sprite != NULL)
			editor->grid.modify_sprite->sprite_id
				= ft_atoi(editor->active_wall_sprite->text);
	}
}

void	wall_sprite_view_events(t_editor *editor)
{
	t_sprite	*sprite;

	if (bui_button(editor->add_wall_sprite_button))
	{
		sprite = new_sprite();
		sprite->sprite_id = 0;
		add_to_list(&editor->grid.modify_wall->sprites,
			sprite, sizeof(t_sprite));
		editor->grid.modify_sprite = sprite;
	}
	change_selected_sprite_texture(editor);
	change_selected_sprite(editor);
	move_selected_sprite(editor);
}
