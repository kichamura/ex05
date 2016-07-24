/*
 * server.c
 *
 *  Created on: Jul 23, 2016
 *      Author: sayaka
 */


/*
 * server.c
 *
 *  Created on: 2016/06/28
 *      Author: b4122024
 */
#include "mynet.h"
#include "water_cooler.h"
#include <stdio.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

#define L_USERNAME 15

#define DEFAULT_PORT 50001

#define S_BUFSIZE 512
#define R_BUFSIZE 512
#define MESG_BUFSIZE 488

#define TIMEOUT_SEC 3

#define HELLO 1
#define HERE 2
#define JOIN 3
#define POST 4
#define MESSAGE 5
#define QUIT 6

static void action_received(int signo);
static int N_client = 0;
static int udp_sock;

int server(char *username, int port)
{
	printf("launch server\n");
	int tcp_sock;
	char s_buf[S_BUFSIZE], r_buf[R_BUFSIZE], buf[MESG_BUFSIZE];
	int strsize;
	fd_set mask, readfds;
	packet r_packet;

	struct sigaction action;
#ifdef __CYGWIN__
	int enable=1;
#endif

	printf("launch TCP\n");
	tcp_sock = init_tcpserver(port, N_client);
	printf("launch UDP\n");
	udp_sock = init_udpserver((in_port_t)port);

	//ビットマスクの準備
	FD_ZERO(&mask);
	FD_SET(tcp_sock, &mask);

	/* シグナルハンドラを設定する */
	action.sa_handler = action_received;
	if(sigfillset(&action.sa_mask) == -1)
	{
		exit_errmesg("sigfillset()");
	}
	action.sa_flags = 0;
	if(sigaction(SIGIO, &action, NULL) == -1)
	{
		exit_errmesg("sigaction()");
	}
	if(fcntl(udp_sock, F_SETOWN, getpid()) == -1)
	{
		exit_errmesg("fcntl():F_SETOWN");
	}

	/* ソケットをノンブロッキング, 非同期モードにする */
#ifdef __CYGWIN__
	if(fcntl(udp_sock, F_SETFL, O_NONBLOCK) == -1)
	{
		exit_errmesg("fcntl():F_SETFL");
	}
	if(ioctl(udp_sock, FIOASYNC, &enable) == -1)
	{
		exit_errmesg("ioctl():FIOASYNC");
	}
#else
	if(fcntl(udp_sock, F_SETFL, O_NONBLOCK|O_ASYNC) == -1)
	{
		exit_errmesg("fcntl():F_SETFL");
	}
#endif

	for(;;)
	{
		/* 受信データの有無をチェック */
		readfds = mask;
		printf("select\n");
		select(tcp_sock+1, &readfds, NULL, NULL, NULL);

		if( FD_ISSET(0, &readfds) )
		{
			/* キーボードから文字列を入力する */
			fgets(buf, MESG_BUFSIZE, stdin);
			//s_buf = add_username(buf, username);

			create_packet(MESSAGE, s_buf);
			strsize = strlen(s_buf);
			Send(tcp_sock, s_buf, strsize, 0);
		}

		if( FD_ISSET(tcp_sock, &readfds) )
		{
			/* サーバから文字列を受信する */
			//POST
			strsize = Recv(tcp_sock, r_buf, R_BUFSIZE-1, 0);
			r_packet = re_packet(r_buf);
			if(analize_header(r_packet.header) == POST)
			{
				r_buf[strsize] = '\0';
			}
			if(analize_header(r_packet.header) == QUIT)
			{
				printf("logout\n");
				//ろぐあうと処理
			}
			else
			{
				continue;
			}

			//発言者付加
			create_packet(MESSAGE, r_buf);
			//クライアント送信
		}
	}

	return 0;

}

static void action_received(int signo) // UDPの方の通信
{
	struct sockaddr_in from_adrs;
	socklen_t from_len;
	char r_buf[R_BUFSIZE], s_buf[S_BUFSIZE];
	int strsize;
	//int sock;

	packet udp_packet;

	for(;;)
	{
		/* 文字列をクライアントから受信する */
		from_len = sizeof(from_adrs);
		if((strsize = recvfrom(udp_sock, r_buf, R_BUFSIZE, 0, (struct sockaddr *)&from_adrs, &from_len)) == -1)
		{
			if(errno == EWOULDBLOCK)
			{
				break;
			}
			else
			{
				exit_errmesg("recvfrom()");
			}
		}

		udp_packet = re_packet(r_buf);

		if(analize_header(udp_packet.header) == HELLO)
		{
			create_packet(HERE, s_buf);
			Sendto(udp_sock, s_buf, strsize, 0, (struct sockaddr *)&from_adrs, sizeof(from_adrs));
		}
	}
	return;
}
