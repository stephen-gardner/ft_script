/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_script.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 16:48:31 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/24 20:25:06 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SCRIPT_H
# define FT_SCRIPT_H
# include <errno.h>
# include <stdio.h>
# include <sys/time.h>
# include "libft.h"

typedef struct iovec	t_iovec;
typedef struct termios	t_termios;
typedef struct timeval	t_timeval;
typedef struct timespec	t_timespec;
typedef struct winsize	t_winsize;

# define PNAME			"ft_script"
# define OPTIONS		"adFkpqr"
# define BUFF_SIZE		65536
# define ERRMSG			sys_errlist[errno]
# define FL(x)			(s->flags & (x))

# define DEFAULT_FILE	"typescript"
# define DEFAULT_SHELL	"/bin/sh"
# define SHELL_KEY		"SHELL"

enum			e_scriptflg
{
	APPEND = 1,
	COMMAND = 1 << 1,
	FLUSH = 1 << 2,
	INSTANT = 1 << 3,
	KEYLOG = 1 << 4,
	PLAYBACK = 1 << 5,
	QUIET = 1 << 6,
	TIMED = 1 << 7
};

enum			e_htype
{
	INPUT = 'i',
	END = 'e',
	OUTPUT = 'o',
	START = 's'
};

typedef struct	s_opt
{
	int		flag;
	int		val;
	int		conflicts;
}				t_opt;

typedef struct	s_header
{
	size_t		size;
	time_t		tv_sec;
	suseconds_t	tv_usec;
	int			type;
}				t_header;

typedef struct	s_session
{
	char *const	*env;
	char		**av;
	char		*file;
	int			fd;
	int			master;
	int			flags;
}				t_session;

/*
** playback.c
*/

void			playback_session(t_session *s);

/*
** pty.c
*/

int				ft_forkpty(int *amaster, int *aslave, t_termios *term,
					t_winsize *ws);
int				ft_login_tty(int fd);
int				ft_openpty(int *amster, int *aslave, t_termios *term,
					t_winsize *ws);

/*
** record.c
*/

int				record_session(t_session *s);

/*
** session.c
*/

void			start_session(t_session *s);

/*
** term.c
*/

t_winsize		*get_winsize(void);
int				term_setraw(t_bool mode);

/*
** util.c
*/

char			*get_env(char *const *env, const char *key);
void			notice(t_session *s, int status);
int				script_err(const char *pre, const char *err, const char *arg);
void			write_timestamp(t_session *s, time_t uet, int status);
#endif
