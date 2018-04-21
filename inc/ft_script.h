/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_script.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 16:48:31 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/21 07:26:00 by sgardner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SCRIPT_H
# define FT_SCRIPT_H
# include <errno.h>
# include <stdio.h>
# include <sys/time.h>
# include "libft.h"

typedef struct termios	t_termios;
typedef struct winsize	t_winsize;
typedef struct timeval	t_timeval;

# define PNAME			"ft_script"
# define OPTIONS		"adFpqr"
# define ERRMSG			sys_errlist[errno]

# define DEFAULT_FILE	"typescript"
# define DEFAULT_SHELL	"/bin/sh"
# define SHELL_KEY		"SHELL"

/*
** -a	O_APPEND
** -d	Ignore timing information in typescript
** -F	Flushes immediately after writing, without timing information
** -p	Plays back typescript
** -q	Quiet mode; no status messages
** -r	Record
*/

enum			e_htype
{
	INPUT = 'i',
	END = 'e',
	OUTPUT = 'o',
	START = 's'
};

typedef struct	s_header
{
	size_t		size;
	time_t		tv_sec;
	int			tv_usec;
	int			type;
}				t_header;

/*
** script [-adpqr] [-F pipe] [file [command ...]]
*/

typedef struct	s_session
{
	char *const	*env;
	char		**av;
	char		*file;
	int			fd;
	int			master;
	t_bool		append: 1;
	t_bool		flush: 1;
	t_bool		instant: 1;
	t_bool		quiet: 1;
	t_bool		record: 1;
}				t_session;

/*
** pty.c
*/

int				ft_forkpty(int *amaster, t_termios *term, t_winsize *ws);
int				ft_login_tty(int fd);
int				ft_openpty(int *amster, int *aslave, t_termios *term,
					t_winsize *ws);

/*
** record.c
*/

void			record_session(t_session *s);

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

const char		*build_path(const char *path, const char *app);
char			*get_env(char *const *env, const char *key);
int				script_err(const char *pre, const char *err, const char *arg);
#endif
