#include "better_libui.h"

t_preset_menu	*bui_new_menu_preset(
		t_bui_window *parent, char *title, t_xywh pos)
{
	t_preset_menu	*preset;

	preset = malloc(sizeof(t_preset_menu));
	memset(preset, 0, sizeof(t_preset_menu));
	preset->menu = bui_new_menu(parent, title, pos);
	preset->quit = bui_new_element(preset->menu, "x",
			(t_xywh){preset->menu->position.w - 20, 0, 20, 20});
	preset->title_bar = bui_new_element(preset->menu, title,
			(t_xywh){0, 0, preset->menu->position.w
			- preset->quit->position.w, preset->quit->position.h});
	return (preset);
}

void	preset_menu_events(t_preset_menu *menu)
{
	if (bui_button(menu->quit))
		menu->menu->show = 0;
	if (menu->menu->libui->mouse_drag
		&& bui_button_allow_duplicate(menu->title_bar))
	{
		menu->menu->position.x += menu->menu->libui->mouse_x
			- menu->menu->libui->last_mouse_x;
		menu->menu->position.x
			= ft_clamp(menu->menu->position.x,
				-menu->title_bar->position.w
				+ menu->title_bar->position.h,
				((t_bui_window *)menu->menu->parent)->position.w
				- menu->title_bar->position.h);
		menu->menu->position.y
			+= menu->menu->libui->mouse_y
			- menu->menu->libui->last_mouse_y;
		menu->menu->position.y
			= ft_clamp(menu->menu->position.y, 0,
				((t_bui_window *)menu->menu->parent)->position.h
				- menu->title_bar->position.h);
	}
}
