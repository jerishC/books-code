/*!
 * \file fig17.35.h
 * \brief The opend.h
 * \author Jianlong Chen <jianlong99@gmail.com>
 * \date 2013-12-24
 */
/* $Id$ */

#ifndef FIG17_35_H
#define FIG17_35_H

#include "apue.h"
#include <errno.h>

#define CS_OPEN "/home/cjl/opend"	/* well-known name */
#define CL_OPEN "open"				/* client's request for server */

extern int	debug;					/* nonzero if interactive (not daemon) */
extern char errmsg[];				/* error message string to return to client */
extern int	oflag;					/* open flag: O_xxx ... */
extern char *pathname;				/* of file to open for client */

typedef struct {	/* one Client struct per connected client */
	int		fd;		/* fd, or -1 if available */
	uid_t	uid;
} Client;

extern Client	*client;	/* ptr to malloc'ed array */
extern int		client_size;/* # entries in client[] array */

int		cli_args (int, char **);
int		client_add (int, uid_t);
void	client_del (int);
void	loop (void);
void	request (char *, int, int, uid_t);

#endif /* !FIG17_35_H */
