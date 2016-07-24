/*
 ============================================================================
 Name        : water_cooler.c
 Author      : Kitamura Sayaka b4122024
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "mynet.h"
#include "water_cooler.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

#define L_USERNAME 15
#define DEFAULT_PORT 50001

#define S_BUFSIZE 512
#define R_BUFSIZE 512

#define TIMEOUT_SEC 1

#define HELLO 1
#define HERE 2
#define JOIN 3
#define POST 4
#define MESSAGE 5
#define QUIT 6

int main(int argc, char *argv[])
{
	struct sockaddr_in broadcast_adrs;
	struct sockaddr_in from_adrs;
	socklen_t from_len;

	int sock, count = 0;
	int broadcast_sw = 1;
	fd_set mask, readfds;
	struct timeval timeout;

	int port = DEFAULT_PORT;
	char username[L_USERNAME] = "JohnDou";

	char s_buf[S_BUFSIZE], r_buf[R_BUFSIZE];
	int strsize = 0;
	packet r_packet;

	/* コマンドライン引数のチェックおよび使用法の表示 */
	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s User_name, Port_number\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	sprintf(username, "%s", argv[1]);
	port = atoi(argv[2]);
	set_sockaddr_in_broadcast(&broadcast_adrs, (in_port_t)port);
	sock = init_udpclient();

	if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *)&broadcast_sw, sizeof(broadcast_sw)) == 1)
	{
		exit_errmesg("setsockopt()");
	}

	FD_ZERO(&mask);
	FD_SET(sock, &mask);
	// s_buf にHELLOパケットを格納
	create_packet(HELLO, s_buf);
	strsize = sizeof(s_buf);

	int i;
	for(i = 0; i < 3; i++)
	{
		Sendto(sock, s_buf, strsize, 0, (struct sockaddr *)&broadcast_adrs, sizeof(broadcast_adrs));

		readfds = mask;
		timeout.tv_sec = TIMEOUT_SEC;
		timeout.tv_usec = 0;

		if(select(sock+1, &readfds, NULL, NULL, &timeout) == 0)
		{
			count++;
			printf("Send HELLO packets %d times.\n", count);
			printf("pct : %s\n", s_buf);
			if(count == 3)
			{
				printf("There are no server, so you are server.\n");
				server(username, port);
			}
			continue;
		}

		// HEREパケットを受信したときの処理
		from_len = sizeof(from_adrs);
		strsize = Recvfrom(sock, r_buf, R_BUFSIZE-1, 0, (struct sockaddr *)&from_adrs, &from_len);
		r_packet = re_packet(r_buf);

		if(analize_header(r_packet.header) == HERE)
		{
			printf("There are server, so you are client.\n");
			client(username, from_adrs);
		}
	}

	return 0;
}
