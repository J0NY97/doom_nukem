/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 12:44:43 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/28 12:44:44 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_sprite	*new_sprite(void)
{
	t_sprite	*sprite;

	sprite = malloc(sizeof(t_sprite));
	memset(sprite, 0, sizeof(t_sprite));
	sprite->sprite_id = 0;
	sprite->scale = 1.0f;
	sprite->real_x = 0;
	sprite->real_y = 0;
	sprite->coord.x = 0;
	sprite->coord.y = 0;
	sprite->coord.w = 0;
	sprite->coord.h = 0;
	return (sprite);
}

void	free_sprite(void *content, size_t size)
{
	t_sprite	*sprite;

	ft_putstr("[free_sprite]\n");
	(void)size;
	if (content == NULL)
		return ;
	sprite = content;
	free(sprite);
}

void	remove_from_sprites(t_list **list, t_sprite *s)
{
	remove_from_list_if_with(list, s, &pointer_compare, &free_sprite);
}

int	sprite_compare(t_sprite *bubble, t_sprite *gum)
{
	if (bubble->sprite_id == gum->sprite_id
		&& bubble->coord.x == gum->coord.x
		&& bubble->coord.y == gum->coord.y
		&& bubble->coord.w == gum->coord.w
		&& bubble->coord.h == gum->coord.h
		&& bubble->scale == gum->scale)
		return (1);
	return (0);
}
