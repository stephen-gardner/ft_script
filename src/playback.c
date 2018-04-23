/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   playback.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/21 00:53:07 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/23 04:32:20 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_script.h"

static void		sleep_diff(t_timespec *prev_time, t_timespec *curr_time)
{
	if ((prev_time->tv_sec = curr_time->tv_sec - prev_time->tv_sec) < 0)
	{
		prev_time->tv_sec = 0;
		prev_time->tv_nsec = 0;
	}
	else if ((prev_time->tv_nsec = curr_time->tv_nsec - prev_time->tv_nsec) < 0)
	{
		if (prev_time->tv_sec)
		{
			--prev_time->tv_sec;
			prev_time->tv_nsec += 1000000000;
		}
		else
			prev_time->tv_nsec = 0;
	}
	nanosleep(prev_time, NULL);
}

static t_bool	process_log(t_session *s, t_header *header, char *data)
{
	static t_timespec	prev_time;
	t_timespec			curr_time;

	curr_time.tv_sec = header->tv_sec;
	curr_time.tv_nsec = header->tv_usec * 1000;
	if (header->type == OUTPUT)
	{
		if (!FL(INSTANT))
			sleep_diff(&prev_time, &curr_time);
		write(STDOUT_FILENO, data, header->size);
		prev_time = curr_time;
	}
	else if (header->type == INPUT)
		;
	else if (header->type == START || header->type == END)
	{
		if (!FL(QUIET))
			write_timestamp(s, header->tv_sec, (header->type == START));
		prev_time = curr_time;
	}
	else
		return (FALSE);
	return (TRUE);
}

void			playback_session(t_session *s)
{
	t_header	header;
	char		buf[BUFF_SIZE];
	int			bytes;

	while (TRUE)
	{
		if ((bytes = read(s->fd, &header, sizeof(t_header))) < 0)
			return ((void)script_err(PNAME, ERRMSG, NULL));
		if (!bytes)
			break ;
		if (bytes < (int)sizeof(t_header)
			|| (size_t)read(s->fd, buf, header.size) < header.size
			|| !process_log(s, &header, buf))
			return ((void)script_err(PNAME, "invalid header", NULL));
	}
}
