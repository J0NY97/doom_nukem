/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_specials.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/15 10:15:12 by nneronin          #+#    #+#             */
/*   Updated: 2021/08/21 10:49:05 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libpf.h"

static const t_pf_style	g_pf_styles[11] =
{
	{"{RESET}", "\x1b[0m", 7},
	{"{RED}", "\x1b[31m", 5},
	{"{GREEN}", "\x1b[32m", 7},
	{"{YELLOW}", "\x1b[33m", 8},
	{"{BLUE}", "\x1b[34m", 6},
	{"{MAGENTA}", "\x1b[35m", 9},
	{"{CYAN}", "\x1b[36m", 6},
	{"{BOLD}", "\x1b[1m", 6},
	{"{ITALIC}", "\x1b[3m", 8},
	{"{UND}", "\x1b[4m", 5},
	{"{INVERT}", "\x1b[7m", 8}
};

static const t_pf_emoji	g_pf_emoji[23] =
{
	{"{OK}", 0x1F44C, 4},
	{"{ERROR}", 0x26D4, 7},
	{"{UP}", 0x2B06, 4},
	{"{DOWN}", 0x2B07, 6},
	{"{RIGHT}", 0x27A1, 7},
	{"{LEFT}", 0x2B05, 6},
	{"{CHECK}", 0x2705, 7},
	{"{WARNING}", 0x26A0, 9},
	{"{TALK}", 0x1F4AC, 6},
	{"{NERD}", 0x1F913, 6},
	{"{NEUTRAL}", 0x1f610, 9},
	{"{TEARS}", 0x1f602, 7},
	{"{HAHA}", 0x1f606, 6},
	{"{FROWN}", 0x1f627, 7},
	{"{FEAR}", 0x1f628, 6},
	{"{SCREAM}", 0x1f631, 8},
	{"{DEAD}", 0x1f635, 6},
	{"{THINKING}", 0x1f914, 10},
	{"{PRAY}", 0x1f64f, 6},
	{"{POOP}", 0x1f4a9, 6},
	{"{ROCKET}", 0x1f680, 8},
	{"{BOMB}", 0x1f4a3, 6},
	{"{SKULL}", 0x2620, 7}
};

static int	color_code(t_pf *p, int i)
{
	int	len;
	int	code;

	len = 0;
	code = ft_atoi(p->format + i);
	if (code >= 0 && code <= 9)
		len = 1;
	else if (code >= 10 && code <= 99)
		len = 2;
	else if (code >= 100 && code <= 255)
		len = 3;
	if (len == 0 || !ft_strnequ((p->format + i + len), "}", 1))
		return (0);
	if (p->fd < 3)
	{
		fill_buffer(p, "\x1b[38;5;", 7);
		fill_buffer(p, p->format + i, len);
		fill_buffer(p, "m", 1);
	}
	p->format += i + len + 1 - 1;
	return (1);
}

static void	fill_buffer_1(t_pf *p, char s)
{
	if (p->print_len != -1)
		p->print_len += 1;
	if (p->chars + 1 > MAX_INT)
		p->print_len = -1;
	if (p->chars + 1 >= PF_BUFF_SIZE)
	{
		write(p->fd, p->buffer, p->chars);
		p->chars = 0;
	}
	if (1 < PF_BUFF_SIZE)
	{
		p->buffer[p->chars] = s;
		p->chars++;
	}
	else
		write(p->fd, &s, 1);
}

static void	ft_wchar(t_pf *p, wchar_t wc)
{
	if (p->fd >= 3)
		return ;
	if (wc <= 0x7F)
		fill_buffer_1(p, (char)wc);
	else if (wc <= 0x7FF)
	{
		fill_buffer_1(p, (char)(0xC0 | (wc >> 6)));
		fill_buffer_1(p, (char)(0x80 | (wc & 0x3F)));
	}
	else if (wc <= 0xFFFF)
	{
		fill_buffer_1(p, (char)(0xE0 | (wc >> 12)));
		fill_buffer_1(p, (char)(0x80 | ((wc >> 6) & 0x3F)));
		fill_buffer_1(p, (char)(0x80 | (wc & 0x3F)));
	}
	else
	{
		fill_buffer_1(p, (char)(0xF0 | (wc >> 18)));
		fill_buffer_1(p, (char)(0x80 | ((wc >> 12) & 0x3F)));
		fill_buffer_1(p, (char)(0x80 | ((wc >> 6) & 0x3F)));
		fill_buffer_1(p, (char)(0x80 | (wc & 0x3F)));
	}
}

static void	pf_style(t_pf *p, int i)
{
	i = -1;
	if (ft_strnequ(&*p->format, "{CLR:", 5) && color_code(p, 5))
		return ;
	while (++i < 11)
	{
		if (ft_strnequ(g_pf_styles[i].str, &*p->format, g_pf_styles[i].len))
		{
			if (p->fd < 3)
				fill_buffer(p, g_pf_styles[i].code, 5);
			p->format += g_pf_styles[i].len - 1;
			return ;
		}
	}
	i = -1;
	while (++i < 23)
	{
		if (ft_strnequ(g_pf_emoji[i].str, &*p->format, g_pf_emoji[i].len))
		{
			ft_wchar(p, g_pf_emoji[i].code);
			p->format += g_pf_emoji[i].len - 1;
			return ;
		}
	}
	fill_buffer(p, p->format, 1);
}

void	pf_specials(t_pf *p)
{
	if (*p->format == '{' && (&*p->format + 1) && ft_isupper(*(p->format + 1)))
		pf_style(p, -1);
	else
		fill_buffer(p, p->format, 1);
}
