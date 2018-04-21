/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 16:55:22 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/21 00:57:19 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "ft_script.h"

int			term_setraw(t_bool mode)
{
	static t_termios	orig;
	t_termios			raw;

	if (mode)
	{
		if (ioctl(STDIN_FILENO, TIOCGETA, &orig) < 0)
			return (-1);
		raw = orig;
		raw.c_cflag &= ~(CSIZE | PARENB);
		raw.c_cflag |= CS8;
		raw.c_iflag &= ~(
			IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
		raw.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
		raw.c_oflag &= ~OPOST;
		return (ioctl(STDIN_FILENO, TIOCSETAF, &raw));
	}
	return (ioctl(STDIN_FILENO, TIOCSETAF, &orig));
}

t_winsize	*get_winsize(void)
{
	static t_winsize	ws;

	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) < 0)
		return (NULL);
	return (&ws);
}
