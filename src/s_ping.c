# include "s_ping.h"

unsigned	short csum(unsigned short *buff, int words_n) {
	unsigned	short	sum = 0;
	for (; words_n > 0; words_n--)
		sum += *buff++;
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return	(unsigned short)(~sum);
}

void	_ping(struct timeval *tv)
{
	char		packet[PK_SIZE], sec_packet[PK_SIZE];
	struct	icmphdr	in_icmphdr;
	socklen_t		a_si = sizeof(struct addrinfo);
	uint16_t		h_id = htons(getpid());
	size_t		icmphdr_size = sizeof(struct icmphdr);
	struct	timeval	en = {0}, st = {0};
	int		seq = 0, _i, _j;
	fd_set		r_set;

	for (;;) {
		struct	icmphdr *icmp_hdr = (struct icmphdr*)packet;
		memset(packet, 0, sizeof(packet));
		icmp_hdr->type = ICMP_ECHO;
		icmp_hdr->un.echo.id = h_id;
		icmp_hdr->un.echo.sequence = seq++;
		icmp_hdr->checksum = csum((unsigned short *)packet, icmphdr_size/2);

		_i = sendto(g_vars.sock, packet, icmphdr_size, 0, g_vars.dest->ai_addr, a_si);
		if (_i < 0)	{ perror("sendto"); break; }
		printf("%s%s\t%s%sicmp_echo%s %s : ", WHT, UND, NRM, WHT, NRM, g_vars.dest_ip);
		g_vars.is_on = 1;

		FD_ZERO(&r_set);
		FD_SET(g_vars.sock, &r_set);
		gettimeofday(&st, NULL);
		_i = select(g_vars.sock + 1, &r_set, NULL, NULL, tv);
		if (_i < 0)	{ perror("select"); break; }
		if (_i) 		g_vars.sent_packs += 1;
		else {
			printf("## timeout (%is) retrying in 1s..\n", tv->tv_sec);
			sleep(1);
			continue ;
	         	}
		gettimeofday(&en, NULL);
		sleep(1);
		_j = 0;
		memset(packet, 0, sizeof(packet));
		while (_j < icmphdr_size) {
			memset(sec_packet, 0, sizeof(sec_packet));
			_i = recvfrom(g_vars.sock, sec_packet, PK_SIZE, 0, g_vars.dest->ai_addr, &a_si);
			if (_i < 0) { perror("recvfrom"); break; }
			else if (!_i) {
				if (_j < icmphdr_size) {
					printf("## invalid response\n");
					continue ;
				}
				break ;
			}
			memcpy(packet + _j, sec_packet, _i);
			_j += _i;
		}
		g_vars.is_on = 0;
		if (_i < 0) break ;
		g_vars.recv_packs += 1;

		memset(&in_icmphdr, 0, icmphdr_size);
		memcpy(&in_icmphdr, packet + sizeof(struct iphdr), icmphdr_size);
		printf("%s%s%s t=%0.3f ms, echo.id=%i, icmp_seq=%i\n", WHT,
			(in_icmphdr.type == ICMP_ECHOREPLY) ? "icmp_echoreply": "icmp_reply", NRM,
			((double)(en.tv_sec - st.tv_sec) * 1000)+((double)(en.tv_usec - st.tv_usec) / 1000),
			in_icmphdr.un.echo.id, in_icmphdr.un.echo.sequence);
	}
}
