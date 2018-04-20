/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_script.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgardner <stephenbgardner@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 16:48:31 by sgardner          #+#    #+#             */
/*   Updated: 2018/04/20 00:34:58 by sgardner         ###   ########.fr       */
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
	suseconds_t	tv_usec;
	int			type;
}				t_header;

/*
** script [-adpqr] [-F pipe] [file [command ...]]
*/

typedef struct	s_session
{
	char *const	*env;
	char *const	*cmd;
	char		*shell;
	char		*file;
	int			fd;
	int			append: 1;
	int			flush: 1;
	int			instant: 1;
	int			quiet: 1;
	int			record: 1;
}				t_session;

/*
** main.c
*/

int				script_err(const char *pre, const char *err, const char *arg);

/*
** pty.c
*/

int				ft_forkpty(int *amaster, t_termios *term, t_winsize *ws);
int				ft_login_tty(int fd);
int				ft_openpty(int *amster, int *aslave, t_termios *term,
					t_winsize *ws);

/*
** session.c
*/

void			start_session(t_session *s);

/*
** term.c
*/

t_termios		*get_raw_term(void);
t_winsize		*get_winsize(void);
void			toggle_echo(int fd);
#endif
