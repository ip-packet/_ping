#ifndef _PING_H
# define _PING_H

#define _GNU_SOURCE

# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <netdb.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/select.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netinet/ip_icmp.h>

# define	PK_SIZE	4000

# define UND "\033[4m"
# define CYN "\x1B[36m"
# define NRM "\x1B[0m"
# define WHT "\x1B[37m"

typedef	struct {
	int		sock;
	struct	addrinfo	*dest;
	char		*dest_ip;
	int		sent_packs;
	int		recv_packs;
	int		is_on;
	struct	timeval	st;
}	_data;

extern	_data	g_vars;

void		_ping(struct timeval *);

#endif
