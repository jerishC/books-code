/*
 * \file ex16.5.c
 * \brief Server program illustrating command writing directly to socket
 * \author Jianlong Chen <jianlong99@gmail.com>
 * \date 2013-12-15
 */
/* $Id$ */

#include "apue.h"
#include "fig16.20.h"
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define QLEN 10

#ifndef HOST_NAME_MAX
#	define HOST_NAME_MAX 256
#endif

void serve (int sockfd)
{
	int		clfd;
	pid_t	pid;

	for (;;) {
		clfd = accept (sockfd, NULL, NULL);
		if (clfd < 0) {
			syslog (LOG_ERR, "ruptimed: accept error: %s",
					strerror (errno));
			exit (1);
		}
		if ((pid = fork ()) < 0) {
			syslog (LOG_ERR, "ruptimed: fork error: %s",
					strerror (errno));
			exit (1);
		} else if (pid == 0) {	/* child */
			/*
			 * The parent called daemonize (Figure 13.1), so
			 * STDIN_FILENO, STDOUT_FILENO, and STDERR_FILENO
			 * are already open to /dev/null. Thus, the call to
			 * close doesn't need to be protected by checks that
			 * clfd isn't already equal to one of these values.
			 */
			if (dup2 (clfd, STDOUT_FILENO) != STDOUT_FILENO ||
					dup2 (clfd, STDERR_FILENO) != STDERR_FILENO) {
				syslog (LOG_ERR, "ruptimd: unexpected error");
				exit (1);
			}
			close (clfd);
			execl ("/usr/bin/uptime", "uptime", (char *)0);
			syslog (LOG_ERR, "ruptimed: unexpected return from exec: %s",
					strerror (errno));
		} else { /* parent */
			close (clfd);
		}
	}
}

void sigchld (int signo)
{
	while (waitpid ((pid_t)-1, NULL, WNOHANG) > 0)
		;
}

int main (int argc, char *argv[])
{
	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	int				sockfd, err, n;
	char			*host;

	if (argc != 1) {
		err_quit ("usage: ruptimed");
	}
#ifdef _SC_HOST_NAME_MAX
	n = sysconf (_SC_HOST_NAME_MAX);
	if (n < 0) /* best guess */
#endif
		n = HOST_NAME_MAX;
	host = malloc (n);
	if (host == NULL) {
		err_sys ("malloc error");
	}
	if (gethostname (host, n) < 0) {
		err_sys ("gethostname error");
	}
	daemonize ("ruptimed");
	if (SIG_ERR == signal (SIGCHLD, sigchld)) {
		err_sys ("signal error");
	}
	hint.ai_flags = AI_CANONNAME;
	hint.ai_family = 0;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if ((err = getaddrinfo (host, "ruptime", &hint, &ailist)) != 0) {
		syslog (LOG_ERR, "ruptimed: getaddrinfo error: %s",
				gai_strerror (err));
		exit (1);
	}
	for (aip = ailist; aip != NULL; aip = aip->ai_next) {
		if ((sockfd = initserver (SOCK_STREAM, aip->ai_addr,
						aip->ai_addrlen, QLEN)) >= 0) {
			serve (sockfd);
			exit (0);
		}
	}
	exit (1);
}
