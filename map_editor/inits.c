/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 12:19:16 by jsalmi            #+#    #+#             */
/*   Updated: 2021/07/19 14:51:36 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	window_init(t_editor *editor, t_bui_libui *libui)
{
	t_xywh	coord;

	coord = ui_init_coords(10, 10, 1920, 1080);
	editor->window = bui_new_window(libui, "Doom Editor", coord, 0);
	bui_set_window_color(editor->window, editor->palette.win);
	bui_set_window_icon(editor->window, ROOT_PATH"ui/ui_images/damage.png");
}

void	edit_window_init(t_editor *editor, t_bui_libui *libui)
{
	t_xywh	coord;

	coord = ui_init_coords(500, 500, 1000, 500);
	editor->new_edit_window = bui_new_window(libui, "Editor", coord, 0);
	bui_set_window_color(editor->new_edit_window, editor->palette.win);
	SDL_MinimizeWindow(editor->new_edit_window->win);
	init_sector_editor(editor);
	init_wall_editor(editor);
	init_entity_editor(editor);
}

static void	copy_bxpm_pix_to_surf_pix(t_bxpm *bxpm, SDL_Surface *surface)
{
	int	i;

	i = 0;
	while (i < bxpm->pix_nb)
	{
		((Uint32 *)surface->pixels)[i] = bxpm->clr[bxpm->pix[i]];
		i++;
	}
}

SDL_Surface	*load_bxpm_to_surface(char *bxpm_file)
{
	SDL_Surface	*surface;
	t_bxpm		*bxpm;
	int			result;

	bxpm = ft_memalloc(sizeof(t_bxpm));
	result = read_bxpm(bxpm, bxpm_file);
	if (result != 1)
	{
		ft_printf("[load_bxpm_to_surface] Couldnt open file: %s\n", bxpm_file);
		free(bxpm);
		return (NULL);
	}
	surface = create_surface(bxpm->w, bxpm->h);
	copy_bxpm_pix_to_surf_pix(bxpm, surface);
	free(bxpm->pix);
	free(bxpm->clr);
	free(bxpm);
	return (surface);
}

static void	load_all_textures(t_editor *editor)
{
	int	i;

	i = 0;
	editor->texture_amount = MAP_TEXTURE_AMOUNT;
	editor->texture_textures = ft_memalloc(sizeof(SDL_Surface *)
			* editor->texture_amount);
	while (i < MAP_TEXTURE_AMOUNT)
	{
		editor->texture_textures[g_map_textures[i].id]
			= load_bxpm_to_surface(g_map_textures[i].path);
		i++;
	}
}

SDL_Surface	*yoink_from_surface(SDL_Surface *image, t_xywh coord)
{
	SDL_Surface	*surface;

	surface = create_surface(coord.w, coord.h);
	SDL_BlitSurface(image, &(SDL_Rect){coord.x, coord.y, coord.w, coord.h},
		surface, NULL);
	return (surface);
}

void	grid_init1(t_editor *editor)
{
	t_xywh	coord;

	coord.x = editor->toolbox->position.w + 20;
	coord.y = 10;
	coord.w = editor->window->active_surface->w - coord.x - 10;
	coord.h = editor->window->active_surface->h - 20;
	editor->grid.elem = bui_new_menu(editor->window, "grid", coord);
	bui_set_element_color(editor->grid.elem, editor->palette.win_elem);
	editor->grid.elem->update = 0;
	editor->grid.hover = EMPTY_VEC;
	editor->grid.last_hover = EMPTY_VEC;
	editor->spawn.pos = (t_vector){.x = 0, .y = 0, .z = 0};
	editor->spawn.direction = 0;
	editor->scale = 1;
	editor->grid.font = TTF_OpenFont("DroidSans.ttf", 20);
}

void	grid_init(t_editor *editor)
{
	editor->grid.selected1 = EMPTY_VEC;
	editor->grid.selected2 = EMPTY_VEC;
	editor->grid.modify_wall = NULL;
	editor->grid.modify_point = NULL;
	editor->grid.modify_sector = NULL;
	editor->grid.modify_entity = NULL;
	editor->grid.points = NULL;
	editor->grid.walls = NULL;
	editor->grid.sectors = NULL;
	editor->grid.entities = NULL;
	editor->grid.gap = 10;
	editor->grid.point_amount = 0;
	editor->grid.wall_amount = 0;
	editor->grid.sector_amount = 0;
	editor->grid.entity_amount = 0;
	editor->grid.x = editor->grid.coords.x;
	editor->grid.y = editor->grid.coords.y;
	editor->grid.dimensions = editor->grid.coords;
	load_all_textures(editor);
	init_entity_presets(&editor->entity_presets);
	editor->default_entity = new_entity_preset("default_entity_name");
}

t_bui_element	*new_selection_mode_button(t_bui_element *parent, char *image_path, int i)
{
	t_bui_element	*button;
	t_xywh			coord;
	int				w;
	int				gap;

	w = 32;
	gap = 10;
	coord = new_xywh((i * (w + gap)) + gap, 20, w, w);
	button = bui_new_element(parent, NULL, coord);
	bui_set_element_image_from_path(button, ELEMENT_ALL, image_path, NULL);
	bui_set_element_state_border(button, 2, 0xff0080ff, ELEMENT_HOVER);
	bui_set_element_state_border(button, 2, 0xff0080ff, ELEMENT_CLICK);
	return (button);
}

void	selection_mode_button_init(t_editor *editor)
{
	editor->select_mode_buttons = NULL;
	editor->active_select_mode = NULL;
	editor->button_draw = new_selection_mode_button(editor->select_mode,
		ROOT_PATH"ui/ui_images/draw_mode.png", 0);
	editor->select_mode_vertex = new_selection_mode_button(editor->select_mode,
		ICON_PATH"selection_mode_vertex.png", 1);
	editor->select_mode_wall = new_selection_mode_button(editor->select_mode,
		ICON_PATH"selection_mode_wall.png", 2);
	editor->select_mode_sector = new_selection_mode_button(editor->select_mode,
		ICON_PATH"selection_mode_sector.png", 3);
	editor->select_mode_entity = new_selection_mode_button(editor->select_mode,
		ICON_PATH"selection_mode_entity.png", 4);
	add_to_list(&editor->select_mode_buttons, editor->button_draw,
		sizeof(t_bui_element));
	add_to_list(&editor->select_mode_buttons, editor->select_mode_vertex,
		sizeof(t_bui_element));
	add_to_list(&editor->select_mode_buttons, editor->select_mode_wall,
		sizeof(t_bui_element));
	add_to_list(&editor->select_mode_buttons, editor->select_mode_sector,
		sizeof(t_bui_element));
	add_to_list(&editor->select_mode_buttons, editor->select_mode_entity,
		sizeof(t_bui_element));
}

void	selection_mode_init(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(10, 25, editor->toolbox->position.w - 20, 55);
	editor->select_mode = bui_new_element(editor->toolbox, "Mode", coord);
	editor->select_mode->update_state = 0;
	bui_set_element_text_font(editor->select_mode, "DroidSans.ttf",
		editor->select_mode->font_size, 0xffffffff);
	bui_set_element_color(editor->select_mode, editor->palette.elem_elem);
	selection_mode_button_init(editor);
}

void	toolbox_init(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(10, 10, editor->window->active_surface->w / 7,
			editor->window->active_surface->h - 20);
	editor->toolbox = bui_new_menu(editor->window, "toolbox", coord);
	editor->toolbox->update_state = 0;
	bui_set_element_color(editor->toolbox, editor->palette.win_elem);
	selection_mode_init(editor);
	button_init(editor);
	coord = new_xywh(10, editor->other_mode->position.y
			+ editor->other_mode->position.h + 10, editor->toolbox->position.w
			- 20, 300);
	info_area_init(editor, coord);
}

void	init_info_box(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(10, editor->info_area->position.h - 40 - 10,
			editor->info_area->position.w - 20, 40);
	editor->info_box = bui_new_element(editor->info_area, NULL, coord);
	editor->info_box->update_state = 0;
	editor->info_box->text_x = 2;
	editor->info_box->text_color = 0xffffffff;
	bui_set_element_color(editor->info_box, editor->info_area->color);
	bui_set_element_border(editor->info_box, 1, editor->palette.light_gray);
}

void	button_init_info_area(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(editor->info_area->position.w - 110, 25, 32, 32);
	editor->button_remove = bui_new_element(editor->info_area, NULL, coord);
	bui_set_element_image_to_states(editor->button_remove,
		ROOT_PATH"ui/ui_images/remove_button.png",
		ROOT_PATH"ui/ui_images/remove_button_click.png",
		ROOT_PATH"ui/ui_images/remove_button_click.png");
	coord = ui_init_coords(editor->info_area->position.w
			- 110 + 32 + 10, 25, 32, 32);
	editor->button_edit = bui_new_element(editor->info_area, NULL, coord);
	bui_set_element_image_to_states(editor->button_edit,
		ROOT_PATH"ui/ui_images/edit_button.png",
		ROOT_PATH"ui/ui_images/edit_button_click.png",
		ROOT_PATH"ui/ui_images/edit_button_click.png");
}

void	general_info_init(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(10, editor->selected_vector_info->position.y
			+ editor->selected_vector_info->position.h + 5,
			editor->info_area->position.w - 20, 40);
	editor->general_info = bui_new_element(editor->info_area, "Info", coord);
	bui_set_element_color(editor->general_info, editor->info_area->color);
	bui_set_element_text_color(editor->general_info, 0xffffffff);
}

void	info_area_init(t_editor *editor, t_xywh c)
{
	t_xywh	coord;

	editor->info_area = bui_new_element(editor->toolbox, NULL, c);
	editor->info_area->update_state = 0;
	bui_set_element_color(editor->info_area, editor->palette.elem_elem);
	coord = ui_init_coords(10, 20, 100, 50);
	editor->hover_info = bui_new_element(editor->info_area, NULL, coord);
	bui_set_element_color(editor->hover_info, editor->info_area->color);
	coord = ui_init_coords(10, 75, 100, 50);
	editor->selected_sector_info
		= bui_new_element(editor->info_area, NULL, coord);
	editor->selected_sector_info->text_color = 0xffffffff;
	bui_set_element_color(editor->selected_sector_info,
		editor->info_area->color);
	coord = ui_init_coords(10, 130, 100, 55);
	editor->selected_vector_info
		= bui_new_element(editor->info_area, NULL, coord);
	bui_set_element_color(editor->selected_vector_info,
		editor->info_area->color);
	coord = ui_init_coords(editor->info_area->position.w - 110, 100, 100, 40);
	editor->scaler = new_changer_prefab(editor->info_area, "Map Scale", coord);
	init_info_box(editor);
	general_info_init(editor);
	button_init_info_area(editor);
}

t_bui_element *
	new_map_type_tickbox(t_bui_element *parent, char *text, t_xywh coord)
{
	t_bui_element	*tick;
	t_xywh			text_coord;

	text_coord = coord;
	text_coord.w = 50;
	bui_new_element(parent, text, text_coord);
	coord.x += text_coord.w;
	tick = bui_new_element(parent, NULL, coord);
	bui_set_element_image_to_states(tick,
		ROOT_PATH"ui/ui_images/tick_box_off.png",
		ROOT_PATH"ui/ui_images/tick_box_hover.png",
		ROOT_PATH"ui/ui_images/tick_box_on.png");
	return (tick);
}

void	map_type_tickbox_init(int gap, t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(editor->button_save->position.x
			+ editor->button_save->position.w + gap,
			editor->button_save->position.y - gap, 20, 20);
	editor->endless_tickbox = new_map_type_tickbox(editor->other_mode,
			"endless", coord);
	coord = ui_init_coords(editor->button_save->position.x
			+ editor->button_save->position.w + gap,
			editor->button_save->position.y + gap, 20, 20);
	editor->story_tickbox = new_map_type_tickbox(editor->other_mode,
			"story", coord);
	if (ft_strendswith(editor->fullpath, ".story") == 0)
		editor->active_map_type = editor->story_tickbox;
	else
		editor->active_map_type = editor->endless_tickbox;
	add_to_list(&editor->map_type_tickboxes,
		editor->endless_tickbox, sizeof(t_bui_element));
	add_to_list(&editor->map_type_tickboxes,
		editor->story_tickbox, sizeof(t_bui_element));
}

void	button_init_other(int button_w, int gap, t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(gap, 20, 100, 20);
	editor->map_name_input
		= bui_new_element(editor->other_mode, editor->mapname, coord);
	editor->map_name_input->text_x = 5;
	coord = ui_init_coords(editor->map_name_input->position.x
			+ editor->map_name_input->position.w + gap, 20,
			button_w, button_w);
	editor->button_save = bui_new_element(editor->other_mode,
			"save", coord);
	editor->button_save->text_y = -20;
	bui_set_element_image_to_states(editor->button_save,
		ROOT_PATH"ui/ui_images/save_button.png",
		ROOT_PATH"ui/ui_images/save_button_click.png",
		ROOT_PATH"ui/ui_images/save_button_click.png");
	map_type_tickbox_init(gap, editor);
}

void	button_init(t_editor *editor)
{
	t_xywh	coord;
	int		gap;
	int		button_w;

	gap = 10;
	button_w = 32;
	coord = ui_init_coords(10, 25, 50, 55);
	coord = ui_init_coords(editor->select_mode->position.x,
			editor->select_mode->position.y
			+ editor->select_mode->position.h + 10,
			editor->toolbox->position.w - 20, 55);
	editor->other_mode = bui_new_element(editor->toolbox, "Other", coord);
	editor->other_mode->update_state = 0;
	bui_set_element_text_font(editor->other_mode, "DroidSans.ttf",
		editor->other_mode->font_size, 0xffffffff);
	bui_set_element_color(editor->other_mode, editor->palette.elem_elem);
	button_init_other(button_w, gap, editor);
}

void	color_palette_init(t_color_palette *pal)
{
	pal->win = 0xff0e1111;
	pal->win_elem = 0xff232b2b;
	pal->elem_elem = 0xff353839;
	pal->elem_elem_elem = 0xffa6a6a4;
	pal->light_gray = 0xffa6a6a4;
	pal->granny_smith_apple = 0xffadf7b6;
	pal->peach_crayola = 0xffffc09f;
	pal->light_blue = 0xffa0ced9;
}

t_changer_prefab *
	new_changer_prefab(t_bui_element *parent_menu, char *title, t_xywh coord)
{
	t_changer_prefab	*prefab;
	t_xywh				temp_coord;

	prefab = ft_memalloc(sizeof(t_changer_prefab));
	prefab->menu = bui_new_element(parent_menu, title, coord);
	bui_set_element_flags(prefab->menu, BUI_ELEMENT_DONT_UPDATE_STATE);
	temp_coord = ui_init_coords(0, coord.h - 20, 20, 20);
	prefab->sub_button = bui_new_element(prefab->menu, "-", temp_coord);
	temp_coord = ui_init_coords(coord.w - 20, coord.h - 20, 20, 20);
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
	bui_change_element_text(changer->value, str);
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
	bui_change_element_text(changer->value, str);
	ft_strdel(&str);
}

void	new_texture_button(
			t_bui_element *parent, t_list **list, SDL_Surface *texture, int i)
{
	t_bui_element	*temp_elem;
	t_xywh			coord;
	int				button_gap;
	int				amount_on_x;
	char			*str;

	button_gap = 20;
	amount_on_x = floor(parent->position.w / (50 + button_gap + 20));
	coord = ui_init_coords(0, 0, 50, 50);
	coord.x = (i % (amount_on_x + 1)) * (coord.w + button_gap) + 20;
	coord.y = (i / (amount_on_x + 1)) * (coord.h + button_gap) + 50;
	str = ft_sprintf("%d", i);
	temp_elem = bui_new_element(parent, str, coord);
	ft_strdel(&str);
	SDL_BlitScaled(texture, NULL, temp_elem->surface[0], NULL);
	SDL_BlitScaled(texture, NULL, temp_elem->surface[1], NULL);
	SDL_BlitScaled(texture, NULL, temp_elem->surface[2], NULL);
	bui_set_element_state_border(temp_elem, 2, 0xff00ff00, ELEMENT_HOVER);
	bui_set_element_state_border(temp_elem, 2, 0xff00ffff, ELEMENT_CLICK);
	add_to_list(list, temp_elem, sizeof(t_bui_element));
}

void	texture_buttons_init(t_editor *editor)
{
	int	i;

	i = 0;
	editor->sector_texture_buttons = NULL;
	while (i < editor->texture_amount)
	{
		new_texture_button(editor->sector_texture_menu,
			&editor->sector_texture_buttons,
			editor->texture_textures[i], i);
		i++;
	}
}

void	floor_and_ceiling_texture_menu_init(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(editor->edit_toolbox_sector->position.x
			+ editor->edit_toolbox_sector->position.w + 5, 5,
			(editor->new_edit_window->position.w
				- editor->edit_toolbox_sector->position.w - 15) * 0.5,
			editor->new_edit_window->position.h - 10);
	editor->sector_texture_menu = bui_new_menu(editor->new_edit_window,
			"Sector Texture Menu", coord);
	bui_set_element_color(editor->sector_texture_menu,
		editor->palette.elem_elem_elem);
	texture_buttons_init(editor);
	coord = ui_init_coords(editor->sector_texture_menu->position.w - 100,
			0, 50, 20);
	editor->floor_texture_title
		= bui_new_element(editor->sector_texture_menu, "Floor", coord);
	bui_set_element_color(editor->floor_texture_title, 0xff0000ff);
	coord = ui_init_coords(editor->sector_texture_menu->position.w - 50,
			0, 50, 20);
	editor->ceiling_texture_title
		= bui_new_element(editor->sector_texture_menu, "Ceiling", coord);
	bui_set_element_color(editor->ceiling_texture_title, 0xff00ff00);
}

void	sector_edit_changer_inits(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(5, (25 * 1) + (40 * 0), 100, 40);
	editor->floor_height = new_changer_prefab(editor->edit_toolbox_sector,
			"floor height", coord);
	coord.y = (25 * 2) + (40 * 1);
	editor->ceiling_height = new_changer_prefab(editor->edit_toolbox_sector,
			"ceiling height", coord);
	coord.y = (25 * 3) + (40 * 2);
	editor->gravity = new_changer_prefab(editor->edit_toolbox_sector,
			"gravity", coord);
	coord.y = (25 * 4) + (40 * 3);
	editor->lighting = new_changer_prefab(editor->edit_toolbox_sector,
			"lighting", coord);
	coord.y = (25 * 6) + (40 * 5);
	editor->floor_scale = new_changer_prefab(editor->edit_toolbox_sector,
			"floor texture scale", coord);
	coord.y = (25 * 7) + (40 * 6);
	editor->ceiling_scale = new_changer_prefab(editor->edit_toolbox_sector,
			"ceiling texture scale", coord);
}

void	sector_slope_floor_changer_init(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(10, editor->slope_sector->position.y
			+ editor->slope_sector->position.h + 10, 100, 20);
	editor->slope_floor_title = bui_new_element(editor->slope_edit_menu,
			"Floor", coord);
	bui_set_element_color(editor->slope_floor_title, 0xff0000ff);
	coord = ui_init_coords(editor->slope_floor_title->position.x,
			editor->slope_floor_title->position.y
			+ editor->slope_floor_title->position.h + 10, 100, 40);
	editor->slope_floor_wall_changer
		= new_changer_prefab(editor->slope_edit_menu, "Wall ID", coord);
	bui_change_element_text(editor->slope_floor_wall_changer->sub_button, "<");
	bui_change_element_text(editor->slope_floor_wall_changer->add_button, ">");
	coord = ui_init_coords(editor->slope_floor_wall_changer->menu->position.x
			+ editor->slope_floor_wall_changer->menu->position.w + 10,
			editor->slope_floor_wall_changer->menu->position.y, 100, 40);
	editor->slope_floor_angle_changer
		= new_changer_prefab(editor->slope_edit_menu, "Slope Angle", coord);
}

void	sector_slope_ceiling_changer_init(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(10,
			editor->slope_floor_wall_changer->menu->position.y
			+ editor->slope_floor_wall_changer->menu->position.h + 10, 100, 20);
	editor->slope_ceiling_title = bui_new_element(editor->slope_edit_menu,
			"Ceiling", coord);
	bui_set_element_color(editor->slope_ceiling_title, 0xff00ff00);
	coord = ui_init_coords(editor->slope_ceiling_title->position.x,
			editor->slope_ceiling_title->position.y
			+ editor->slope_ceiling_title->position.h + 10, 100, 40);
	editor->slope_ceiling_wall_changer
		= new_changer_prefab(editor->slope_edit_menu, "Wall ID", coord);
	bui_change_element_text(editor->slope_ceiling_wall_changer->sub_button,
		"<");
	bui_change_element_text(editor->slope_ceiling_wall_changer->add_button,
		">");
	coord = ui_init_coords(editor->slope_ceiling_wall_changer->menu->position.x
			+ editor->slope_ceiling_wall_changer->menu->position.w + 10,
			editor->slope_ceiling_wall_changer->menu->position.y, 100, 40);
	editor->slope_ceiling_angle_changer
		= new_changer_prefab(editor->slope_edit_menu, "Slope Angle", coord);
}

void	sector_slope_edit_menu_init(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(editor->sector_texture_menu->position.x
			+ editor->sector_texture_menu->position.w + 5, 5,
			editor->new_edit_window->position.w
			- editor->sector_texture_menu->position.w
			- editor->edit_toolbox_sector->position.w - 20,
			editor->new_edit_window->position.h - 10);
	editor->slope_edit_menu = bui_new_menu(editor->new_edit_window,
			"Slope Edit", coord);
	bui_set_element_color(editor->slope_edit_menu,
		editor->palette.elem_elem_elem);
	coord = ui_init_coords(10, 20, editor->slope_edit_menu->position.w - 20,
			editor->slope_edit_menu->position.h * 0.5);
	editor->slope_sector = bui_new_element(editor->slope_edit_menu,
			"Sector", coord);
	bui_set_element_flags(editor->slope_sector, BUI_ELEMENT_DONT_UPDATE_STATE);
	sector_slope_floor_changer_init(editor);
	sector_slope_ceiling_changer_init(editor);
}

void	init_sector_editor(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(5, 5, editor->new_edit_window->position.w * 0.20f,
			editor->new_edit_window->position.h - 10);
	editor->edit_toolbox_sector = bui_new_menu(editor->new_edit_window,
			"New Toolbox", coord);
	bui_set_element_color(editor->edit_toolbox_sector,
		editor->palette.elem_elem_elem);
	floor_and_ceiling_texture_menu_init(editor);
	sector_edit_changer_inits(editor);
	sector_slope_edit_menu_init(editor);
}

void	new_wall_texture_button(
		t_bui_element *parent, t_list **list, SDL_Surface *texture, int i)
{
	t_bui_element	*temp_elem;
	t_xywh			coord;
	char			*str;
	int				button_gap;
	int				amount_on_x;

	button_gap = 15;
	amount_on_x = floor(parent->position.w / (50 + button_gap + 5));
	coord = ui_init_coords(0, 0, 50, 50);
	coord.x = (i % (amount_on_x + 1)) * (coord.w + button_gap) + 5;
	coord.y = (i / (amount_on_x + 1)) * (coord.h + button_gap) + 70;
	str = ft_itoa(i);
	temp_elem = bui_new_element(parent, str, coord);
	ft_strdel(&str);
	SDL_BlitScaled(texture, NULL, temp_elem->surface[0], NULL);
	SDL_BlitScaled(texture, NULL, temp_elem->surface[1], NULL);
	SDL_BlitScaled(texture, NULL, temp_elem->surface[2], NULL);
	draw_rect_border(temp_elem->surface[ELEMENT_CLICK],
		(t_xywh){0, 0, temp_elem->position.w, temp_elem->position.h},
		0xff00ff00, 5);
	draw_rect_border(temp_elem->surface[ELEMENT_HOVER],
		(t_xywh){0, 0, temp_elem->position.w, temp_elem->position.h},
		0xff0000ff, 5);
	add_to_list(list, temp_elem, sizeof(t_bui_element));
}

void	portal_texture_tab_init(t_editor *editor)
{
	t_bui_element	**elems;

	elems = preset_tab_add(editor->wall_tab, "Wall Texture");
	bui_set_element_color(elems[0], editor->palette.light_blue);
	bui_set_element_color(elems[1], editor->palette.light_blue);
	editor->wall_texture_view = elems[1];
	bui_set_element_flags(editor->wall_texture_view,
		BUI_ELEMENT_DONT_UPDATE_STATE);
	free(elems);
}

void	wall_texture_tab_init(t_editor *editor)
{
	t_bui_element	**elems;

	elems = preset_tab_add(editor->wall_tab, "Portal Texture");
	bui_set_element_color(elems[0], editor->palette.granny_smith_apple);
	bui_set_element_color(elems[1], editor->palette.granny_smith_apple);
	editor->portal_texture_view = elems[1];
	bui_set_element_flags(editor->portal_texture_view,
		BUI_ELEMENT_DONT_UPDATE_STATE);
	free(elems);
}

void	wall_sprite_texture_tab_init(t_editor *editor)
{
	t_bui_element	**elems;

	elems = preset_tab_add(editor->wall_tab, "Wall Sprite");
	bui_set_element_color(elems[0], editor->palette.peach_crayola);
	bui_set_element_color(elems[1], editor->palette.peach_crayola);
	editor->wall_sprite_view = elems[1];
	bui_set_element_flags(editor->wall_sprite_view,
		BUI_ELEMENT_DONT_UPDATE_STATE);
	free(elems);
}

void	tabsystem_wall_editor_init(t_editor *editor)
{
	t_xywh			coord;

	coord = ui_init_coords(5, 20,
			editor->edit_toolbox_wall->position.w - 10,
			editor->edit_toolbox_wall->position.h - 25);
	editor->wall_tab = bui_new_tab_preset(editor->edit_toolbox_wall,
			"texture tabs", coord);
	bui_set_element_color(editor->wall_tab->tabsystem,
		((t_bui_element *)editor->wall_tab->tabsystem->parent)->color);
	editor->wall_tab->tabsystem->update_state = 0;
	portal_texture_tab_init(editor);
	wall_texture_tab_init(editor);
	wall_sprite_texture_tab_init(editor);
}

void	wall_tab_init(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(5, 20, 80, 40);
	editor->texture_scale_changer
		= new_changer_prefab(editor->wall_texture_view, "texture scale", coord);
	bui_set_element_color(editor->texture_scale_changer->menu, 0xff06D6A0);
	bui_set_element_color(editor->texture_scale_changer->sub_button,
		0xff06D6A0);
	bui_set_element_color(editor->texture_scale_changer->value, 0xff06D6A0);
	bui_set_element_color(editor->texture_scale_changer->add_button,
		0xff06D6A0);
	coord = ui_init_coords(115, 20, 100, 20);
	editor->wall_solid = bui_new_element(editor->wall_texture_view,
			"Solid:", coord);
	bui_set_element_color(editor->wall_solid,
		((t_bui_element *)editor->wall_solid->parent)->color);
	coord = ui_init_coords(40, 0, 20, 20);
	editor->wall_solid_tick = bui_new_element(editor->wall_solid, NULL, coord);
	bui_set_element_image_to_states(editor->wall_solid_tick,
		ROOT_PATH"ui/ui_images/tick_box_off.png",
		ROOT_PATH"ui/ui_images/tick_box_hover.png",
		ROOT_PATH"ui/ui_images/tick_box_on.png");
}

void	portal_tab_init(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(115, 40, 100, 20);
	editor->wall_portal = bui_new_element(editor->wall_texture_view,
			"Portal:", coord);
	bui_set_element_color(editor->wall_portal,
		((t_bui_element *)editor->wall_portal->parent)->color);
	coord = ui_init_coords(40, 0, 20, 20);
	editor->wall_portal_tick = bui_new_element(editor->wall_portal,
			NULL, coord);
	bui_set_element_image_to_states(editor->wall_portal_tick,
		ROOT_PATH"ui/ui_images/tick_box_off.png",
		ROOT_PATH"ui/ui_images/tick_box_hover.png",
		ROOT_PATH"ui/ui_images/tick_box_on.png");
}

void	wall_sprite_tab_init(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(95, 20, 80, 20);
	editor->add_wall_sprite_button = bui_new_element(editor->wall_sprite_view,
			"add sprite", coord);
	bui_set_element_color(editor->add_wall_sprite_button, 0xff06D6A0);
	coord = ui_init_coords(95, 40, 80, 20);
	editor->remove_wall_sprite_button
		= bui_new_element(editor->wall_sprite_view, "remove sprite", coord);
	bui_set_element_color(editor->remove_wall_sprite_button, 0xff06D6A0);
	coord = ui_init_coords(5, 20, 80, 40);
	editor->sprite_scale_changer = new_changer_prefab(editor->wall_sprite_view,
			"sprite scale", coord);
	bui_set_element_color(editor->sprite_scale_changer->menu, 0xff06D6A0);
	bui_set_element_color(editor->sprite_scale_changer->sub_button, 0xff06D6A0);
	bui_set_element_color(editor->sprite_scale_changer->value, 0xff06D6A0);
	bui_set_element_color(editor->sprite_scale_changer->add_button, 0xff06D6A0);
	coord = ui_init_coords(0, 0, 50, 20);
	editor->sprite_changer = new_changer_prefab(editor->wall_sprite_view,
			"Wall Sprite ID", coord);
	editor->selected_sprite = 0;
}

void	wall_texture_buttons_init(t_editor *editor)
{
	int	i;

	editor->wall_texture_buttons = NULL;
	editor->active_wall_texture = NULL;
	i = 0;
	while (i < editor->texture_amount)
	{
		new_wall_texture_button(editor->wall_texture_view,
			&editor->wall_texture_buttons, editor->texture_textures[i], i);
		i++;
	}
}

void	portal_texture_buttons_init(t_editor *editor)
{
	int	i;

	editor->portal_texture_buttons = NULL;
	editor->active_portal_texture = NULL;
	i = 0;
	while (i < editor->texture_amount)
	{
		new_wall_texture_button(editor->portal_texture_view,
			&editor->portal_texture_buttons, editor->texture_textures[i], i);
		i++;
	}
}

void	wall_sprite_texture_buttons_init(t_editor *editor)
{
	int	i;

	editor->wall_sprite_buttons = NULL;
	editor->active_wall_sprite = NULL;
	i = 0;
	while (i < editor->texture_amount)
	{
		new_wall_texture_button(editor->wall_sprite_view,
			&editor->wall_sprite_buttons, editor->texture_textures[i], i);
		i++;
	}
}

void	init_wall_editor(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(5, 5, editor->new_edit_window->position.w * 0.20f,
			editor->new_edit_window->position.h - 10);
	editor->edit_toolbox_wall = bui_new_menu(editor->new_edit_window,
			"New Toolbox", coord);
	coord = ui_init_coords(editor->edit_toolbox_sector->position.x
			+ editor->edit_toolbox_sector->position.w + 5, 5,
			editor->new_edit_window->position.w
			- editor->edit_toolbox_sector->position.w - 15,
			editor->new_edit_window->position.h - 10);
	editor->edit_view_wall = bui_new_menu(editor->new_edit_window,
			"New View", coord);
	editor->edit_view_wall->update = 0;
	tabsystem_wall_editor_init(editor);
	wall_tab_init(editor);
	wall_texture_buttons_init(editor);
	portal_tab_init(editor);
	portal_texture_buttons_init(editor);
	wall_sprite_tab_init(editor);
	wall_sprite_texture_buttons_init(editor);
}

void	new_radio_button(
		t_list **list, t_bui_element *parent, t_xywh c, char *str)
{
	t_xywh			coord;
	t_bui_element	*radio;

	coord = ui_init_coords(c.x, c.y, 15, 15);
	radio = bui_new_element(parent, str, coord);
	radio->text_y = -100;
	bui_set_element_color(radio, 0x00);
	bui_set_element_image_to_states(radio,
		ROOT_PATH"ui/ui_images/radio_button_off.png",
		ROOT_PATH"ui/ui_images/radio_button_hover.png",
		ROOT_PATH"ui/ui_images/radio_button_on.png");
	add_to_list(list, radio, sizeof(t_bui_element));
}

void	init_entity_presets(t_list **list)
{
	int				i;
	int const		*tc;
	SDL_Surface		*image;
	t_entity_preset	*preset;

	i = 0;
	while (i < ENTITY_AMOUNT)
	{
		preset = new_entity_preset(g_entity_data[i].name);
		preset->mood = ENTITY_TYPE_HOSTILE;
		if (!g_entity_data[i].hostile)
			preset->mood = ENTITY_TYPE_NEUTRAL;
		image = load_bxpm_to_surface(g_entity_data[i].path);
		tc = g_entity_data[i].tc;
		preset->texture = yoink_from_surface(image,
				(t_xywh){tc[0], tc[1], tc[2], tc[3]});
		add_to_list(list, preset, sizeof(t_entity_preset));
		SDL_FreeSurface(image);
		i++;
	}	
	ft_printf("[init_entity_presets]\n");
}

void	add_to_entity_editor(t_editor *editor)
{
	t_xywh	coord;
	t_list	*curr;

	curr = editor->entity_presets;
	while (curr)
	{
		preset_dropdown_add_element(editor->entity_type_drop,
			((t_entity_preset *)curr->content)->name);
		curr = curr->next;
	}
	coord = ui_init_coords(editor->edit_toolbox_entity->position.w * 0.5f - 50,
			editor->edit_toolbox_entity->position.h * 0.5f, 100, 100);
	coord.x -= 7;
	new_radio_button(&editor->entity_direction_radio_buttons,
		editor->edit_entity_direction, (t_xywh){coord.x, 25, 0, 0}, "270");
	new_radio_button(&editor->entity_direction_radio_buttons,
		editor->edit_entity_direction, (t_xywh){coord.x + 25, 25 + 25, 0, 0},
		"0");
	new_radio_button(&editor->entity_direction_radio_buttons,
		editor->edit_entity_direction, (t_xywh){coord.x, 25 + 50, 0, 0}, "90");
	new_radio_button(&editor->entity_direction_radio_buttons,
		editor->edit_entity_direction, (t_xywh){coord.x - 25, 25 + 25, 0, 0},
		"180");
}

void	init_entity_editor2(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(5, 20,
			editor->edit_toolbox_entity->position.w - 10, 20);
	editor->entity_type_drop = bui_new_dropdown_preset(
			editor->edit_toolbox_entity, "Entity types", coord);
	coord = ui_init_coords(editor->edit_toolbox_entity->position.w * 0.5f
			- 50, editor->edit_toolbox_entity->position.h * 0.5f, 100, 100);
	editor->edit_entity_direction = bui_new_element(
			editor->edit_toolbox_entity, "Direction", coord);
	bui_set_element_color(editor->edit_entity_direction,
		editor->palette.elem_elem);
	bui_set_element_text_color(editor->edit_entity_direction, 0xffffffff);
	editor->edit_entity_direction->update_state = 0;
}

void	init_entity_editor(t_editor *editor)
{
	t_xywh	coord;

	coord = ui_init_coords(5, 5, editor->new_edit_window->position.w
			* 0.20f, editor->new_edit_window->position.h - 10);
	editor->edit_toolbox_entity = bui_new_menu(editor->new_edit_window,
			"Entity Toolbox", coord);
	bui_set_element_color(editor->edit_toolbox_entity,
		editor->palette.elem_elem_elem);
	coord = ui_init_coords(editor->edit_toolbox_sector->position.x
			+ editor->edit_toolbox_sector->position.w + 5, 5,
			editor->new_edit_window->position.w
			- editor->edit_toolbox_sector->position.w - 15,
			editor->new_edit_window->position.h - 10);
	editor->edit_view_entity = bui_new_menu(editor->new_edit_window,
			"Entity View", coord);
	bui_set_element_color(editor->edit_view_entity,
		editor->palette.elem_elem_elem);
	init_entity_editor2(editor);
	add_to_entity_editor(editor);
}
