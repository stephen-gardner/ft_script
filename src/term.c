/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 16:55:22 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/20 02:02:19 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "ft_script.h"

void		toggle_echo(int fd)
{
	t_termios	term;

	if (ioctl(fd, TIOCGETA, &term) < 0)
		return ;
	term.c_lflag &= ~(ECHO | ICANON | ISIG);
	ioctl(fd, TIOCSETAF, &term);
}

t_termios	*get_raw_term(void)
{
	static t_termios	term;

	if (ioctl(STDIN_FILENO, TIOCGETA, &term) < 0)
		return (NULL);
	term.c_cflag &= ~(CSIZE | PARENB);
	term.c_cflag |= CS8;
	term.c_iflag &= ~(
		IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	term.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	term.c_oflag &= ~OPOST;
	return (&term);
}

t_winsize	*get_winsize(void)
{
	static t_winsize	ws;

	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) < 0)
		return (NULL);
	return (&ws);
}
