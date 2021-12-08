#include "editor.h"

/*
 * Description: Aligns element with position of 'size' inside 'inside'
 * 				with 'gap' amount of pixels inbetween each element;
 *
 * NOTE: this function ignores y/h;
 * 		aka it assumes you have infinite height on 'inside';
 *
 * inside == inside which w and h the output vec2 should fit in;
 * inside.x == width;
 * inside.y == height;
 *
 * size == of the element you want to fit inside 'inside';
*/
t_vec2	align_fill(t_vec2 inside, t_vec4 size, int gap, int i)
{
	int	amount_x;

	amount_x = (inside.x - size.x) / (size.w + gap);
	while ((int)amount_x * (size.w + gap) < inside.x - size.x)
		gap++;
	return (vec2(size.x + (i % (int)amount_x * (size.w + gap)),
			size.y + (i / (int)amount_x * (size.h + gap))));
}

Uint32	random_blue_color(void)
{
	Uint32	color;

	color = rgba_to_hex((t_rgba){
			.a = 255, .r = rand() % 255,
			.g = rand() % 255, .b = 245});
	return (color);
}

t_vec2i	conversion(t_editor *editor, t_vec2i v)
{
	t_vec2i	result;

	result.x = (v.x - editor->offset.x) * (editor->gap_size * editor->zoom);
	result.y = (v.y - editor->offset.y) * (editor->gap_size * editor->zoom);
	return (result);
}

/*
 * NOTE: selection_drop_menu closes when you click something,
 * 		so thats why we have to see if it was last frame clicked;
*/
int	hover_over_open_menus(t_editor *editor)
{
	if (ui_element_is_hover(editor->sector_edit_menu)
		|| ui_element_is_hover(editor->menu_wall_edit)
		|| ui_element_is_hover(editor->sprite_edit_menu)
		|| ui_element_is_hover(editor->texture_menu)
		|| ui_element_is_hover(editor->event_edit_menu)
		|| ui_element_is_hover(editor->spawn_edit_menu)
		|| ui_element_was_hover(editor->selection_dropdown_menu)
		|| ui_element_is_hover(editor->menu_toolbox_top))
		return (1);
	return (0);
}

float	get_aspect(float w, float h)
{
	return (w / h);
}

/*
 * Font should already be opened before this function call;
*/
void	draw_text(
		SDL_Surface *surface, char *text,
		TTF_Font *font, t_vec2i pos, Uint32 color)
{
	SDL_Surface	*text_surface;
	t_rgba		rgba;

	if (font)
	{
		rgba = hex_to_rgba(color);
		TTF_SetFontHinting(font, TTF_HINTING_MONO);
		text_surface = TTF_RenderText_Blended(font, text,
				(SDL_Color){rgba.r, rgba.g, rgba.b, rgba.a});
		SDL_BlitSurface(text_surface, NULL, surface,
			&(SDL_Rect){pos.x - (text_surface->w / 2),
			pos.y - (text_surface->h / 2),
			text_surface->w, text_surface->h});
		SDL_FreeSurface(text_surface);
	}
	else
		ft_printf("[%s] Error : \"%s\" no font.\n", __FUNCTION__, text);
}

void	set_elem_parent_and_recipe(
		t_ui_element *elem, int ui_type, t_ui_element *parent, char *recipe_id)
{
	g_acceptable[ui_type].maker(parent->win, elem);
	ui_element_set_parent(elem, parent, UI_TYPE_ELEMENT);
	ui_element_edit(elem, ui_layout_get_recipe(parent->win->layout, recipe_id));
}

void	draw_arrow(SDL_Surface *surface, t_vec2i start, int len, float yaw)
{
	float	angle;
	t_vec2i	p1;
	t_vec2i	p2;

	angle = yaw * (M_PI / 180);
	p1 = start;
	p2 = vec2i(cos(angle) * (len * 2) + p1.x, sin(angle) * (len * 2) + p1.y);
	ui_surface_line_draw(surface, p1, p2, 0xffaaab5d);
	angle = (yaw - 45) * (M_PI / 180);
	p1 = vec2i(cos(angle) * -len + p2.x, sin(angle) * -len + p2.y);
	ui_surface_line_draw(surface, p2, p1, 0xffaaab5d);
	angle = (yaw + 45) * (M_PI / 180);
	p1 = vec2i(cos(angle) * -len + p2.x, sin(angle) * -len + p2.y);
	ui_surface_line_draw(surface, p2, p1, 0xffaaab5d);
}

t_vec2i	get_middle(t_vec2i p1, t_vec2i p2)
{
	return (vec2i((p1.x + p2.x) / 2, (p1.y + p2.y) / 2));
}
