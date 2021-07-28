#include "editor.h"

void	unselect_selected(t_editor *editor, t_grid *grid)
{
	grid->selected1 = (t_vector){0, 0, 0};
	grid->selected2 = (t_vector){0, 0, 0};
	grid->modify_point = NULL;
	grid->modify_wall = NULL;
	grid->modify_entity = NULL;
	editor->grid.modify_sprite = NULL;
	grid->modify_sector = NULL;
}

t_vector	*get_scaled_line(
	SDL_Surface *surface, t_wall *wall, t_vector center, float scale)
{
	t_vector	new_orig;
	t_vector	new_dest;
	float		dist_to_middle_x;
	float		dist_to_middle_y;
	t_vector	*out;

	out = ft_memalloc(sizeof(t_vector) * 2);
	dist_to_middle_x = (surface->w / 2) - center.x;
	dist_to_middle_y = (surface->h / 2) - center.y;
	new_orig = wall->orig->pos;
	new_dest = wall->dest->pos;
	new_orig = (t_vector){new_orig.x - center.x, new_orig.y - center.y, 0};
	new_orig = gfx_vector_multiply(new_orig, scale);
	new_orig = (t_vector){new_orig.x + center.x, new_orig.y + center.y, 0};
	new_dest = (t_vector){new_dest.x - center.x, new_dest.y - center.y, 0};
	new_dest = gfx_vector_multiply(new_dest, scale);
	new_dest = (t_vector){new_dest.x + center.x, new_dest.y + center.y, 0};
	new_orig.x += dist_to_middle_x;
	new_orig.y += dist_to_middle_y;
	new_dest.x += dist_to_middle_x;
	new_dest.y += dist_to_middle_y;
	out[0] = new_orig;
	out[1] = new_dest;
	return (out);
}

t_wall	*get_longest_wall_from_list(t_list *list)
{
	t_list	*curr;
	t_wall	*wall;
	t_wall	*long_wall;
	int		temp_dist;
	int		curr_longest;

	curr = list;
	curr_longest = -2147483648;
	long_wall = NULL;
	while (curr)
	{
		wall = curr->content;
		temp_dist = gfx_distance(wall->orig->pos, wall->dest->pos);
		if (temp_dist > curr_longest)
		{
			curr_longest = temp_dist;
			long_wall = wall;
		}
		curr = curr->next;
	}
	return (long_wall);
}
