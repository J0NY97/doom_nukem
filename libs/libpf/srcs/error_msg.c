/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 14:50:02 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/08 15:51:53 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libpf.h"
#include <fcntl.h>

void	error_msg(const char *restrict format, ...)
{
	t_pf	p;
	char	buff[PF_BUFF_SIZE];

	write(1, "\x1b[31m[ERROR]\x1b[00m", 17);
	if (format)
	{
		write(1, "\t", 1);
		pf_init(&p, 1);
		p.buffer = buff;
		va_start(p.ap, format);
		pf_read_format((char *)format, &p);
		write(p.fd, p.buffer, p.chars);
		va_end(p.ap);
	}
	else
		write(1, "\n", 1);
	exit(1);
}

//void	pf_error(const char *file, const char *func, int line,
//		const char *restrict format, ...)
//{
//	t_pf	p;
//	char	buff[PF_BUFF_SIZE];
//
//	ft_printf("\e[0;31m[ERROR]\e[0m %s : %s : %d\n", file, func, line);
//	if (format)
//	{
//		write(1, "\t", 1);
//		pf_init(&p, 1);
//		p.buffer = buff;
//		va_start(p.ap, format);
//		pf_read_format((char *)format, &p);
//		write(p.fd, p.buffer, p.chars);
//		va_end(p.ap);
//	}
//	write(1, "\n", 1);
//	exit(1);
//}
//
//void	pf_info(const char *file, const char *func, int line,
//		const char *restrict format, ...)
//{
//	t_pf	p;
//	char	buff[PF_BUFF_SIZE];
//
//	ft_printf("\e[0;36m[INFO]\e[0m %s : %s : %d\n", file, func, line);
//	if (format)
//	{
//		write(1, "\t", 1);
//		pf_init(&p, 1);
//		p.buffer = buff;
//		va_start(p.ap, format);
//		pf_read_format((char *)format, &p);
//		write(p.fd, p.buffer, p.chars);
//		va_end(p.ap);
//	}
//	write(1, "\n", 1);
//}
