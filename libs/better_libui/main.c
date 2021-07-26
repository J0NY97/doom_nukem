#include "better_libui.h"

void parse_commands(t_preset_console *console)
{
	if (!console->latest_command)
		return ;
	if (ft_strcmp(console->latest_command, " CLS") == 0)
		bui_change_element_text(console->view, " ");
	ft_strdel(&console->latest_command);
}

typedef struct s_fps
{
	float curr;
	float prev;
	int fps;
	int count;
}		t_fps;

t_fps	*new_fps(void)
{
	return (ft_memalloc(sizeof(t_fps)));
}

void	fps_func(t_fps *fps)
{
	fps->curr = SDL_GetTicks();
	fps->count++;
	if (fps->curr - fps->prev >= 1000)
	{
		fps->prev = fps->curr;
		fps->fps = fps->count;
		fps->count = 0;
		ft_putnbr(fps->fps);
		ft_putchar('\n');
	}
}

void test_program(void)
{
	t_bui_libui *libui;
	t_bui_window *win;
	t_fps *fps = new_fps();

	libui = bui_new_libui();
	win = bui_new_window(libui, "name", (t_xywh) {100, 100, 800, 600}, SDL_WINDOW_RESIZABLE);
	bui_set_window_color(win, 0xff000000);

	t_bui_element *tst_menu = bui_new_menu(win, "menu", (t_xywh){0, 0, 100, 100});

	t_list *el = libui->elements;
	int a = 0;
	while (el)
	{
		ft_printf("elem nr : %d\n", a++);
		el = el->next;	
	}
	exit(0);

	// Preest testes
	t_preset_menu *preset_menu = bui_new_menu_preset(win, "test", (t_xywh) {525, 10, 200, 200});

	// Menu
	t_preset_menu *preset_menu2 = bui_new_menu_preset(win, "menu", (t_xywh) {10, 10, 500, 500});

	t_bui_element *butt = bui_new_element(preset_menu2->menu, "butt", (t_xywh) {10, 10, 50, 20});
	t_bui_element *view = bui_new_element(preset_menu2->menu, "view", (t_xywh) {10, 65, 200, 200});
	t_bui_element *view_butt = bui_new_element(view, "button", (t_xywh) {10, 10, 50, 20});
	bui_set_element_color(view_butt, 0xff06D6A0);

	// Slider
	t_preset_slider *slider_1 = bui_new_slider_preset(preset_menu->menu, (t_xywh) {50, 50, 100, 20});

	// Tabs
	t_bui_element *tab1 = bui_new_element(preset_menu2->menu, "tab1", (t_xywh) {215, 40, 50, 20});
	t_bui_element *tab2 = bui_new_element(preset_menu2->menu, "tab2", (t_xywh) {215 + 50 + 5, 40, 50, 20});
	t_bui_element *tab3 = bui_new_element(preset_menu2->menu, "tab3", (t_xywh) {215 + 50 + 50 + 5 + 5, 40, 50, 20});
	t_bui_element *viewer = bui_new_element(preset_menu2->menu, "viewer", (t_xywh) {215, 60, 200, 200});
	t_bui_element *tab1_view = bui_new_element(viewer, "tab1 view", (t_xywh) {5, 15, 200 - 10, 200 - 20});
	t_bui_element *tab2_view = bui_new_element(viewer, "tab2 view", (t_xywh) {5, 15, 200 - 10, 200 - 20});
	t_bui_element *tab3_view = bui_new_element(viewer, "tab3 view", (t_xywh) {5, 15, 200 - 10, 200 - 20});
	tab2_view->show = 0;
	tab3_view->show = 0;
	bui_set_element_color(tab1, 0xffEF476F);
	bui_set_element_image_from_path(tab1, ELEMENT_ALL, "./sticker.bmp", &(t_xywh) {0, 0, 10, 10});
	bui_set_element_image_from_path(tab1_view, ELEMENT_ALL, "./sticker.bmp", NULL);
	bui_set_element_color(viewer, 0xffEF476F);
	bui_set_element_color(tab2, 0xffe6D6A0);
	bui_set_element_color(tab3, 0xff26547C);
	bui_set_element_text(tab2_view, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum", 40, 0);
	t_bui_element *tab1_view_button = bui_new_element(tab1_view, "button :)", (t_xywh) {10, 10, 50, 20});
	bui_set_element_color(tab1_view_button, tab1->color);

	// Preset tab
	t_preset_tab *tabsystem = bui_new_tab_preset(preset_menu2->menu, "tabsystem",
		(t_xywh) {215, 270, 200, 200});
	t_bui_element **tabulator1 = preset_tab_add(tabsystem, "tab1");
	bui_set_element_color(tabulator1[0], 0xffef476f);
	bui_set_element_color(tabulator1[1], 0xffef476f);
	free(tabulator1);

	t_bui_element **tabulator2 = preset_tab_add(tabsystem, "tab2");
	bui_set_element_color(tabulator2[0], 0xff06d6a0);
	bui_set_element_color(tabulator2[1], 0xff06d6a0);
	free(tabulator2);

	t_bui_element **tabulator3 = preset_tab_add(tabsystem, "tab3");
	bui_set_element_color(tabulator3[0], 0xff26547c);
	bui_set_element_color(tabulator3[1], 0xff26547c);
	free(tabulator3);

	// Slider
	t_preset_slider *slider = bui_new_slider_preset(preset_menu2->menu, (t_xywh) {225, 300, 100, 20});
	slider->button->position.x =
		bui_set_slider_value(0, -100, 50, slider->slider->position.w) - (slider->button->position.w * 0.5f);


	t_bui_element *toggle_button = bui_new_element(preset_menu->menu, "toggle", (t_xywh) {10, 30, 20, 20});

	// This is how you do only text?
	t_bui_element *see_through = bui_new_element(preset_menu2->menu, "see this?", (t_xywh) {10, 300, 50, 20});
	bui_set_element_color(see_through, 0x00000000);
	bui_remove_element_text(see_through);

	// UserInput
	t_bui_element *input = bui_new_element(preset_menu2->menu, "User Input!", (t_xywh) {10, 350, 100, 20});


	// Drop down
	t_bui_element *drop = bui_new_element(preset_menu2->menu, "Drop the bass", (t_xywh) {10, 400, 100, 20});
	t_bui_element *down = bui_new_element(preset_menu2->menu, "bass",
					(t_xywh) {
						drop->position.x,
						drop->position.y + drop->position.h,
						drop->position.w,
						drop->position.h
						});
	down->show = 0;

	// Drop down prefab
	t_preset_dropdown *dropdown = bui_new_dropdown_preset(preset_menu->menu, "default drop text", (t_xywh) {10, 100, 100, 20});
	t_bui_element *bass_butt1 = preset_dropdown_add_element(dropdown, "test1");
	t_bui_element *bass_butt2 = preset_dropdown_add_element(dropdown, "test2");
	t_bui_element *bass_butt3 = preset_dropdown_add_element(dropdown, "test3");

	// Console prest
	t_preset_console *console = bui_new_console_preset(win, (t_xywh) {500, 50, 500, 200});

	// fancy slider
	t_bui_element *fancy_slider = bui_new_element(preset_menu2->menu, " ", (t_xywh) {75, 275, 100, 20});
	bui_set_element_color(fancy_slider, 0xff753525);

	t_bui_element *fancy_button_for_fancy_slider = bui_new_element(fancy_slider, " ", (t_xywh) {-50, 0, 100, 20});
	bui_set_element_color(fancy_button_for_fancy_slider, 0xff106010);

	TTF_Font *d = TTF_OpenFont("DroidSans.ttf", 20);
	SDL_Surface *fancy_slider_text_surface = bui_new_text("70", d, 0x00000000, fancy_slider->position.w);
	TTF_CloseFont(d);

	ft_printf("sizeof t_bui_element : %d\n", sizeof(t_bui_element));
	ft_printf("sizeof t_bui_window : %d\n", sizeof(t_bui_window));
	ft_printf("sizeof t_bui_libui : %d\n", sizeof(t_bui_libui));
	int i = 1;


	t_bui_window *win2;
	win2 = bui_new_window(libui, "name2", (t_xywh) {200, 100, 800, 600}, 0);
	bui_set_window_color(win2, 0xff000000);

	// Testing purposeum
	/*
	SDL_FreeSurface(win2->active_surface);
	win2->active_surface = create_surface(win2->position.w, win2->position.h);
	bui_set_window_flags(win2, BUI_WINDOW_DONT_UPDATE);

	SDL_FreeSurface(win->active_surface);
	win2->active_surface = create_surface(win->position.w, win->position.h);
	bui_set_window_flags(win, BUI_WINDOW_DONT_UPDATE);

	SDL_Renderer *renderer2 = SDL_CreateRenderer(win2->win, -1, SDL_RENDERER_ACCELERATED);
	SDL_Renderer *renderer1 = SDL_CreateRenderer(win->win, -1, SDL_RENDERER_ACCELERATED);
	*/
	// ENd testing
	
	t_bui_element *foi = bui_new_menu(win2, NULL, (t_xywh){100, 100, 100, 20});
//	bui_set_element_flags(foi, ~BUI_ELEMENT_DONT_UPDATE_STATE);

SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
	while(libui->run)
	{
	bui_event_handler_new(libui);
		fps_func(fps);

		if (fps->count == 0)
		{
			bui_set_element_text_font(tab1_view_button, "DroidSans.ttf", i, tab1_view_button->text_color);
			i++;
			i = i > 20 ? 0 : i;
		}

		// Preset events test
		preset_menu_events(preset_menu);
		preset_menu_events(preset_menu2);

		// Console preset events
		preset_console_events(console);
		if (key_pressed(libui, KEY_Z))
			console_log(console, "jonyssimo bonaparte rulez");
		parse_commands(console);

		// Slider preset events
		preset_slider_events(slider);
		preset_slider_events(slider_1);

		// drop down preset events
		preset_dropdown_events(dropdown);

		// tab preset events
		preset_tab_events(tabsystem);

		// user input
		bui_input(input);

		// drop down menu
		if (bui_button_toggle(drop))
		{}
		down->show = drop->toggle;
		if (bui_button(down))
			ft_putstr("wub wub");

		char *temp_text = ft_sprintf("%d\n", SDL_GetTicks());
		bui_change_element_text(tab2_view, temp_text);
		ft_strdel(&temp_text);



		if (bui_button(butt))
			ft_putstr("Hello world!\n");
		if (bui_button(view_butt))
			ft_putstr("butt butt\n");

		if (key_press(libui, "A"))
			ft_printf("MOUSE(%d, %d)\n", libui->mouse_x, libui->mouse_y);
		if (key_press(libui, "B"))
			preset_menu2->menu->show = 0;
		// all menus will be revealed
		if (key_press(libui, "C"))
		{
			t_list *men;

			men = win->menus;
			while (men)
			{
				((t_bui_element *)men->content)->show = 1;
				men = men->next;
			}
		}

		// tab event things
		if (bui_button(tab1))
		{
			bui_set_element_color(viewer, tab1->color);
			tab1_view->show = 1;
			tab2_view->show = 0;
			tab3_view->show = 0;
		}
		if (bui_button(tab2))
		{
			bui_set_element_color(viewer, tab2->color);
			tab1_view->show = 0;
			tab2_view->show = 1;
			tab3_view->show = 0;
		}
		if (bui_button(tab3))
		{
			bui_set_element_color(viewer, tab3->color);
			tab1_view->show = 0;
			tab2_view->show = 0;
			tab3_view->show = 1;
		}
		if (bui_button(tab1_view_button))
			ft_putstr("I like turtles\n");

		// toggle buton test
		if (bui_button_toggle(toggle_button))
			ft_putstr("toggled");


		// Show menu again button
		if (key_press(libui, "E"))
		{
			preset_menu2->menu->position.x = 10;
			preset_menu2->menu->position.y = 10;
		}

		// HERE WE TEST IF WE CAN REMOVE ELEMENTS
		//t_bui_element *temp = bui_new_menu(win, NULL, (t_xywh){0, 0, 100, 100});
		//temp->remove = 1;
		//ft_putstr("[lower main] temp elem freed, starting render.\n");

		//bui_render(win);
		fill_surface(win->active_surface, 0xff000000);
		//bui_render_the_event(libui);
		bui_render_new(libui);

		// Testing purposeum
		/*
		SDL_Texture *win2_texture = SDL_CreateTextureFromSurface(renderer2, win2->active_surface);
		SDL_RenderCopy(renderer2, win2_texture, NULL, NULL);
		SDL_RenderPresent(renderer2);
		SDL_DestroyTexture(win2_texture);

		SDL_Texture *win1_texture = SDL_CreateTextureFromSurface(renderer1, win->active_surface);
		SDL_RenderCopy(renderer1, win1_texture, NULL, NULL);
		SDL_RenderPresent(renderer1);
		SDL_DestroyTexture(win1_texture);
		*/
	}
	free(fps);


	// Preset menu free
	free(preset_menu);
	free(preset_menu2);

	// tabsystem free
	t_list *curr;
	t_list *rruc;
	curr = tabsystem->tabs;
	while (curr)
	{
		rruc = curr->next;
		ft_lstdelone(&curr, &dummy_free_er);
		curr = rruc;
	}
	curr = tabsystem->views;
	while (curr)
	{
		rruc = curr->next;
		ft_lstdelone(&curr, &dummy_free_er);
		curr = rruc;
	}
	free(tabsystem);

	// Slider free
	free(slider_1);
	free(slider);

	// Drop free
	curr = dropdown->elements;
	while (curr)
	{
		rruc = curr->next;
		ft_lstdelone(&curr, &dummy_free_er);
		curr = rruc;
	}
	free(dropdown);

	// Console free
	free(console->menu);
	ft_strdel(&console->latest_command);
	free(console);

	//
	SDL_FreeSurface(fancy_slider_text_surface);

	bui_libui_quit(libui);
	return ;
}

int	main(void)
{
	test_program();
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
//	while (1);
	return (0);
}
