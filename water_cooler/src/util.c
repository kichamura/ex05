/*
 * util.c
 *
 *  Created on: Jul 23, 2016
 *      Author: sayaka
 */


/*
 * util.c
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

#define MSG_BUFSIZE 488
#define L_USERNAME 15

static int N_client;
static imember *Client;
static imember *head;
int sock_accepted;

//クライアントのログイン処理を行う関数
int client_login(int sock_listen, char *username)
{
	imember login_client;

	if((login_client = (imember)malloc(sizeof(imember))) == NULL)
	{
		exit_errmesg("malloc()");
	}

	sock_accepted = Accept(sock_listen, NULL, NULL);
	sprintf(login_client->username, "%s", username);
	login_client->sock = sock_accepted;

	*head = login_client;
	(*head)->next = NULL;

	if((*Client)->next != NULL)
	{
		login_client->next = *Client;
		Client = head;
	}

	return 0;
}

//クライアントのログアウト処理を行う関数
int client_logout()
{
	//削除するクライアントをソケットから特定
	//ポインタつなぎなおし
	//free()
	//////
	return 0;
}

//発言したクライアントを除く他の全クライアントに対してメッセージを送信する関数
void send_message(int sock, char *message)
{
	int i;
	for(i = 0; i < N_client; i++)
	{
		/*if(Client->sock == sock)
		{
			continue;
		}*/
		Send(Client[i]->sock, message, strlen(message),0);
	}
}

//発言メッセージの冒頭にユーザ名の付加処理を行う関数
char *add_username(char *buf, char *username)
{
	sprintf(buf, "[ %s ] : %s", username, buf);
	return buf;
}

//文字列に改行コードがあれば取り除く関数
char *chop_nl(char *s)
{
	int len;
	len = strlen(s);
	if(s[len-1] == '\n')
	{
		s[len-1] = '\0';
	}
	return(s);
}

// 受信バッファを構造体に格納しなおす関数
/*void re_packet(char *r_buf)
{
	char *p;

	snprintf(pct.header, 5, "%s", r_buf);

	if((p = strchr(r_buf+4, (int)' ')) != NULL || (p = strchr(r_buf+4, (int)'0')) != NULL)
	{
		printf("p:%s\n", p);
		if(p == r_buf+4)
		{
			pct.sep = *p;
		}
	}

	sprintf(pct.data, "%s", r_buf+5);
}*/


