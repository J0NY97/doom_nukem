/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bui_preset_dropdown.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 14:05:32 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/27 14:05:33 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "better_libui.h"

t_preset_dropdown	*bui_new_dropdown_preset(
		t_bui_element *menu, char *text, t_xywh pos)
{
	t_preset_dropdown	*drop;

	drop = ft_memalloc(sizeof(t_preset_dropdown));
	drop->drop = bui_new_element(menu, text, pos);
	drop->arrow = bui_new_element(drop->drop, text,
			(t_xywh){pos.w - 20, 0, pos.h, 20});
	bui_set_element_text_position(drop->arrow, 5, 2);
	bui_set_element_text(drop->arrow, "<");
	drop->bass = bui_new_element(menu, "view", (t_xywh){
			drop->drop->position.x,
			drop->drop->position.y + drop->drop->position.h,
			drop->drop->position.w,
			drop->drop->position.h});
	drop->bass->show = 0;
	drop->elements = NULL;
	drop->active = NULL;
	drop->count = 0;
	return (drop);
}

void	dropdown_preset_free(void *preset, size_t size)
{
	t_preset_dropdown	*drop;

	drop = preset;
	ft_lstdel(&drop->elements, &dummy_free_er);
	ft_memdel((void **)&drop);
}

t_bui_element	*preset_dropdown_add_element(
		t_preset_dropdown *dropdown, char *text)
{
	t_bui_element	*elem;
	t_xywh			new_pos;

	elem = bui_new_element(dropdown->bass, text, (t_xywh){
			0, dropdown->drop->position.h * (dropdown->count),
			dropdown->bass->position.w,
			dropdown->drop->position.h});
	dropdown->count++;
	new_pos = dropdown->bass->position;
	new_pos.h = dropdown->drop->position.h * (dropdown->count);
	bui_element_resize(dropdown->bass, new_pos);
	add_to_list(&dropdown->elements, elem, sizeof(t_bui_element));
	return (elem);
}

int	preset_dropdown_events(t_preset_dropdown *dropdown)
{
	bui_button_toggle(dropdown->drop);
	dropdown->bass->show = dropdown->drop->toggle;
	bui_set_element_text(dropdown->arrow, "<");
	if (dropdown->bass->show)
	{
		bui_set_element_text(dropdown->arrow, "^");
		if (only_one_button_toggled_at_a_time(dropdown->elements,
				&dropdown->active))
		{
			dropdown->bass->show = 0;
			dropdown->drop->toggle = 0;
		}
	}
	if (dropdown->active != NULL)
	{
		bui_set_element_text(dropdown->drop, dropdown->active->text);
		SDL_BlitScaled(dropdown->active->surface[0], NULL,
			dropdown->drop->surface[0], NULL);
		SDL_BlitScaled(dropdown->active->surface[1], NULL,
			dropdown->drop->surface[1], NULL);
		SDL_BlitScaled(dropdown->active->surface[2], NULL,
			dropdown->drop->surface[2], NULL);
		return (1);
	}
	return (0);
}
