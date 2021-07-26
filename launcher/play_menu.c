#include "launcher.h"

void	difficulty_drop_init(t_ui_menu_play *play_menu, t_xywh c)
{
	// Difficulty text elem
	c = ui_init_coords(25, 25, 100, 20);
	play_menu->difficulty_text_elem
		= bui_new_element(play_menu->endless_view, "Difficulty:", c);
	bui_set_element_color(play_menu->difficulty_text_elem,
		play_menu->endless_view->color);
	play_menu->difficulty_text_elem->update_state = 0;
	// Difficulty dropdown
	play_menu->difficulty_dropdown
		= bui_new_dropdown_preset(play_menu->endless_view, "Difficulty",
			(t_xywh){play_menu->difficulty_text_elem->position.x,
			play_menu->difficulty_text_elem->position.y
			+ play_menu->difficulty_text_elem->position.h, 100, 20});
	play_menu->difficulty_dropdown->drop->text_x = 5;
	bui_set_element_image_to_states(play_menu->difficulty_dropdown->arrow,
		ICON_PATH"mc_button.png",
		ICON_PATH"mc_button_hover.png",
		ICON_PATH"mc_button_click.png");
	// Bass elements
	new_bass_element(play_menu->difficulty_dropdown, "easy");
	new_bass_element(play_menu->difficulty_dropdown, "normal");
	play_menu->difficulty_dropdown->active
		= play_menu->difficulty_dropdown->elements->content;
	new_bass_element(play_menu->difficulty_dropdown, "hard");
}

void	endless_tab_init(t_ui_menu_play *play_menu)
{
	t_bui_element	**temp;
	t_xywh			c;

	temp = preset_tab_add(play_menu->play_tabs, "Endless");
	play_menu->endless_button = temp[0];
	play_menu->endless_view = temp[1];
	bui_remove_element_text(play_menu->endless_view);
	bui_set_element_color(play_menu->endless_button, 0xff505168);
	bui_set_element_color(play_menu->endless_view, 0xff505168);
	play_menu->endless_view->update_state = 0;
	free(temp);
	// Difficulty text elem
	c = ui_init_coords(25, 25, 100, 20);
	difficulty_drop_init(play_menu, c);
	// Divider
	c = ui_init_coords(c.x + c.w + c.x, 5, 1,
			play_menu->endless_view->active_surface->h - 10);
	bui_new_element(play_menu->endless_view, NULL, c);
	// Maps view
	c.x = c.x + c.w + 5;
	c.y = 5;
	c.w = play_menu->endless_view->position.w - c.x - 15;
	c.h = play_menu->endless_view->position.h - 10;
	play_menu->endless_maps_view
		= bui_new_element(play_menu->endless_view, NULL, c);
	play_menu->endless_maps_view->update_state = 0;
	bui_set_element_color(play_menu->endless_maps_view, 0xff505168);
	//
	play_menu->endless_maps = NULL;
}

void	story_tab_init(t_ui_menu_play *play_menu)
{
	t_bui_element	**temp;

	temp = preset_tab_add(play_menu->play_tabs, "Story");
	play_menu->story_button = temp[0];
	play_menu->story_view = temp[1];
	bui_remove_element_text(play_menu->story_view);
	bui_set_element_color(play_menu->story_button, 0xffB3C0A4);
	bui_set_element_color(play_menu->story_view, 0xff505168);
	play_menu->story_view->update_state = 0;
	free(temp);
	play_menu->story_maps = NULL;
}

t_ui_menu_play	*play_menu_init(t_bui_element *parent_elem)
{
	t_ui_menu_play	*play_menu;
	t_xywh			c;

	play_menu = ft_memalloc(sizeof(t_ui_menu_play));
	play_menu->menu = new_main_menu_menu(parent_elem, "Play");
	// Play tab
	c.x = play_menu->menu->position.w / 8 / 2;
	c.y = play_menu->menu->position.w / 8 / 2;
	c.w = play_menu->menu->position.w - c.x * 2;
	c.h = play_menu->menu->position.h - c.y * 2;
	play_menu->play_tabs = bui_new_tab_preset(play_menu->menu, "tab", c);
	bui_set_element_color(play_menu->play_tabs->tabsystem, 0x00);
	// Endless
	endless_tab_init(play_menu);
	// Story
	story_tab_init(play_menu);
	return (play_menu);
}

void	map_button(t_map_button_data data)
{
	char	*text;

	text = ft_strdup(data.str);
	ft_strremove(text, ".endless");
	add_to_list(data.list, new_map_button(data.parent, text, data.i, data.cap),
		sizeof(t_bui_element));
	ft_strdel(&text);
}

void	init_play_menu_map_buttons(t_ui_menu_play *menu, t_list *maps)
{
	t_list	*curr;
	int		endless_x;
	int		story_x;

	endless_x = 0;
	story_x = 0;
	curr = maps;
	while (curr)
	{
		if (ft_strendswith(curr->content, ".endless") == 0)
		{
			map_button((t_map_button_data){&menu->endless_maps,
				menu->endless_maps_view, curr->content,
				new_xywh(10, 10, menu->endless_maps_view->position.w,
					menu->endless_maps_view->position.h), endless_x++});
		}
		else if (ft_strendswith(curr->content, ".story") == 0)
		{
			map_button((t_map_button_data){&menu->story_maps, menu->story_view,
				curr->content, new_xywh(30, 30, menu->story_view->position.w,
					menu->story_view->position.h), story_x++});
		}
		curr = curr->next;
	}
}

void	launch_game(
		t_launcher *launcher, t_ui_menu_play *menu,
		t_ui_menu_settings *settings, char *text)
{
	char	*arr[10];

	arr[0] = ft_sprintf("%s", GAME_PATH"doom");
	arr[1] = ft_sprintf("%s%s", ROOT_PATH"map_editor/maps/", text);
	arr[2] = ft_sprintf("-size=%s", settings->resolution_drop->active->text);
	arr[3] = ft_sprintf("-res=%.2f",
			(float)settings->texture_scale_slider->value / 100);
	arr[4] = ft_sprintf("-mouse=%.3fx%.3f",
			(float)settings->mouse_x_slider->value / 1000,
			(float)settings->mouse_y_slider->value / 1000);
	if (ft_strequ(menu->difficulty_dropdown->active->text, "easy"))
		arr[5] = ft_sprintf("-diff=1");
	else if (ft_strequ(menu->difficulty_dropdown->active->text, "hard"))
		arr[5] = ft_sprintf("-diff=3");
	else
		arr[5] = ft_sprintf("-diff=2");
	arr[6] = ft_sprintf("-fov=%d", settings->fov_slider->value);
	arr[7] = ft_strdup("-launcher");
	if (settings->developer_mode_toggle->toggle)
		arr[8] = ft_strdup("-debug");
	else
		arr[8] = ft_strdup("");
	arr[9] = NULL;
	free_launcher(launcher);
	execv(arr[0], arr);
}

char	*check_map_clicked(t_ui_menu_play *menu)
{
	char	*text;

	text = NULL;
	if (menu->endless_view->show)
	{
		preset_dropdown_events(menu->difficulty_dropdown);
		text = get_clicked_map_button(menu->endless_maps);
		if (text)
			ft_stradd(&text, ".endless");
	}
	else if (menu->story_view->show)
	{
		text = get_clicked_map_button(menu->story_maps);
		if (text)
			ft_stradd(&text, ".story");
	}
	return (text);
}

void	play_menu_events(
		t_launcher *launcher, t_ui_menu_play *menu,
		t_ui_menu_settings *settings)
{
	char	*text;

	text = NULL;
	preset_tab_events(menu->play_tabs);
	if (menu->story_button->was_clicked_last_frame)
	{
		bui_set_element_color(menu->endless_button, 0xffb3c0a4);
		bui_set_element_color(menu->story_button, 0xff505168);
	}
	else if (menu->endless_button->was_clicked_last_frame)
	{
		bui_set_element_color(menu->endless_button, 0xff505168);
		bui_set_element_color(menu->story_button, 0xffb3c0a4);
	}
	text = check_map_clicked(menu);
	if (text)
		launch_game(launcher, menu, settings, text);
}
