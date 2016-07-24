/*
 * water_cooler.h
 *
 *  Created on: 2016/06/28
 *      Author: b4122024
 */

#ifndef WATER_COOLER_H_
#define WATER_COOLER_H_

#define L_USERNAME 15
#define MESG_BUFSIZE 488

typedef struct idobata
{
	char header[4];
	char sep;
	char data[];
} packet;

typedef struct _imember
{
	char username[L_USERNAME];
	int sock;
	struct _imember *next;
} *imember;

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int Send(int s, void *buf, size_t len, int flags);
int Recv(int s, void *buf, size_t len, int flags);
packet re_packet(char *r_buf);
void create_packet(u_int32_t type, char *message);
u_int32_t analize_header( char *header );
char *add_username(char *buf, char *username);
int server(char *username, int port);
#endif /* WATER_COOLER_H_ */
