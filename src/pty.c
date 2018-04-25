/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pty.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 00:46:13 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/24 21:36:30 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "ft_script.h"

/*
** Parent process would normally close slave fd here, but we need it for
** window resizing...
*/

int	ft_forkpty(int *amaster, int *aslave, t_termios *term, t_winsize *ws)
{
	int	master;
	int	slave;
	int	pid;

	if (ft_openpty(&master, &slave, term, ws) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid)
	{
		*aslave = slave;
		*amaster = master;
		return (pid);
	}
	close(master);
	if (ft_login_tty(slave) < 0)
		script_err("forkpty", "unable to set pty as controlling tty", NULL);
	return (0);
}

int	ft_login_tty(int fd)
{
	int	res;

	setsid();
	res = ioctl(fd, TIOCSCTTY);
	dup2(fd, STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
	close(fd);
	return (res);
}

int	ft_openpty(int *amaster, int *aslave, t_termios *term, t_winsize *ws)
{
	int		master;
	int		slave;
	char	sname[128];

	if ((master = open("/dev/ptmx", O_RDWR | O_NOCTTY)) < 0)
		return (-1);
	if (ioctl(master, TIOCPTYGRANT) < 0
		|| ioctl(master, TIOCPTYUNLK) < 0
		|| ioctl(master, TIOCPTYGNAME, sname) < 0
		|| (slave = open(sname, O_RDWR | O_NOCTTY)) < 0)
	{
		close(master);
		return (-1);
	}
	*amaster = master;
	*aslave = slave;
	if (term)
		ioctl(slave, TIOCSETAF, term);
	if (ws)
		ioctl(slave, TIOCSWINSZ, ws);
	return (0);
}
