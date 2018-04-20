/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 00:20:41 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/20 15:14:11 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <sys/ioctl.h>
#include <termios.h>
#include <sys/select.h>
#include <unistd.h>
#include "ft_script.h"
/*
static t_bool	open_log(t_session *s)
{
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (s->append)
		flags |= O_APPEND;
	if ((s->fd = open(s->file, flags, 0644)) < 0)
	{
		script_err(PNAME, s->file, ERRMSG);
		return (FALSE);
	}
	return (TRUE);
}*/

void			start_session(t_session *s)
{
	char	*av[2];
	char	buf[4096];
	int		master;
	int		bytes;
	fd_set	fds;

	av[0] = s->shell;
	av[1] = 0;
	if (ft_forkpty(&master, NULL, get_winsize()))
	{
		toggle_raw(TRUE);
		while (TRUE)
		{
			FD_ZERO(&fds);
			FD_SET(STDIN_FILENO, &fds);
			FD_SET(master, &fds);
			select(master + 1, &fds, NULL, NULL, NULL);
			if (FD_ISSET(STDIN_FILENO, &fds))
			{
				if ((bytes = read(STDIN_FILENO, buf, 4096)) <= 0)
					break ;
				write(master, buf, bytes);
			}
			if (FD_ISSET(master, &fds))
			{
				if ((bytes = read(master, buf, 4096)) <= 0)
					break ;
				write(STDOUT_FILENO, buf, bytes);
			}
		}
		close(master);
		toggle_raw(FALSE);
	}
	else
		execve(s->shell, av, s->env);
}
