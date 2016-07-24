/*
 * client.c
 *
 *  Created on: 2016/06/28
 *      Author: b4122024
 */

#include "mynet.h"
#include "water_cooler.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

#define S_BUFSIZE 512
#define R_BUFSIZE 512

#define HELLO 1
#define HERE 2
#define JOIN 3
#define POST 4
#define MESSAGE 5
#define QUIT 6

int client(char *username, struct sockaddr_in server)
{
	int port = server.sin_port;
	char servername[100];
	int sock;
	char s_buf[S_BUFSIZE], r_buf[R_BUFSIZE];
	int strsize;

	fd_set mask, readfds;

	packet r_packet;
	int head;

	sprintf(servername, "%d", server.sin_addr.s_addr);
	sock = init_tcpclient(servername, port);
	Send(sock, "JOIN", 4, 0);

	FD_ZERO(&mask);
	FD_SET(0, &mask);
	FD_SET(sock, &mask);

	for(;;)
	{
		readfds = mask;
		select(sock+1, &readfds, NULL, NULL, NULL);

		if(FD_ISSET(0, &readfds)) // 送信処理
		{
			fgets(s_buf, S_BUFSIZE, stdin);

			//s_bufがQUITの処理
			if((strstr(s_buf, "QUIT")) != NULL)
			{
				create_packet(QUIT, s_buf);
			}
			else // POST付加
			{
				create_packet(POST, s_buf);
			}

			strsize = strlen(s_buf);
			Send(sock, s_buf, strsize, 0);
		}

		if(FD_ISSET(sock, &readfds)) // 受信処理
		{
			strsize = Recv(sock, r_buf, R_BUFSIZE-1, 0);

			/* サーバから文字列を受信する */
			if(strsize == 0)
			{
				printf("Server is down.\n");
				exit(EXIT_FAILURE);
			}
			r_buf[strsize] = '\0';

			r_packet = re_packet(r_buf);
			//パケット処理：MESG
			head = analize_header(r_packet.header);
			if(head == MESSAGE)
			{
				printf("%s",r_buf);
				fflush(stdout);
			}
		}
	}

	/* Never Reach */
	return 0;
}
