/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   record.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 00:20:41 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/19 05:16:27 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	av[0] = s->shell;
	av[1] = 0;
	if (ft_forkpty(&master, NULL, NULL))
	{
		fd_set	fds;

		while (TRUE)
		{
			FD_ZERO(&fds);
			FD_SET(STDIN_FILENO, &fds);
			FD_SET(master, &fds);
			select(master + 1, &fds, NULL, NULL, NULL);
			if (FD_ISSET(STDIN_FILENO, &fds))
			{
				if ((bytes = read(STDIN_FILENO, buf, 4096)))
					write(master, buf, bytes);
			}
			if (FD_ISSET(master, &fds))
			{
				if ((bytes = read(master, buf, 4096)))
					write(STDOUT_FILENO, buf, bytes);
			}
		}
	}
	else
		execve(s->shell, av, s->env);
}
