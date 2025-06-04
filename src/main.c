#include <_ping.h> 

_data	g_vars = {
	.sock = -1,
	.dest = NULL,
	.dest_ip = NULL,
	.sent_packs = 0,
	.recv_packs = 0,
	.is_on = 0,
	.st = {-1}
};


void	signal_handler(int sig_num) {
	if (g_vars.sock > 0) close(g_vars.sock);
	if (g_vars.dest) freeaddrinfo(g_vars.dest);
	printf("\nquiting..\n");
	if (g_vars.dest_ip) {
		printf("=== %s stats ===\n", g_vars.dest_ip);
		printf("packets sent: %i, packet received: %i\n", g_vars.sent_packs - g_vars.is_on, g_vars.recv_packs);
		if (g_vars.st.tv_sec != -1) {
			struct	timeval	en = {0};
			gettimeofday(&en, NULL);
			printf("time: %0.3f ms\n", ((double)((en.tv_sec - g_vars.st.tv_sec) - g_vars.recv_packs) * 1000)
				+ ((double)(en.tv_usec - g_vars.st.tv_usec) / 1000));
		}
	}
	exit(sig_num);
}

void	print_usage() {
	printf("_PING\n");
	printf("- simple ping program to test the connectivity\n\tof various hosts on the internet\n");
	printf("USAGE:\n");
	printf("./_ping DES [time]\n");
	printf("\tDES: ip address of the target host or its FQDN\n");
	printf("\ttime: optional timeout for the host response in seconds, default 3\n");
	exit(1);
}

int	is_digit(char *str) {
	for (int i=0; str[i]; i++)
		if (str[i] < '0' || str[i] > '9')	return 0;
	return 1;
}

struct	addrinfo	*getAddr(char *host, char **dest) {
	struct	addrinfo		hints;
	struct	addrinfo		*res;
	int			_i;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags = AI_PASSIVE;
	_i = getaddrinfo(host, NULL, &hints, &res);
	if (_i != 0 || !res) {
		printf("host unreachable: (%s)\n", host);
		if (strlen(host) < 3)	print_usage();
		exit(1);
	}
	*dest = inet_ntoa(((struct sockaddr_in*)res->ai_addr)->sin_addr);
	printf("%s (%s)\n", host, *dest);
	return	res;
}

int		main(int c, char **v) {

	if (c < 2 || c > 3 || (c==3 && !is_digit(v[2])))
		print_usage();

	struct	timeval	tv = {0};
	int	_op;

	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);

	g_vars.sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g_vars.sock < 0) {
		perror("socket");
		exit(1);
	}
	g_vars.dest = getAddr(v[1], &g_vars.dest_ip);
	tv.tv_sec = (c == 3) ? atoi(v[2]) : 3;

	_op = IPTOS_LOWDELAY;
	setsockopt(g_vars.sock, IPPROTO_IP, IP_TOS, &_op, sizeof(_op));
	_op = 0;
	setsockopt(g_vars.sock, IPPROTO_IP, IP_HDRINCL, &_op, sizeof(_op));
	gettimeofday(&g_vars.st, NULL);

	_ping(&tv);
	signal_handler(0);
}

