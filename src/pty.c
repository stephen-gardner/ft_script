/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pty.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/19 00:46:13 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/19 05:10:22 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "ft_script.h"

int	ft_forkpty(int *amaster, t_termios *termp, t_winsize *winp)
{
	int	master;
	int	slave;
	int	pid;

	if (ft_openpty(&master, &slave, termp, winp) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid)
	{
		close(slave);
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

int	ft_openpty(int *amaster, int *aslave, t_termios *termp, t_winsize *winp)
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
	if (termp)
		ioctl(slave, TIOCSETA | TCSAFLUSH, termp);
	if (winp)
		ioctl(slave, TIOCSWINSZ, winp);
	return (0);
}
