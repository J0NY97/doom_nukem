#include "better_libui.h"

t_bui_element	*bui_new_menu(t_bui_window *parent, char *name, t_xywh pos)
{
	t_bui_element	*menu;

	menu = ft_memalloc(sizeof(t_bui_element));
	menu->state = ELEMENT_DEFAULT;
	menu->show = 1;
	menu->type = TYPE_MENU;
	menu->id = NULL;
	menu->update = 1;
	menu->children = NULL;
	menu->libui = parent->libui;
	menu->parent = parent;
	menu->position = pos;
	menu->surface[0] = create_surface(pos.w, pos.h);
	menu->surface[1] = create_surface(pos.w, pos.h);
	menu->surface[2] = create_surface(pos.w, pos.h);
	bui_set_element_color(menu, 0xff0000ff);
	menu->active_surface = create_surface(pos.w, pos.h);
	menu->text_color = 0xff000000;
	menu->font_size = 12;
	menu->font_name = ft_strdup(BUI_PATH"DroidSans.ttf");
	menu->font_style = TTF_STYLE_NORMAL;
	bui_set_element_text(menu, name, 0, 0);
	add_to_list(&parent->menus, menu, sizeof(t_bui_element));
	add_to_list(&menu->libui->elements, menu, sizeof(t_bui_element));
	return (menu);
}
