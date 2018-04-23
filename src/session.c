/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 00:20:41 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/23 04:48:59 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>
#include "ft_script.h"

static int	open_log(t_session *s)
{
	int	flags;

	if (FL(PLAYBACK))
		flags = O_RDONLY;
	else
	{
		flags = O_WRONLY | O_CREAT;
		flags |= ((FL(APPEND)) ? O_APPEND : O_TRUNC);
	}
	if ((s->fd = open(s->file, flags, 0644)) < 0)
		return (script_err(PNAME, s->file, ERRMSG));
	return (0);
}

static void	log_command(t_session *s)
{
	t_iovec	out[3];

	out[0].iov_base = "command: ";
	out[0].iov_len = 9;
	out[1].iov_base = s->av[0];
	out[1].iov_len = LEN(s->av[0]);
	out[2].iov_base = "\n";
	out[2].iov_len = 1;
	writev(s->fd, out, 3);
}

static void	report_status(t_session *s, int status)
{
	t_timeval	tstamp;

	notice(s, status);
	if (!FL(TIMED))
	{
		gettimeofday(&tstamp, NULL);
		write_timestamp(s, tstamp.tv_sec, status);
	}
}

void		start_session(t_session *s)
{
	if (open_log(s) < 0)
		return ;
	if (FL(PLAYBACK))
		playback_session(s);
	else
	{
		if (!FL(QUIET))
			report_status(s, 1);
		if (FL(COMMAND) && !FL(TIMED))
			log_command(s);
		if (record_session(s) < 0)
			return ;
		if (!FL(QUIET))
			report_status(s, 0);
	}
	close(s->fd);
}
