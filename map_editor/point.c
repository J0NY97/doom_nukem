#include "editor.h"

void	point_render(t_editor *editor, t_point *point, Uint32 color)
{
	ui_surface_circle_draw_filled(editor->drawing_surface,
		conversion(editor, point->pos), 3, color);
}

/*
 * Creates new point and adds it to editor->points;
 * Returns itself;
*/
t_point	*add_point(t_editor *editor)
{
	t_point	*point;

	point = ft_memalloc(sizeof(t_point));
	++editor->point_amount;
	add_to_list(&editor->points, point, sizeof(t_point));
	return (point);
}

/*
 * Removes point from all sectors, walls,
 * 	and from anything else that are using this point;
*/
int	remove_point(t_editor *editor, t_point *point)
{
	if (!point)
		return (0);
	remove_from_list(&editor->points, point);
	free(point);
	point = NULL;
	ft_printf("[%s] Done removing point.\n", __FUNCTION__);
	--editor->point_amount;
	return (1);
}

t_point	*get_point_with_id(t_list *list, int id)
{
	while (list)
	{
		if (((t_point *)list->content)->id == id)
			return (list->content);
		list = list->next;
	}
	return (NULL);
}

t_point	*get_point_from_list_at_pos(t_list *list, t_vec2i v)
{
	t_list	*curr;
	t_point	*p;

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

t_point	*get_point_from_list_around_radius(
		t_list *points, t_vec2i pos, float allowed_radius)
{
	t_point		*temp;
	float		x;
	float		y;

	temp = get_point_from_list_at_pos(points, pos);
	if (temp)
		return (temp);
	x = -allowed_radius;
	while (x <= allowed_radius)
	{
		y = -allowed_radius;
		while (y <= allowed_radius)
		{
			temp = get_point_from_list_at_pos(points,
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

t_point	*get_point_from_wall_at_pos(t_wall *wall, t_vec2i pos)
{
	if (compare_veci(wall->p1->pos.v, pos.v, 2))
		return (wall->p1);
	else if (compare_veci(wall->p2->pos.v, pos.v, 2))
		return (wall->p2);
	return (NULL);
}

t_point	*get_point_from_sector_around_radius(
		t_sector *sector, t_vec2i pos, float allowed_radius)
{
	t_list		*curr;
	t_point		*temp;
	float		x;
	float		y;

	curr = sector->walls;
	while (curr)
	{
		x = -allowed_radius;
		while (x <= allowed_radius)
		{
			y = -allowed_radius;
			while (y <= allowed_radius)
			{
				temp = get_point_from_wall_at_pos(curr->content,
						vec2i(pos.x + (int)x, pos.y + (int)y));
				if (temp)
					return (temp);
				y += 0.5f;
			}
			x += 0.5f;
		}
		curr = curr->next;
	}
	return (NULL);
}

void	remove_from_list(t_list **list, void *pointer)
{
	t_list	*curr;

	curr = *list;
	while (curr)
	{
		if (curr->content == pointer)
			ft_lstdelone_nonfree(list, curr);
		curr = curr->next;
	}
}

/*
 * Is this point a point in any of the point in wall;
*/
int	point_in_wall(t_wall *wall, t_point *point)
{
	return (wall->p1 == point || wall->p2 == point);
}

/*
 * Is this point a point in any of the walls in sector;
*/
int	point_in_sector(t_sector *sector, t_point *point)
{
	t_list	*curr;

	if (!sector || !point)
		return (0);
	curr = sector->walls;
	while (curr)
	{
		if (point_in_wall(curr->content, point))
			return (1);
		curr = curr->next;
	}
	return (0);
}

/*
 *  Check if the point is in any of the list of sectors;
 *  	(must be list of t_sector);
*/
int	point_in_any_sector(t_list *sectors, t_point *point)
{
	while (sectors)
	{
		if (point_in_sector(sectors->content, point))
			return (1);
		sectors = sectors->next;
	}
	return (0);
}

void	remove_all_lonely_points(t_editor *editor)
{
	t_list	*curr;
	t_list	*next;

	curr = editor->points;
	while (curr)
	{
		next = curr->next;
		if (!point_in_any_sector(editor->sectors, curr->content))
			remove_point(editor, curr->content);
		curr = next;
	}
}

void	draw_points(t_editor *editor, t_list *points, Uint32 color)
{
	while (points)
	{
		point_render(editor, points->content, color);
		points = points->next;
	}
}
