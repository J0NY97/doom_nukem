#include "editor.h"

t_point	*get_point_from_wall_in_sector(t_sector *sector, t_point *v)
{
	t_wall	*w;
	t_list	*wall;

	wall = sector->walls;
	while (wall)
	{
		w = wall->content;
		if (vector_compare(w->orig->pos, v->pos))
			return (w->orig);
		else if (vector_compare(w->dest->pos, v->pos))
			return (w->dest);
		wall = wall->next;
	}
	return (NULL);
}

t_point	*get_existing_point_or_new(t_grid *grid, t_vector pos)
{
	t_point	*point;

	point = get_point_from_wall_in_sector(grid->modify_sector,
			&(t_point){0, pos});
	if (point == NULL)
	{
		point = new_point(pos);
		add_to_list(&grid->points, point, sizeof(t_point));
	}
	return (point);
}

t_wall	*get_existing_wall_or_new(t_grid *grid, t_point *temp1, t_point *temp2)
{
	t_wall	*wall;

	wall = NULL;
	wall = get_wall_from_list(&grid->modify_sector->walls, temp1, temp2);
	if (wall == NULL)
	{
		wall = new_wall(temp1, temp2);
		add_to_list(&grid->walls, wall, sizeof(t_wall));
		add_to_list(&grid->modify_sector->walls, wall, sizeof(t_wall));
	}
	return (wall);
}

void	check_selected(t_grid *grid)
{
	t_point	*temp1;
	t_point	*temp2;
	t_wall	*temp_wall;

	temp1 = NULL;
	temp2 = NULL;
	temp_wall = NULL;
	if (vector_is_empty(grid->selected2))
		return ;
	temp1 = get_existing_point_or_new(grid, grid->selected1);
	temp2 = get_existing_point_or_new(grid, grid->selected2);
	temp_wall = get_existing_wall_or_new(grid, temp1, temp2);
	if (grid->modify_sector->first_point == NULL)
		grid->modify_sector->first_point = temp1;
	if (grid->modify_sector->first_point == temp_wall->dest)
	{
		grid->modify_sector->first_point = NULL;
		grid->modify_sector = NULL;
		grid->selected1 = (t_vector){0, 0, 0};
		grid->selected2 = (t_vector){0, 0, 0};
	}
	grid->selected1 = grid->selected2;
	grid->selected2 = (t_vector){0, 0, 0};
}
