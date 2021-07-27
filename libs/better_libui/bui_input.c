#include "better_libui.h"

char	*remove_from_input(t_bui_element *input)
{
	int		len;
	char	*new_str;

	len = 0;
	new_str = NULL;
	if (input->text)
		len = ft_strlen(input->text);
	if (len > 0)
		new_str = ft_strndup(input->text, len - 1);
	return (new_str);
}

char	*add_to_input(t_bui_element *input)
{
	t_bui_libui	*libui;
	char		*new_str;

	libui = input->libui;
	if (input->text)
		new_str = ft_strdup(input->text);
	else
		new_str = ft_strnew(0);
	if (key_pressed(libui, KEY_SPACE))
		ft_straddchar(&new_str, ' ');
	else
	{
		if (alpha_pressed(libui) || number_pressed(libui))
		{
			if (!key_pressed(libui, KEY_LSHIFT))
				ft_straddchar(&new_str,
					ft_tolower(libui->last_key[0]));
			else
				ft_stradd(&new_str, libui->last_key);
		}
	}
	return (new_str);
}

int	bui_input(t_bui_element *input)
{
	t_bui_libui	*libui;
	char		*new_str;
	int			len;

	libui = input->libui;
	if (bui_button_toggle(input))
	{
		if (libui->last_key != NULL)
		{
			if (key_pressed(libui, KEY_RETURN))
			{
				input->toggle = 0;
				return (1);
			}
			else if (key_pressed(libui, KEY_BACKSPACE))
				new_str = remove_from_input(input);
			else
				new_str = add_to_input(input);
			bui_change_element_text(input, new_str);
			ft_strdel(&new_str);
		}
	}
	return (0);
}
