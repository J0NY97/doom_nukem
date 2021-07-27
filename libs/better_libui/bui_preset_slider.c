#include "better_libui.h"

t_preset_slider	*bui_new_slider_preset(t_bui_element *menu, t_xywh pos)
{
	t_preset_slider	*slider;

	slider = ft_memalloc(sizeof(t_preset_slider));
	slider->slider = bui_new_element(menu, " ", pos);
	slider->button = bui_new_element(slider->slider, " ",
			(t_xywh){0, 0, 20, 20});
	bui_set_element_color(slider->button, 0xffef476f);
	bui_set_slider_values(slider, 50, 0, 100);
	return (slider);
}

void	bui_set_slider_values(
		t_preset_slider *slider, int value, int min, int max)
{
	char	*slide_str;

	slider->value = value;
	slider->min = min;
	slider->max = max;
	slider->button->position.x
		= bui_set_slider_value(value, min,
			max, slider->slider->position.w);
	slide_str = ft_itoa(slider->value);
	bui_set_element_text(slider->button, slide_str,
		slider->button->text_x, slider->button->text_y);
	bui_center_element_text(slider->button);
	ft_strdel(&slide_str);
}

void	preset_slider_events(t_preset_slider *slider)
{
	t_bui_libui	*libui;
	char		*slide_str;
	int			half_butt;

	libui = slider->slider->libui;
	if (bui_button_allow_duplicate(slider->button)
		|| bui_button(slider->slider))
	{
		half_butt = slider->button->position.w * 0.5f;
		slider->button->position.x = ft_clamp(libui->mouse_x
				- slider->slider->screen_pos.x - half_butt, 0,
				slider->slider->position.w - slider->button->position.w);
		slider->value = bui_get_slider_value(slider->min, slider->max,
				slider->button->position.x, slider->slider->position.w
				- slider->button->position.w);
		slide_str = ft_itoa(slider->value);
		bui_set_element_text(slider->button, slide_str,
			slider->button->text_x, slider->button->text_y);
		bui_center_element_text(slider->button);
		ft_strdel(&slide_str);
	}
}
